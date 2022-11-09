<div id="inicio">
    <h1>PROBLEMA 2 TEC 499 Sitemas Digitas</h1>
</div>
<div id="sumario">
    <h1>Sumário</h1>
	<ul>
        	<li><a href="#equipe"> <b>Equipe de Desenvolvimento</b></li>
          	<li><a href="#inicio"> <b>Introdução</b></li>
		<li><a href="#recursos-utilizados"> <b>Recursos Utilizados</b> </a></li>
        	<li><a href="#desenvolvimento"> <b>Descrição em alto nível do sistema proposto</b> </a> </li>
        	<li><a href="#protocolo"> <b>Descrição do protocolo de comunicação desenvolvido;</b> </a> </li>
          <li><a href="#simulacao"> <b>Descrição e análise dos testes e simulações realizadas.;</b> </a> </li>
		      <li><a href="#anexos"> <b>Anexos</b> </a></li>
	</ul>	
</div>

<div id="equipe">
    <h1>Equipe de Desenvolvimento</h1>
    <ul>
		<li><a href="https://github.com/VitorVA6"> Vitor Vaz Andrade </li>
		<li><a href="https://github.com/ViniciusDJM"> Vinícius Dias de Jesus Maciel </a></li>
	</ul>
    <h1>Tutor</h1>
    <ul>
        <li><a href="https://github.com/thiagocj">Thiago Cerqueira de Jesus</a></li>
    </ul>
</div>



<div id="Introdução">
     <h1> Introdução </h1>
     <ul>
	Para o problema 2, foi nos entregue o desafio de dar prosseguimento ao desenvolvimento do protótipo de um sistema digital baseado em um processador ARM, o próximo passo compreenderia então a implementação de um protótipo de sistema de sensoriamento genérico.
	
Visando a realização do mesmo, foi usada uma plataforma baseada no NodeMCU como pilar para a confecção das unidades de sensoriamento, visando a criação de um ecossistema de Internet das Coisas (IoT).
	
Como tarefa tivemos que fazer um sistema que pudesse ser comandado por um Single Board Computer e que deveria ser capaz de controlar o acionamento de um conjunto variável de sensores, assim como monitorar o seu funcionamento de maneira automatizada. As operações de leitura deveriam ser representadas por um código para que o sistema embarcado na NodeMCU pudesse ser capaz de interpretá-los e realizá-los de maneira adequada, por meio da comunicação UART.
	</ul>
</div>
<div id="recursos-utilizados">
    <h1>Recursos utilizados</h1>
    <ul>
    A solução do problema foi desenvolvida para funcionar em uma Raspberry Pi Zero, utilizando como periférico de saída um display LCD Hitachi HD44780U (LCD-II) de 16x2. A Raspberry em questão possui arquitetura ARMv6 de 32bits.

Este projeto também utiliza uma ESP8266 (NodeMCU) conectada a sensores de umidade e temperatura analógicos e digitais. Porém por limitações laboratoriais os sensores digitais são emulados por push buttons e o analógico por um potenciômetro.

Para comunicação com os periféricos, tanto da NodeMCU quanto da Raspberry PI, foi utilizada a biblioteca HardwareSerial.h, enquanto que a utilização da UART ocorreu através dos comandos de controle e utilização oferecidos pela biblioteca TermIOs. Todos os códigos utilizados foram produzidos em linguagem C. Também foi necessária a utilização da biblioteca ArduinoOTA para possibilitar o carregamento de código na NodeMCU via Wifi.
    </ul>
</div>


<div id="como-executar">
    <h1>Como executar o projeto</h1> 
    <h3>Rapsberry PI:</h3>
    <ul>
    Para iniciar a execução é necessário clonar o projeto da máquina que será usada. Para isso segue o passo a passo:
		
	$ https://github.com/VitorVA6/PBL2_SD
• Transfira os seguintes arquivos baixados para o Raspberry Pi Zero;

	clear.s
	writeLCD.s
	initLCD.s
	main.c
	makefile
	mapping.s

• Por fim, execute os seguintes comandos:
</ul>

	$ make
	$ sudo ./main
  
