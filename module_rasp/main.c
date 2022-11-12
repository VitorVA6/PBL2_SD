#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

extern void mapping();  //Biblioteca para mapear a LCD
extern void initLcd();  //Biblioteca para inicializar a LCD
extern void clear();    //Biblioteca para limpar display da LCD
extern void write_lcd(int a);   //Biblioteca para escrever caracter na LCD

int uart0_file = -1;

//Função que recebe uma string como parâmetro e a escreve na LCD
void write_string(char word[]){

    for (int i=0; i<strlen(word); i++){     //Percorre a string, escrevendo caracter a caracter na LCD
        write_lcd(word[i]);                 //Chamada da biblioteca de escrever caracter na LCD 
    }
}

//Função para configuração do UART PL011
void setting_uart(){
    
    uart0_file = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY); //Abrindo o arquivo da UART PL011 e obtendo seu file descriptor

    if(uart0_file == -1){  //Caso o open não tenha aberto o arquivo, retorna -1
        printf("Erro na abertura da UART\n");
    }

    struct termios options;   //É criado uma struct para configurar o funcionamento da UART
    tcgetattr(uart0_file, &options);   //Obtém os parâmetros associados ao descritor de arquivo especificado e os armazena na struct termios de referência

    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;  //#baundrate de 9600, #palabras de 8 bits, #permite leitura
    options.c_iflag = IGNPAR;                        //Ignora bit de paridade   
    options.c_oflag = 0;                                
    options.c_lflag = 0;

    tcflush(uart0_file, TCIFLUSH);  //Função que descarta dados recebidos no file descriptor, que não foram lidos
    tcsetattr(uart0_file, TCSANOW, &options);
}

//Função para envio dos bytes, recebe o byte do comando e o de endereço
void tx_command(unsigned char com, unsigned char addr){
    unsigned char req[10];      //Criação do array de bytes para envio
    unsigned char *p_req;       //Criação de um ponteiro para apontar pro array criado anteriormente

    p_req = &req[0];            //Ponteiro aponta para o endereço do primeiro elemento do array
    *p_req++ = com;             //Coloca o byte do comando na primeira posição array
    *p_req++ = addr;            //Coloca o byte do endereço do sensor na segunda posição do array

    int cont = write(uart0_file, &req[0], (p_req - &req[0]));   //Chama a função que enviar os dados via serial
                                                                //Os parâmetros são o file descriptor, e a quantidade de bytes

    if(cont < 0){   //Se ocorrer um erro no envio, a função write retorn -1
        printf("Erro no envio de dados\n");     
    }
}

//Função para recebimento de dados via serial
unsigned char rx_command(){         
    unsigned char rx_buffer[100];   //Criação do buffer que receberá os dados
    int rx_length = read(uart0_file, (void*)rx_buffer, 100);    //Chamada da função de leitura, os parâmetros são o file descriptor,
                                                                //um ponteiro apontando pro primeiro elemento do buffer e o comprimento do buffer    
    if (rx_length < 0){             //Se der erro na leitura, a função retorna -1
        printf("Erro na leitura\n");
    }
    else if(rx_length == 0){        //Se não ocorrer erro, mas não tiver dados para ler, retorna 0
        printf("Nenhum dado disponível\n");
    }
    else{
        clear();                    //Chamada da biblioteca para limpar o display lcd
        rx_buffer[rx_length] = '\0'; //Coloca o byte '\0' para finalizar a string no segundo byte
        if (rx_buffer[0] == 0x01){      //Verifica se o comando de resposta é o código do sensor analógico
            write_string("Voltage:");   //Escreve "Voltage" na lcd
            rx_buffer[0] = ' ';         //Substitui o primeiro elemento do buffer que é um '0x01' por um ' '
            rx_buffer[strlen(rx_buffer)-1] = ' ';   //Faz o mesmo com o último elemento
            rx_buffer[strlen(rx_buffer)-2] = 'V';   //Susbtitui o penúltimo caracter do buffer por um 'V'
            write_string(rx_buffer);                //Escreve o valor recebido da ESP na lcd
        }
        else if (rx_buffer[0] == 0x02){             //Verifica se o comando de resposta é o código dos sensores digitais
            if (rx_buffer[1] == 0x01){              //Verifica se o segundo byte do buffer indica nível lógico alto
                write_string("Sensor: HIGH");       //Se sim, escreve na LCD "HIGH"
            }
            else if(rx_buffer[1] == 0x00){
                write_string("Sensor: LOW");        //Se não, escreve "LOW"
            }
            else{
                write_string("Sensor: --");
            }
        }
        else if (rx_buffer[0] == 0x03){             //Verifica se o comando de resposta é o código da LED
            if (rx_buffer[1] == 0x01){              //Verifica se o segundo byte do buffer indica nível lógico alto
                write_string("LED Status: OFF");    //Se sim, escreve na LCD "OFF"
            }
            else{
                write_string("LED Status: ON");     //Se não, escreve "ON"
            }
        }
        else if(rx_buffer[0] == 0x1F){
            write_string("NodeMCU: OFF");
        }
        else if(rx_buffer[0] == 0x00){
            write_string("NodeMCU: ON");
        }
    }
}

