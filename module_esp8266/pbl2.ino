/*
Obs: 
  1) LED embutido acende em nivel logico baixo - PIN 2
  2) Não apagar nenhuma função do OTA para não desconfigurar o ESP, não mexer por curiosidade 
  3) B0 e B1 é GPIO 16 e GPIO 5 respectivamente
      
*/

//Bibliotecas
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

unsigned char lvlD0 = 0x01;
unsigned char lvlD1 = 0x01;
unsigned char statusLed = 0x00;
unsigned char comAnswer = 0x00;
float voltage = 0;


void setup() {
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  
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
  if (Serial.available()>0){
    byte com = Serial.read();
    delay(500);
    byte addr = Serial.read();
    //if(Serial.availableForWrite()>0){
    if(com == 0x04){
      comAnswer = 0x01;
      voltage = analogRead(A0)*(3.3/1023.0);
      Serial.write(comAnswer);
      Serial.println(voltage);
    }
    if(com == 0x05){
      comAnswer = 0x02;
      switch(addr){
        case 0x20:{
          if(digitalRead(D0) == 0){
            lvlD0 = 0x00;
          }
          else{
            lvlD0 = 0x01;
          }
          Serial.write(comAnswer);
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

    if(com == 0x06){
      comAnswer = 0x03;
      if (digitalRead(LED_BUILTIN) == 0){
        digitalWrite(LED_BUILTIN, 1);
        statusLed = 0x01;
      }else{
        digitalWrite(LED_BUILTIN, 0);
        statusLed = 0x00;
      }     
      Serial.write(comAnswer);
      Serial.write(statusLed); 
    }

  }
}