<h3>Node MCU:</h3>
    <ul>
    Para tudo funcionar bem o ambiente deve-se estar minimamente preparado, com todos os recursos citados na seção de recursos utilizados em mãos em conjunto com os códigos presentes neste repositório. Além disso é necessário a inserção de alguns dados em linhas de código específicas, pois imaginando-se o cenário de upload via wifi o código presente nos trechos do ArduinoOTA devem ficar desse modo:
    </ul>
    
    const char* ssid = "SSID-DA-REDE-A-SER-UTILIZADA";
    const char* password = "SENHA-DA-REDE-A-SER-UTILIZADA";
    
    void setup() {
	  Serial.begin(9600);
	  Serial.println("Booting");
	  WiFi.mode(WIFI_STA);
	  WiFi.begin(ssid, password);
	  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
	    Serial.println("Connection Failed! Rebooting...");
	    delay(5000);
	    ESP.restart();
	  }

	  ArduinoOTA.setHostname("ENDEREÇO-DA-ESP-A-SER-UTILIZADA");

	   ArduinoOTA.onStart([]() {
	    String type;
	    if (ArduinoOTA.getCommand() == U_FLASH) {
	      type = "sketch";
	    } else { // U_FS
	      type = "filesystem";
	    }

	  ArduinoOTA.begin();
    
</div>
<div id="desenvolvimento">
    <h1>Descrição em alto nível do sistema proposto</h1> 
    <ul>
<p>
Em resumo a solução apresentada neste repositório consiste no envio de informações via UART da Raspberry Pi para a NodeMCU, a depender de qual tipo de informação for recebida o projeto apresentará uma ação de acordo com a mesma. 

O principal fluxo de troca de informações é a leitura das informações vinda dos sensores e seu imediato envio via UART para a Rapsberry Pi que deverá exibir o dado na display. Outros fluxos podem ser resumidos em checagem de status da própria NODEMcu e o ligar/desligar do led, em todos os casos existe o envio de informação para a Raspberry e sua consequente exibição no LCD.
	    
O projeto em questão conseguiu atender todos os requisitos listados no problema para o SBC: Capacidade de interligação com até 32 sensores, mecanismo de controle de status de funcionamento dos sensores, Apenas o SBC será capaz de iniciar uma comunicação. 

Além disso, o projeto também cumpre todas as requisições propostas para o NodeMCU como: Deverá ser capaz de ler e interpretar comandos oriundos do SBC, Os comandos serão compostos por palavras de 8 bits, As requisições do SBC são compostas de 2 bytes (Comando + Endereço do sensor), Os resultados deverão ser exibidos no display LCD usando a biblioteca desenvolvida anteriormente.

</p>
	  
Ilustração do sistema realizado em diagrama de blocos:
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/200962092-483609dc-aad2-4f7b-8ada-161ba77d8b3f.png width="700px" />
	</div>
     </ul>	
	
</div>

<div id="protocolo">
<h1>Descrição do protocolo de comunicação desenvolvido;</h1>
	<p>
	Como protocolo de comunicação a ser desenvolvido, foram feitas 2 tabelas com o propósito de melhorar a descrição do protocolo em si, uma tabela será o protocolo da Rasp para a ESP e a segunda tabela sera da ESP para a Rasp.
	Por propósito de esclarecimento, a tabela de protocolo de Rasp para a ESP pode também ser lida como Tabela de Requisições e a de ESP para Rasp como Tabela de Resposta. 
	</p>
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/200955146-d509b820-b4d3-4656-8cc1-4882ddbfd8d8.jpg width="500px" />
	</div>
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/200955463-9fc81621-c1f0-496c-98dd-feef86d8e0b1.jpg width="500px" />
	</div>
</div>



<div id="simulacao">
<h1> Descrição e análise dos testes e simulações realizadas.</h1> 
	<p>
	Como primeiro teste do Problema 2, foi realizado um loopback, ou seja, a Raspberry recebe o mesmo dado que está enviando, o teste em questão tem como propósito verificar se o funcionamento da UART atendia aos padrões de qualidade do projeto ao receber corretamente um dado especificado. 
	Com a garantia do resultado satisfatório do teste do loopback pudemos avançar para outros testes mais voltados aos resultados finais que o problema exigia, são eles: 
	<ul>
	<li>Teste para a requisição do status da Node:</li> 
	O teste foi concluído com êxito apresentando a mensagem esperada no planejamento do projeto.
	</p>
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/200957819-442341a2-8f05-427d-8a39-38cc78f85fbc.jpeg width="350px" />
	</div>
	<p>
	<li>Teste para a requisição do sensor de entrada analógica:</li> 
	Para o teste em questão foi realizado várias medições com voltagens distintas para garantir que o valor de voltagem seria alterado para mais ou para menos à medida que manipulamos o sensor. 
	</p>
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/200958499-df8b5559-a8f9-488f-9eb7-5aeddf13b9c9.jpeg width="350px" />
	</div>
	<p>
	<li>Teste para a requisição do valor de uma das entradas digitais:</li> 
	Para esse teste tivemos que mapear os sensores para os 2 botões disponíveis, enviamos requisições para quando o botão não estava sendo pressionado e o mesmo devolvia a resposta esperada que seria o "Sensor: High", já quando o botão fosse pressionado recebíamos "Sensor: Low", concluindo os testes para este requisito.
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/200960003-b895a2a7-c7aa-4e16-bc65-4fc08a479bb8.jpeg width="350px" />
	</div>
	<p>
	<li>Teste para Ligar/Desligar led:</li> 
	Por fim o último teste foi realizado manipulando o led da nodeMCU, além da resposta ao comando para tal ser representada pelo ligar e desligar da própria led foi também exibido na LCD o estado da led, se estava ligada ou desligada. O teste garantiu o funcionamento desta etapa com êxito. 
	<div align="center">
	<img src=https://user-images.githubusercontent.com/29680023/200960437-940cc2c1-b583-4e8a-8d1a-116cfc2f6070.jpeg width="350px" />
	</div>
	</ul>
	</p>
</div>

<div id="anexos">
<h1> Materiais utilizados no desenvolvimento</h1> 
</div>

&nbsp;&nbsp;&nbsp;[ESP8266](https://arduino-esp8266.readthedocs.io/en/latest/)

&nbsp;&nbsp;&nbsp;[Comunicação Serial](https://www.arduino.cc/reference/en/language/functions/communication/serial/)

&nbsp;&nbsp;&nbsp;[UART - Rasp](https://raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart)

</div>


<hr/>
</div>




