//Função para obter o endereço do sensor digital desejado pelo usuário
unsigned char get_addr(){
    int sensor = 0;     //Variável que armazena a opção escolhida pelo usuário
    printf("\n\nSelect a sensor: \n");
    printf("1 -> Sensor 1 (PIN D0): \n");
    printf("2 -> Sensor 2 (PIN D1): \n");
    printf("3 -> Sensor 3 (PIN D2): \n");
    printf("4 -> Sensor 4 (PIN D3): \n");
    printf("5 -> Sensor 5 (PIN D4): \n");
    printf("6 -> Sensor 6 (PIN D5): \n");
    printf("7 -> Sensor 7 (PIN D6): \n");
    printf("8 -> Sensor 8 (PIN D7): \n");
    scanf("%d", &sensor);
    switch(sensor){
        case 1:{
            return 0x20;  //Se usuário digitar '1', o endereço do sensor é o 0x20
        }
        case 2:{
            return 0x21;  //Se usuário digitar '2', o endereço do sensor é o 0x21
        }
        case 3:{
            return 0x22;  //Se usuário digitar '3', o endereço do sensor é o 0x22
        }
        case 4:{
            return 0x23;  //Se usuário digitar '4', o endereço do sensor é o 0x23
        }
        case 5:{
            return 0x24;  //Se usuário digitar '5', o endereço do sensor é o 0x24
        }
        case 6:{
            return 0x25;  //Se usuário digitar '6', o endereço do sensor é o 0x25
        }
        case 7:{
            return 0x26;  //Se usuário digitar '7', o endereço do sensor é o 0x26
        }
        case 8:{
            return 0x27;  //Se usuário digitar '8', o endereço do sensor é o 0x27
        }
        default:{
            printf("Invalid sensor\n\n");
            break;
        }
    }
}

int main(int argc, const char * argv[]){

    mapping();      //Chamada da biblioteca de mapeamento
    initLcd();      //Chamada da biblioteca de inicialização da LCD
    setting_uart(); //Chamada da função de configuração do UART

    int infinite = 1;   //Variável para controle do looping do menu
    
    int command = 0;    //Variável que armazena a opção de operação selecionada pelo usário
    unsigned char address = 0;  //Variável que armazena o endereço do sensor digital
    
    while(infinite){
        printf("Wellcome to Sensor Manager \n");
        printf("Select an operation:\n\n");
        printf("1 -> NodeMCU status;\n");
        printf("2 -> Analogic sensor status; \n");
        printf("3 -> Digital sensor status;\n");
        printf("4 -> Turn on/off Led;\n");
        printf("5 -> Enable/Disable NodeMCU\n");  
        printf("6 -> exit;\n\n");
        scanf("%d", &command);
        
        if(command == 6){  //Se for selecionada a opção '5', a aplicação é encerrada
            break;
        }
        
        switch(command){
            case 1:{
                tx_command(0x03, 0x00);  //Envia o código do comando para status da NodeMCU
                sleep(2);
                rx_command();           //Recebe informações
                break;
            }
            case 2:{
                tx_command(0x04, 0x00); //Envia o código do comando para status do sensor analógico
                sleep(2);
                rx_command();
                break;
            }
            case 3:{
                address = get_addr();   //Obtem o endereço do sensor digital especificado pelo usuário
                tx_command(0x05, address);  //Envia o código do comando de status de sersor digital e o endereço do respectivo sensor
                sleep(2);
                rx_command();
                break;
            }
            case 4:{
                tx_command(0x06, 0x00); //Envia o código do comando para acender/desligar a LED
                sleep(2);
                rx_command();
                break;
            }

            case 5:{
                tx_command(0x07, 0x00);
                sleep(2);
                rx_command();
                break;
            }
            
            default: {
                printf("Invalid operation\n");
                break;
            }
        }
        
    }

    close(uart0_file);      //Fecha o arquivo do terminal UART

    return 0;
}