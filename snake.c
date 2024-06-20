#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void game_over(WINDOW** win, int score);
int game_start(WINDOW** win);
void readFile(WINDOW** win);

int menu(WINDOW** win){
  refresh();
  const char* opcoes[] = {"Jogar","Pontuações","Sair"};
  int select = 0;
  int escolha;
  int score;
                       
  while(true){
    erase();
    box(*win, 0, 0);
    mvprintw(2,(COLS - 41) / 2," ____    _   _      _      _  __  _____");
    mvprintw(3,(COLS - 41) / 2,"/ ___|  | \\ | |    / \\    | |/ / | ____|");
    mvprintw(4,(COLS - 41) / 2,"\\___ \\  |  \\| |   / _ \\   | ' /  |  _|  ");
    mvprintw(5,(COLS - 41) / 2," ___) | | |\\  |  / ___ \\  | . \\  | |___ ");
    mvprintw(6,(COLS - 41) / 2,"|____/  |_| \\_| /_/   \\_\\ |_|\\_\\ |_____|");
    for (int i = 0; i < 3; i++){
      if (i == select){wattron(*win, A_REVERSE);}
      mvprintw((LINES  / 2) + i + 2, (COLS  / 2) - 3, "%s", opcoes[i]);
      wattroff(*win, A_REVERSE);
    }
    escolha = wgetch(*win);
    switch (escolha)
    {
      case KEY_UP:
        if (select == 0){continue;}
        select--;
        break;
      case KEY_DOWN:
        if (select == 2){continue;}
        select++;
        break;
      default:
        break;
    }
    if (escolha == 10 || escolha == KEY_ENTER){
      switch (select)
      {
        case 0:
          score = game_start(win);
          erase();
          refresh();
          game_over(win, score);
          break;
        case 1:
          readFile(win);
          break;
        case 2:
          endwin();
          return 0;
      }
    }
  }
  return 0;
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

void readFile(WINDOW** win){
  erase();
  box(*win, 0, 0);
  FILE* file = fopen("saves.txt", "r");
  mvprintw(2, (COLS - 33) / 2, "   ___    ___    ___    ___   ___ ");
  mvprintw(3, (COLS - 33) / 2, "  / __|  / __|  / _ \\  | _ \\ | __|");
  mvprintw(4, (COLS - 33) / 2, "  \\__ \\ | (__  | (_) | |   / | _| ");
  mvprintw(5, (COLS - 33) / 2, "  |___/  \\___|  \\___/  |_|_\\ |___|");
  mvprintw(7, (COLS / 2) - 9, "Pontuação");
  mvprintw(7, (COLS / 2) + 7, "Nome");

  if (file != NULL){
    int i = 0;  int ascii; char c; char names[10] = {' '}; char values[10] = {' '};
    int flagDash = 0;
    int iterNames = 0; int iterValues = 0;
    while (true){
      while((ascii = fgetc(file)) != EOF){
        c = (char)ascii;
        if (c == '-'){
          flagDash = 1;
          int fScore = atoi(values);
          if (i < 7){
          mvprintw(9 + i, ((COLS) / 2) - 5, "%d", fScore);}

        }
        else if (flagDash == 0){
          values[iterValues] = c;
          iterValues++; 
        }
        else if (flagDash == 1 && c != '\n' && c != '\r'){
          names[iterNames] = c;
          iterNames++;
        }
        else if(c == '\n'){
          if (i < 7){
            mvprintw(9 + i, ((COLS) / 2) + 5, "%s", names);
          }
          i++;
          iterNames = 0; iterValues = 0;
          flagDash = 0;
          memset(names, '\0', sizeof names);
          memset(values, '\0', sizeof values);
          wrefresh(*win);
        }  
      }
      mvprintw(10 + i, (COLS/ 2) - 1, "Sair");
      wattron(*win, A_REVERSE);
      int key = wgetch(*win);
      if (key == 10){
        break;
      }
    }
  }
  erase();
  refresh();
  fclose(file);
}

void saveInFile(FILE* read, char* string, int replaceLine, int linhasTotais){
  fseek(read, 0, SEEK_SET);
  FILE* fileWrite = fopen("temp__saves.txt", "w");
  char linha[24];

  for (int i = 1; i < linhasTotais; ++i){
    fgets(linha, 24, read);
    if (i == replaceLine){
      fputs(string, fileWrite);
    }
    else{
      fputs(linha, fileWrite);
    }
  }
  if (replaceLine == -1){
    fputs(string, fileWrite);
  }
  fclose(read);
  fclose(fileWrite);
  remove("saves.txt");
  rename("temp__saves.txt", "saves.txt");
}

int saveScore(WINDOW** win, int score){
  erase();
  box(*win, 0, 0);
  mvprintw(2, (COLS - 33) / 2, "   ___    ___    ___    ___   ___ ");
  mvprintw(3, (COLS - 33) / 2, "  / __|  / __|  / _ \\  | _ \\ | __|");
  mvprintw(4, (COLS - 33) / 2, "  \\__ \\ | (__  | (_) | |   / | _| ");
  mvprintw(5, (COLS - 33) / 2, "  |___/  \\___|  \\___/  |_|_\\ |___|");
  mvprintw(7, (COLS / 2) - 9, "Pontuação");
  mvprintw(7, (COLS / 2) + 7, "Nome");
  FILE* file = fopen("saves.txt", "r");
  char c; char names[12] = {'\0'}; char values[11] = {'\0'};
  char buf3[24] = {'\0'}; int w = 0; int pos; int line = 1; int replaceLine = -1;
  int ascii;
  if (file != NULL){
    int i = 0;
    int flagDash = 0;  int flagScorePrinted = 0;
    int iterNames = 0; int iterValues = 0;
    while((ascii = fgetc(file)) != EOF){
      c = (char)ascii;
      if (c == '-'){
        flagDash = 1;
        int fScore = atoi(values);
        if (score > fScore && flagScorePrinted != 1){
          mvprintw(9 + i, ((COLS) / 2) - 5, "%d", score);
          pos = i;
          i++;
          replaceLine = line;
          flagScorePrinted = 1;
        }
        if (i < 6)
        {
          mvprintw(9 + i, ((COLS) / 2) - 5, "%d", fScore);
        }
      }
      
      else if (flagDash == 0){
        values[iterValues] = c;
        iterValues++; 
      }
      else if (flagDash == 1 && c != '\n' && c != '\r'){
        names[iterNames] = c;
        iterNames++;
      }
      else if(c == '\n'){
        if (i < 6){
          mvprintw(9 + i, (COLS/2) + 5, "%s", names);
          i++;
        }
        line++;
        iterNames = 0; iterValues = 0;
        flagDash = 0;
        memset(names, '\0', sizeof (names));
        memset(values, '\0', sizeof (values));
      }  
    }
    if (flagScorePrinted == 0){
      mvprintw(9 + i, ((COLS) / 2) - 5, "%d", score);
      pos = i;
    }
    mvprintw(12 + i, (COLS / 2) - 16, "<Pressione Enter para Confirmar>");
    while (true){
      int key = wgetch(*win);
      switch (key)
      {
        case 10:
          erase();
          refresh();
          
          char* sbuf = (char*)malloc(24 * sizeof (char)); 
          sprintf(sbuf, "%d-%s\n", score, buf3);
          saveInFile(file, sbuf, replaceLine, line);
          free(sbuf);

          return 0;
        case 127:
        case KEY_BACKSPACE:
          if (w != 0){
            w--;
            buf3[w] = ' ';
          }
          break;
        default:
          if (key != -1 && w < 10){
            buf3[w] = key;
            w++;
          }
          break;
      }
      mvprintw(9 + pos, (COLS / 2) + 5, "%s", buf3);
      refresh();
    }
  }
  erase();
  refresh();
  fclose(file);
  return 0;
}
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
  mvprintw(0,5, "Score: %d", snake->score - 5);
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

void game_over(WINDOW** win, int score){
  erase();
  box(*win, 0, 0);
  mvprintw((LINES / 2), (COLS / 2), "Game Over!");
  usleep(9999);
  saveScore(win, score - 5);
}

int game_start(WINDOW** win){
    int timer = 110000;
    int size = 1;
    int flag = 0;
    snake snake;
    snake.head.x = 10; snake.head.y = 10;
    snake.dir.x = 1; snake.dir.y = 0;
    snake.score = 5;
    snake.body = (pos*)malloc(size * 300 * sizeof(pos));
    pos fruta = gera_fruta(LINES - 1, COLS - 1);
    char head = '>';
    while (true){
    int pressed = wgetch(*win);
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
        dpausar(&snake, win);
        desenhar(win, &snake, head, &fruta, timer);
      case 'q':
        free(snake.body);
        snake.body = NULL;
        return snake.score;
    }

    for(int i = snake.score; i > 0; i--){
      snake.body[i] = snake.body[i - 1];
      if (snake.head.x == snake.body[i].x && snake.head.y == snake.body[i].y){ free(snake.body); snake.body = NULL; return snake.score;}
    }

    snake.body[0] = snake.head;

    snake.head.x += snake.dir.x;
    snake.head.y += snake.dir.y;
    
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

    desenhar(win, &snake, head, &fruta, timer);

    if(snake.head.x == COLS - 1 || snake.head.y == LINES - 1 || snake.head.x == 0 || snake.head.y == 0)
    {
      free(snake.body);
      snake.body = NULL;
      return snake.score;
    }
  }
}

int main(){

    WINDOW* win = initscr();
    FILE* file = fopen("saves.txt", "w");
    fclose(file);
    refresh();
    keypad(win, true);
    nodelay(win, true);
    noecho();
    curs_set(0);

    menu(&win);

    endwin();
    return 0;
}
