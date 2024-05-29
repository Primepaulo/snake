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

void desenhar(WINDOW** win, snake* snake, char head, pos* fruta){
  erase();
  box(*win, 0, 0);
  for (int i = 0; i < snake->score; i++){
    mvaddch(snake->body[i].y, snake->body[i].x, '*');
  }
  mvaddch(LINES, COLS, 'v');
  mvaddch(snake->head.y, snake->head.x, head);
  mvaddch(fruta->y,fruta->x, 'O');
  usleep(110000);
}


int main(){

    WINDOW* win = initscr();
    keypad(win, true);
    nodelay(win, true);
    curs_set(0);
    int size = 1;
    snake snake;
    snake.head.x = 10; snake.head.y = 10;
    snake.dir.x = 1; snake.dir.y = 0;
    //snake.score = 0;
    snake.score = 30;
    snake.body = (pos*)malloc(size * 300 * sizeof(pos));

    pos fruta = gera_fruta(LINES - 1, COLS - 1);
  
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
            case KEY_END:
                goto appkiller;
        }

        for(int i = snake.score; i > 0; i--){
          snake.body[i] = snake.body[i - 1];
        }

        snake.body[0] = snake.head;

        snake.head.x += snake.dir.x;
        snake.head.y += snake.dir.y;

        if (snake.head.x == fruta.x && snake.head.y == fruta.y){
          snake.score++;
          if (snake.score == size * 300){
            size++;
            snake.body = (pos*)realloc(snake.body, size * 300 * sizeof(pos));
          }
          fruta = gera_fruta(LINES - 1, COLS - 1);
        }
        
        if(snake.head.x == COLS - 1 || snake.head.y == LINES - 1 || snake.head.x == 0 || snake.head.y == 0)
        {
          break;
        }

        for(int i = snake.score; i >= 0; i--){
          if (snake.head.x == snake.body[i].x && snake.head.y == snake.body[i].y){
            break;
          }
        }
        

        desenhar(&win, &snake, head, &fruta);
    }
    appkiller:
      endwin();
      printf("\n X: %d | %d \n Y: %d | %d \n", snake.head.x, COLS, snake.head.y, LINES);
    return 0;
}
