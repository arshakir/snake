#include <ncurses.h>
#include <iostream>
#include <vector>
using namespace std;
const int SNAKE_LEN=3;
const int NUM_APPLES=10; 

struct snake {
  int vx, vy, x, y;
};

struct apple {
  int x, y;
};

void input(snake &head) {
  int c;
  c = getch();
  if (c == KEY_UP && head.vy == 0) {
    head.vy = -1;
    head.vx = 0;
  } else if (c == KEY_DOWN && head.vy == 0) {
    head.vy = 1;
    head.vx = 0;
  } else if (c == KEY_LEFT && head.vx == 0) {
    head.vx = -1;
    head.vy = 0;
  } else if (c == KEY_RIGHT && head.vx == 0) {
    head.vx = 1;
    head.vy = 0;
  }
}

void eat(vector<snake> &snakes, vector<apple> &apples, int h, int w) {
  for (int i = 0; i < apples.size(); i++) {
    if (snakes[0].x == apples[i].x && snakes[0].y == apples[i].y) {
      snakes.resize(snakes.size() + 1);
      apples[i].x = rand() % (w - 2) + 1;
      apples[i].y = rand() % (h - 2) + 1;
    }
  }
}

void updatesnakes(vector<snake> &snakes) {

  for (int i = snakes.size() - 1; i > 0; i--) {
    snakes[i].x = snakes[i - 1].x;
    snakes[i].y = snakes[i - 1].y;
  }

  snakes[0].x += snakes[0].vx;
  snakes[0].y += snakes[0].vy;
}

bool gameover(vector<snake> &snakes, int h, int w) {
  for (int i = 1; i < snakes.size(); i++) {
    if (snakes[0].x == snakes[i].x && snakes[0].y == snakes[i].y) {
      return true;
    }
  }
  return snakes[0].x < 1 || snakes[0].x > w - 2 || snakes[0].y < 1 ||
         snakes[0].y > h - 2;
}

void print(vector<snake> &snakes, vector<apple> &apples) {
  clear();
  wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
  mvprintw(0,0,"Score: %d",snakes.size()-SNAKE_LEN);
  for (int i = 0; i < apples.size(); i++) {
    mvaddch(apples[i].y, apples[i].x, ACS_BLOCK | COLOR_PAIR(1));
  }
  for (int i = 0; i < snakes.size(); i++) {
    mvaddch(snakes[i].y, snakes[i].x, ACS_BLOCK | COLOR_PAIR(2));
  }
  refresh();
}

int main() {
  srand(time(0));
  initscr();
  noecho();
  keypad(stdscr, true);
  curs_set(0);
  timeout(50);
  start_color();
  int h, w;
  getmaxyx(stdscr, h, w);

  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  vector<snake> snakes(SNAKE_LEN);
  snakes[0].vx = 1;
  snakes[0].vy = 0;
  for (int i = 0; i < snakes.size(); i++) {
    snakes[i].x = (w / 2) - i;
    snakes[i].y = h / 2;
  }

  vector<apple> apples(NUM_APPLES);

  for (int i = 0; i < apples.size(); i++) {
    apples[i].x = rand() % (w - 2) + 1;
    apples[i].y = rand() % (h - 2) + 1;
  }


  while (!gameover(snakes, h, w)) {
    input(snakes[0]);

    eat(snakes, apples, h, w);

    updatesnakes(snakes);

    print(snakes, apples);
  }
  endwin();
}
