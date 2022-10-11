#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

extern void mapping();
extern void initLcd();
extern void clear();
extern void write(int a);

void delay(int seconds){
    int mili = 1000*seconds;
    clock_t start_time = clock();
    while(clock() < start_time + mili);
}

void write_string(char word[]){

    for (int i=0; i<strlen(word); i++){
        write(word[i]);
    }
}

int main(){

    mapping();
    initLcd();

    write_string("testando...");
    
    
}