#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


void menu(){
  //menu principal!
}

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

pos gera_fruta(int lines, int cols){
  pos fruta;
  fruta.y = 0; fruta.x = 0;
  while (fruta.y == 0){
    fruta.y = rand() % lines;
  }
  while(fruta.x == 0){
    fruta.x = rand() % cols;
  }
  return fruta;
}

void desenhar(WINDOW** win, snake* snake, char head, pos* fruta, int timer){
  erase();
  box(*win, 0, 0);
  for (int i = 0; i < snake->score; i++){
    mvaddch(snake->body[i].y, snake->body[i].x, '*');
  }
  mvaddch(LINES, COLS, 'v');
  mvaddch(snake->head.y, snake->head.x, head);
  mvaddch(fruta->y,fruta->x, 'O');
  usleep(timer);
}

void dpausar(snake* snake, WINDOW** win){
    erase();
    box(*win, 0, 0);
    mvprintw(1, 5,"headx: %d heady: %d",snake->head.x, snake->head.y);
    for(int i = 0; i < snake->score; i++){
      mvprintw((LINES/2) + i - 5, 5,"%d / %d", snake->body[i].x, snake->body[i].y);
    }
    refresh();
    usleep(9999999);
}

void game_over(){
  //solicitar nome pra salvar score no arquivo
  menu();
}

int main(){

    WINDOW* win = initscr();
    keypad(win, true);
    nodelay(win, true);
    curs_set(0);

    menu();

    int timer = 110000;
    int size = 1;
    snake snake;
    snake.head.x = 10; snake.head.y = 10;
    snake.dir.x = 1; snake.dir.y = 0;
    snake.score = 5;
    snake.body = (pos*)malloc(size * 300 * sizeof(pos));

    pos fruta = gera_fruta(LINES - 1, COLS - 1);
    char head = '>';
    while (true){
        int pressed = wgetch(win);
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
            case KEY_HOME:
              dpausar(&snake, &win);
              desenhar(&win, &snake, head, &fruta, timer);
        }

        for(int i = snake.score; i > 0; i--){
          snake.body[i] = snake.body[i - 1];
          if (snake.head.x == snake.body[i].x && snake.head.y == snake.body[i].y){game_over();}
        }

        snake.body[0] = snake.head;

        snake.head.x += snake.dir.x;
        snake.head.y += snake.dir.y;

        for(int i = 1; i < snake.score; i++)
        {
          if (snake.head.x == snake.body[i].x && snake.head.y == snake.body[i].y)
          { 
            break;
          }
        }

        if (snake.head.x == fruta.x && snake.head.y == fruta.y){
          snake.score++;
          if (snake.score % 3 == 0){
            timer = timer - 5;
          }
          if (snake.score == size * 300){
            size++;
            snake.body = (pos*)realloc(snake.body, size * 300 * sizeof(pos));
          }
          fruta = gera_fruta(LINES - 1, COLS - 1);
        }

        desenhar(&win, &snake, head, &fruta, timer);

        if(snake.head.x == COLS - 1 || snake.head.y == LINES - 1 || snake.head.x == 0 || snake.head.y == 0)
        {
          game_over();
        }
    }
    
    endwin();
    return 0;
}
