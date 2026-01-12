#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME='q'};
enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=3};

/* Кнопки управления */
struct control_buttons{
	int down;
	int up;
	int left;
	int right;
}control_buttons;

struct control_buttons default_controls = {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT};

/* Голова змеи */
typedef struct snake_t {
	int x;
	int y;
	int direction;
	size_t tsize;
	struct tail_t *tail;
	struct control_buttons controls;
} snake_t ;

/* Тело змеи */
typedef struct tail_t {
	int x;
	int y;
} tail_t;

/* Инициализация тела */
void initTail(tail_t t[], size_t size){
	struct tail_t init_t={0,0};
	for(size_t i=0; i<size; i++) t[i]=init_t;
}

/* Инициализация головы */
void initHead(snake_t *head,int x,int y){
	head->x = x;
	head->y = y;
	head->direction = RIGHT;
}

/* Инициализация змеи */
void initSnake(snake_t *head, size_t size, int x, int y){
	tail_t* tail = (tail_t*)
	malloc(MAX_TAIL_SIZE*sizeof(tail_t));
	
	initTail(tail, MAX_TAIL_SIZE);
	initHead(head, x, y);
	
	head->tail = tail;
	head->tsize = size+1;
	head->controls = default_controls;
}

/* Двтжение головы */
void go(struct snake_t *head){
	char ch = '@';
	mvprintw(head->y, head->x, " "); //очищаем один символ

	int max_x=0, max_y=0;
	getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
	
	switch (head->direction)
	{
		case LEFT:
			if(head->x <= 0) head->x = max_x;
			mvprintw(head->y, --(head->x),"%c", ch);
			break;
		case RIGHT:
			if(head->x >= max_x) head->x = 0;
			mvprintw(head->y, ++(head->x), "%c", ch);
			break;
		case UP:
			if(head->y <= 0) head->y = max_y;
			mvprintw(--(head->y), head->x, "%c", ch);
			break;
		case DOWN:
			if(head->y >= max_y) head->y = 0;
			mvprintw(++(head->y), head->x, "%c", ch);
			break;
		default:;
	}
	
	refresh();
}

/* Движение тела относительно головы */
void goTail(struct snake_t *head){
	char ch = '*';
	
	mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x," ");
	
	for(size_t i = head->tsize-1; i>0; i--)
	{
		head->tail[i] = head->tail[i-1];
		
		if( head->tail[i].y || head->tail[i].x)
			mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
	}
	
	head->tail[0].x = head->x;
	head->tail[0].y = head->y;
}

/* изменение направления */
void changeDirection(snake_t* snake, const int32_t key, const int32_t key_pred){
	if ((key == snake->controls.down) && (key_pred != UP))
		snake->direction = DOWN;
	else if ((key == snake->controls.up) && (key_pred != DOWN))
		snake->direction = UP;
	else if ((key == snake->controls.right) && (key_pred != LEFT))
		snake->direction = RIGHT;
	else if ((key == snake->controls.left) && (key_pred != RIGHT))
		snake->direction = LEFT;
}

int main(int argc, char **argv){
	snake_t* snake = (snake_t*)malloc(sizeof(snake_t));
	initSnake(snake,START_TAIL_SIZE,10,10);
	
	initscr();
	keypad(stdscr, TRUE); // Включаем F1,F2, стрелки и т.д.
	raw(); // Откдючаем line buffering
	noecho(); // Отключаем echo()режим при вызове getch
	curs_set(FALSE); //Отключаем курсор
	mvprintw(0, 0,"Use arrows for control.Press 'q' for EXIT");
	timeout(0); //Отключаем таймаут после нажатия клавиши в цикле
	
	int key_pressed = 0, key_pred = 0;
	
	while( key_pressed != STOP_GAME )
	{
		key_pred = snake->direction;
		key_pressed = getch(); // Считываем клавишу
		go(snake);
		goTail(snake);
		timeout(100); // Задержка при отрисовке(Скорость движения змеи)
		changeDirection(snake, key_pressed, key_pred);
	}
	free(snake->tail);
	free(snake);
	endwin();
	// printf("ready");
	// Завершаем режим curses mod
	return 0;
}


	
