#include <stdio.h>
#include <stdlib.h>

enum Menu{
    NOVO = 'n',
    SAIR = 'e'
};

enum Directions{
    RIGHT = 'R',
    LEFT = 'L',
    DOWN = 'D',
    UP = 'U'
};

typedef struct pos{
    int x;
    int y;
    int direction;
} pos;

void start(){};

void menu(){
    char buf[4];
    while (buf[0] != SAIR){
        scanf("%4s", buf);
        switch (buf[0])
        {
            case NOVO:
                start();

            case SAIR:
                break;

            default:
                printf("Valor Inválido!");
        }
    }
}

void move(int posX, int posY){
    //while buf != direction => move direction
}

// Função única (em Move) ou múltiplas?
void Up(){}
void Down(){}
void Left(){}
void Right(){}


pos head;
int main(){
    menu();
    head.x=25;
    head.y=25;
    head.direction = RIGHT;
    return 0;
}