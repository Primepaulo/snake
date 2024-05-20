#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct{
  int x;
  int y;
} pos;

typedef struct
{
  pos head;
  pos dir;
  pos* body;
  int score;
} snake;

pos gera_fruta(int rows, int cols){
  pos fruta;
  fruta.x = rand() % rows / 3;
  fruta.y = rand() % cols;
  return fruta;
}


int main(){

    // Iniciar a tela, receber input (ncurses)
    WINDOW* win = initscr();
    keypad(win, true);
    nodelay(win, true);
    curs_set(0);
    int row, col;
    getmaxyx(win, col, row);
    printf("%d and %d", row, col); 
    //Criar a cobra e a primeira fruta
    snake snake;
    snake.head.x = 10; snake.head.y = 10;
    snake.dir.x = 1; snake.dir.y = 0;
    snake.score = 0;
    snake.body = (pos*)malloc(300 * sizeof(pos));

    pos fruta = gera_fruta(row, col);
  
    while (true){
        int pressed = wgetch(win);
        char head;
        switch (pressed){
            case KEY_UP:
                if (snake.dir.y == 1){continue;}
                snake.dir.x = 0;
                snake.dir.y = -1;
                head = '^';
                break;
            case KEY_DOWN:
                if (snake.dir.y == -1){continue;}
                snake.dir.x = 0;
                snake.dir.y = 1;
                head = 'v';
                break;
            case KEY_RIGHT:
                if (snake.dir.x == -1){continue;}
                snake.dir.x = 1;
                snake.dir.y = 0;
                head = '>';
                break;
            case KEY_LEFT:
                if (snake.dir.x == 1){continue;}
                snake.dir.x = -1;
                snake.dir.y = 0;
                head = '<';
                break;
        }

        for(int i = snake.score; i > 0; i--){
          snake.body[i] = snake.body[i - 1];
        }

        snake.body[0] = snake.head;

        snake.head.x += snake.dir.x;
        snake.head.y += snake.dir.y;

        if (snake.head.x == fruta.x && snake.head.y == fruta.y){
          snake.score++;
          fruta = gera_fruta(row, col);
        }

        // Desenhar no terminal (ncurses)
        erase();
        box(win, 0, 0);
        for (int i = 0; i < snake.score; i++){
          mvaddch(snake.body[i].y, snake.body[i].x * 2, '*');
        }
        mvaddch(snake.head.y, snake.head.x * 2, head);
        mvaddch(fruta.y,fruta.x * 2, 'O');
        usleep(100000);

    
    }
    endwin();
    return 0;
}
