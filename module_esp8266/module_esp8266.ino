#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


//Definicoes
#ifndef STASSID
#define STASSID "INTELBRAS"
#define STAPSK  "Pbl-Sistemas-Digitais"
#endif

//Constantes e variaveis
const char* ssid = STASSID;
const char* password = STAPSK;

int nodeStatus = 1;     //Variável que controla o status de funcionamento dos sensores do NodeMCU
unsigned char lvlD0 = 0x01;   //Variável que contém o comando de resposta (Segundo byte) para o status do botão do pino D0
unsigned char lvlD1 = 0x01;   //Variável que contém o comando de resposta (Segundo byte) para o status do botão do pino D1
unsigned char statusLed = 0x00;   //Variável que contém o comando de resposta (Segundo byte) para o status da LED
unsigned char comAnswer = 0x00;   //Variável que contem o comando de resposta geral (primeiro byte) de todas as requisições do SBC
float voltage = 0;                //Variável que guarda o valor em volts, do pino A0


void setup() {
  pinMode(D0, INPUT);  //Configura o pino D0 como INPUT
  pinMode(D1, INPUT);   //Configura o pino D1 como INPUT
  pinMode(LED_BUILTIN, OUTPUT); //Configura o pino da LED como OUTPUT
  pinMode(A0, INPUT);           //Configura o pino A0 como output
  
  Serial.begin(9600);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("ESP-10.0.0.109");

  // No authentication by defaultser
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //digitalWrite(LED_BUILTIN,HIGH);
}


void loop() {
  ArduinoOTA.handle();
  //mySerial.listen();
  if (Serial.available()>0){  //Verifica se há pelo menos 1 byte para leitura
    byte com = Serial.read(); //Lê o primeiro byte do comando
    delay(500);
    byte addr = Serial.read();  //Lê o segundo byte do comando
    //As condicionais abaixo verificam qual é a requisição do SBC, comparando os bytes recebidos de acordo o protocolo criado pela equipe
    if(com == 0x03){            //Verifica o status do NodeMCU, se os sensores estão habilitados ou não
      if(nodeStatus == 1){
        comAnswer = 0x00;
        Serial.write(comAnswer);  //Devolve 0x00 se estiver habilitado
      }
      else{
        comAnswer = 0x1F;
        Serial.write(comAnswer);  //Retorna 0x1F se não estiver
      }
    }
    if(com == 0x04){      //Obtem o valor da entrada analógica A0 e envia para o SBC
      comAnswer = 0x01;   
      voltage = analogRead(A0)*(3.3/1023.0);    //Converte o intervalo da entrada analógica que varia de 0 a 1023, para unidades de volts (de 0 a 3.3 volts)
      Serial.write(comAnswer);        //Envia para o SBC o byte de resposta especificado no protoco para status de sensor analógico e o valor do sensor analógico
      Serial.println(voltage);
    }
    if(com == 0x05){    //Obtém o valor de uma das entradas digitais
      comAnswer = 0x02;
      switch(addr){     //Verifica qual é o sensor desejado, a partir do endereço enviado pelo SBC
        case 0x20:{
          if(digitalRead(D0) == 0){     //Faz a leitura do pino especificado
            lvlD0 = 0x00;
          }
          else{
            lvlD0 = 0x01;
          }
          Serial.write(comAnswer);    //Envia para o SBC o byte de resposta especificado no protoco para status de sensor digital e o valor do sensor digital
          Serial.write(lvlD0);
          break;
        }
      
        case 0x21:{
          if(digitalRead(D1) == 0){
            lvlD1 = 0x00;
          }
          else{
            lvlD1 = 0x01;
          }
          Serial.write(comAnswer);    
          Serial.write(lvlD1);
          break;
        }
      }
    }

    if(com == 0x06){    //Acende ou apaga a LED do NodeMCU
      comAnswer = 0x03;
      if (digitalRead(LED_BUILTIN) == 0){   //Apaga caso esteja acesa
        digitalWrite(LED_BUILTIN, 1);
        statusLed = 0x01;
      }else{
        digitalWrite(LED_BUILTIN, 0);     //Acende caso esteja apagada
        statusLed = 0x00;
      }     
      Serial.write(comAnswer);       //Envia para o SBC o byte de resposta estabelecido no protocolo para controlar a LED e o status da LED
      Serial.write(statusLed);      
    }

    if(com == 0x07){        //Habilita ou desabilita o envio de dados dos sensores do NodeMCU
      if(nodeStatus == 1){
        nodeStatus = 0;
        Serial.write(0x1F);
      }
      else{
        nodeStatus = 1;
        Serial.write(0x00);
      }
    }

  }
}