<div id="inicio">
    <h1>PROBLEMA 2 TEC 499 Sitemas Digitas</h1>
</div>
<div id="sumario">
    <h1>Sumário</h1>
	<ul>
        	<li><a href="#equipe"> <b>Equipe de Desenvolvimento</b></li>
          	<li><a href="#inicio"> <b>Introdução</b></li>
		<li><a href="#recursos-utilizados"> <b>Recursos Utilizados</b> </a></li>
        	<li><a href="#sistema-proposto"> <b>Sistema proposto para o problema</b> </a> </li>
        	<li><a href="#limitacoes-solucao"> <b>Descrição do protocolo de comunicação desenvolvido;</b> </a> </li>
          <li><a href="#limitacoes-solucao"> <b>Descrição e análise dos testes e simulações realizadas.;</b> </a> </li>
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
	
Visando a realização do mesmo, foi usada uma plataforma baseada no NodeMCU como pilar para a confecção das unidades de sensoriamento, visando a criação de um ecossisstema de Internet das Coisas (IoT).
	
Como tarefa tivemos que fazer um sistema que pudesse ser comandado por um Single Board Computer e que deveria ser capaz de controlar o acionamento de um conjunto variável de sensores, assim como monitorar o seu funcionamento de maneira automatizada. As operações de leitura deveriam ser representadas por um código para que o sistema embarcado na NodeMCU pudesse ser capaz de interpretá-los e realizá-los de maneira adequada, por meio da comunicação UART.
	</ul>
</div>
<div id="recursos-utilizados">
    <h1>Recursos utilizados</h1>
    <ul>
    A solução do problema foi desenvolvida para funcionar em uma Raspberry Pi Zero, utilizando como periférico de saída um display LCD Hitachi HD44780U (LCD-II) de 16x2. A Raspberry em questão possui arquitetura ARMv6 de 32bits.

Este projeto também utiliza uma ESP8266 (NodeMCU) conectada a sensores de umidade e temperatura analógicos e digitais. Porém por limitações laboratoriais os sensores digitais são emulados por push buttons e o analógico por um potenciômetro.

Para comunicação com os periféricos, tanto da NodeMCU quanto da Raspberry PI, foi utilizada a biblioteca --, enquanto que a utilização da UART ocorreu através dos comandos de controle e utilização oferecidos pela biblioteca TermIOs. Todos os códigos utilizados foram produzidos em linguagem C. Também foi necessária a utilização da bibliotca ArduinoOTA para possibilitar o carregamento de código na NodeMCU via Wifi.
    </ul>
</div>

<div id="sistema-proposto">
    <h1>Sistema Proposto</h1> 
   
</div>



<div id="anexos">
<h1>Descrição do protocolo de comunicação desenvolvido;</h1>
</div>



<div id="anexos">
<h1> Descrição e análise dos testes e simulações realizadas.</h1> 
</div>

<div id="anexos">
<h1> Materiais utilizados no desenvolvimento</h1> 
</div>

&nbsp;&nbsp;&nbsp;[Stephen Smith - Raspberry Pi Assembly Language
&nbsp;&nbsp;&nbsp;Programming
](https://link.springer.com/book/10.1007/978-1-4842-5287-1)

&nbsp;&nbsp;&nbsp;[HD44780U (LCD-16x2)](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)

&nbsp;&nbsp;&nbsp;[BCM2835 ARM Peripherals](https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)

&nbsp;&nbsp;&nbsp;[ARM1176JZF-S Technical Reference Manual](https://developer.arm.com/documentation/ddi0301/h?lang=en)

&nbsp;&nbsp;&nbsp;[Linux system Calls - ARM 32bit EABI](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm-32_bit_EABI)
</div>


<hr/>
</div>




















