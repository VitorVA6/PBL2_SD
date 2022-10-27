#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

extern void mapping();
extern void initLcd();
extern void clear();
extern void write_lcd(int a);

int uart0_file = -1;

void delay(int seconds){
    int mili = 1000*seconds;
    clock_t start_time = clock();
    while(clock() < start_time + mili);
}

void write_string(char word[]){

    for (int i=0; i<strlen(word); i++){
        write_lcd(word[i]);
    }
}

void setting_uart(){
    
    uart0_file = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    if(uart0_file == -1){
        printf("Erro na abertura da UART\n");
    }

    struct termios options;
    tcgetattr(uart0_file, &options);

    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_file, TCIFLUSH);
    tcsetattr(uart0_file, TCSANOW, &options);
}

void tx_command(unsigned char com, unsigned char addr){
    unsigned char req[10];
    unsigned char *p_req;

    p_req = &req[0];
    *p_req++ = com;
    *p_req++ = addr;

    int cont = write(uart0_file, &req[0], (p_req - &req[0]));

    if(cont < 0){
        printf("Erro no envio de dados\n");
    }
}

void rx_command(){
    unsigned char rx_buffer[100];
    int rx_length = read(uart0_file, (void*)rx_buffer, 100);
    
    if (rx_length < 0){
        printf("Erro na leitura\n");
    }
    else if(rx_length == 0){
        printf("Nenhum dado disponível\n");
    }
    else{
        rx_buffer[rx_length] = '\0';
        write_string(rx_buffer);
        printf("%s", rx_buffer);
    }
}

int main(int argc, const char * argv[]){

    mapping();
    initLcd();
    setting_uart();

    int infinite = 1;
    
    int command = 0;
    int address = 0;
    
    while(infinite){
        printf("Wellcome to Sensor Manager \n");
        printf("Select an operation:\n\n");
        printf("1 -> NodeMCU status;\n");
        printf("2 -> Analogic sensor status; \n");
        printf("3 -> Digital sensor status;\n");
        printf("4 -> Turn on Led;\n");
        printf("5 -> exit;\n\n");
        scanf("%d", &command);
        
        if(command == 5){
            break;
        }
        
        switch(command){
            case 1:{
                tx_command('a', 'b');
                sleep(1);
                rx_command();
                break;
            }
            case 2:{
                tx_command('c', 'd');
                sleep(1);
                rx_command();
                break;
            }
            case 3:{
                tx_command('e', 'f');
                sleep(1);
                rx_command();
                break;
            }
            case 4:{
                tx_command('g', 'h');
                sleep(1);
                rx_command();
                break;
            }
            
            default: {
                printf("Invalid operation\n");
                break;
            }
        }
        
    }

    close(uart0_file);

    return 0;
}