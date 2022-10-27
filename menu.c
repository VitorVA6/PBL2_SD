#include <stdio.h>

int main()
{
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
                printf("NodeMCU status;\n");
                break;
            }
            case 2:{
                printf("Analogic sensor status;\n");
                break;
            }
            case 3:{
                printf("Digital sensor status;\n");
                break;
            }
            case 4:{
                printf("Led status;\n");
                break;
            }
            
            default: {
                printf("Invalid operation\n");
                break;
            }
        }
        
    }

    return 0;
}