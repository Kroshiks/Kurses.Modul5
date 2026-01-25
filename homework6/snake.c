#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

#define NUM_SNAKES 1

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME='q'};
enum {MAX_TAIL_SIZE = 100, START_TAIL_SIZE = 3, MAX_FOOD_SIZE = 20, SEED_NUMBER = 3,  FOOD_EXPIRE_SECONDS = 5};

/* Кнопки управления */
struct control_buttons {
    int down;
    int up;
    int left;
    int right;
};

struct control_buttons default_controls[NUM_SNAKES] = {
    {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT}                          
};

/* Голова змеи */
typedef struct snake_t {
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    struct control_buttons controls;
} snake_t;

/* Тело змеи */
typedef struct tail_t {
    int x;
    int y;
} tail_t;

/* Еда */
struct food {
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food[MAX_FOOD_SIZE];

/* Инициализация тела */
void initTail(tail_t t[], size_t size) {
    struct tail_t init_t = {0, 0};
    for(size_t i = 0; i < size; i++) 
        t[i] = init_t;
}

/* Инициализация головы */
void initHead(snake_t *head, int x, int y) {
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

/* Инициализация змеи */
void initSnake(snake_t *head, size_t size, int x, int y, int snake_index) {
    tail_t* tail = (tail_t*)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    
    head->tail = tail;
    head->tsize = size + 1;
    head->controls = default_controls[snake_index];
}

/* Инициализация еды */
void initFood(struct food f[], size_t size) {
    struct food init = {0, 0, 0, 0, 0};
    for(size_t i = 0; i < size; i++) 
        f[i] = init;
}

/* Движение головы */
_Bool go(struct snake_t *head) {
    char ch = '@';
    mvprintw(head->y, head->x, " "); // очищаем один символ

    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    
    switch (head->direction) {
        case LEFT:
            if(head->x <= 0) 
                head->x = max_x - 1;
            mvprintw(head->y, --(head->x), "%c", ch);
            break;
        case RIGHT:
            if(head->x >= max_x - 1) 
                head->x = 0;
            mvprintw(head->y, ++(head->x), "%c", ch);
            break;
        case UP:
            if(head->y <= 0) 
                head->y = max_y - 1;
            mvprintw(--(head->y), head->x, "%c", ch);
            break;
        case DOWN:
            if(head->y >= max_y - 1) 
                head->y = 0;
            mvprintw(++(head->y), head->x, "%c", ch);
            break;
        default:
            break;
    }

    for(size_t i = 0; i < head->tsize; i++) {
        if(head->x == head->tail[i].x && head->y == head->tail[i].y) 
            return 1;    
    }
    
    refresh();
    return 0;
}

/* Движение тела относительно головы */
void goTail(struct snake_t *head) {
    char ch = '*';
    
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    
    for(size_t i = head->tsize - 1; i > 0; i--) {
        head->tail[i] = head->tail[i - 1];
        
        if(head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

/* изменение направления */
void changeAllDirections(snake_t snakes[], const int32_t key, size_t num_snakes) {
    for (size_t i = 0; i < num_snakes; i++)
     {
        switch(key) {
            case 'w': case 'W':
                if (i == 0) 
                    snakes[i].direction = UP;
                break;
            case 's': case 'S':
                if (i == 0) 
                    snakes[i].direction = DOWN;
                break;
            case 'a': case 'A':
                if (i == 0) 
                    snakes[i].direction = LEFT;
                break;
            case 'd': case 'D':
                if (i == 0) 
                    snakes[i].direction = RIGHT;
                break;
            default:
                if (key == snakes[i].controls.down)
                    snakes[i].direction = DOWN;
                else if (key == snakes[i].controls.up)
                    snakes[i].direction = UP;
                else if (key == snakes[i].controls.right)
                    snakes[i].direction = RIGHT;
                else if (key == snakes[i].controls.left)
                    snakes[i].direction = LEFT;
                break;
        }
    }
}

int chekDirection(snake_t *snake, int32_t key_pressed) {
    // Проверяем текущее направление и предотвращаем поворот на 180 градусов
    
    if(snake->direction == UP)
    {
        if(key_pressed == snake->controls.down || key_pressed == 's' || key_pressed == 'S')
            return 0;
    }
    else if(snake->direction == DOWN)
    {
        if(key_pressed == snake->controls.up || key_pressed == 'w' || key_pressed == 'W')
            return 0;
    }
    else if(snake->direction == LEFT)
    {
        if(key_pressed == snake->controls.right || key_pressed == 'd' || key_pressed == 'D')
            return 0;
    }
    else if(snake->direction == RIGHT)
    {
        if(key_pressed == snake->controls.left || key_pressed == 'a' || key_pressed == 'A')
            return 0;
    }
    
    return 1;
}

/* Размещение еды */
void putFoodSeed(struct food *fp) {
    int max_x = 0, max_y = 0;
    char spoint[2] = {0};
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(fp->y, fp->x, " ");
    
    fp->x = rand() % (max_x - 1);
    fp->y = rand() % (max_y - 2) + 1;
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
    
    spoint[0] = fp->point;
    mvprintw(fp->y, fp->x, "%s", spoint);
}

/* Размещение еды */
void putFood(struct food f[], size_t number_seeds) {
    for(size_t i = 0; i < number_seeds; i++)
        putFoodSeed(&f[i]);
}

/* Обновление зерна */
void refreshFood(struct food f[], int nfood) {
    for(size_t i = 0; i < nfood; i++) {
        if(f[i].put_time) {
            if(!f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS) {
                putFoodSeed(&f[i]);
            }
        }
    }
}

_Bool haveEat(struct snake_t *head, struct food f[])
{
    for (size_t i = 0; i < MAX_FOOD_SIZE; i++)
    {
        if (f[i].enable && head->x == f[i].x && head->y == f[i].y)
        {
            f[i].enable = 0;
            mvprintw(f[i].y, f[i].x, " ");
            return 1; 
        }
    }
    return 0; 
}
/*
 Увеличение хвоста на 1 элемент
 */
void addTail(struct snake_t *head){
    if (head->tsize < MAX_TAIL_SIZE)
    {
        head->tsize++;
        head->tail[head->tsize - 1].x = head->tail[head->tsize - 2].x;
        head->tail[head->tsize - 1].y = head->tail[head->tsize - 2].y;
    }
}

_Bool isCrush(snake_t * snake){
    for (size_t i = 1; i < snake->tsize; i++)
    {
        if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y)
        {
            return 1;
        }
    }
          
    return 0; 
}

void repairSeed(struct food f[], size_t nfood, struct snake_t *head){
    for (size_t i = 0; i < head->tsize; i++)
    {
        for (size_t j = 0; j < nfood; j++)
        {
            if (f[j].enable && head->tail[i].x == f[j].x && head->tail[i].y == f[j].y)
            {
                putFoodSeed(&f[j]);
            }
        }
    }
    
    for (size_t j = 0; j < nfood; j++)
    {
        if (f[j].enable && head->x == f[j].x && head->y == f[j].y)
            continue;
    }
    
    for (size_t i = 0; i < nfood; i++)
    {
        for (size_t j = i + 1; j < nfood; j++)
        { 
            if (f[i].enable && f[j].enable && f[i].x == f[j].x && f[i].y == f[j].y)
                putFoodSeed(&f[j]);    
        }
    }
    
    for (size_t j = 0; j < nfood; j++)
    {
        if (f[j].enable && head->x == f[j].x && head->y == f[j].y) 
            putFoodSeed(&f[j]);
    }
}

int main() {
    snake_t snakes[NUM_SNAKES];
    for(int i = 0; i < NUM_SNAKES; i++) {
        initSnake(&snakes[i], START_TAIL_SIZE, 10 + i * 5, 10, i);
    }

    srand(time(NULL)); // Инициализируем генератор случайных чисел
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw(); // Отключаем line buffering
    noecho(); // Отключаем echo() режим при вызове getch
    curs_set(FALSE); // Отключаем курсор
    mvprintw(1, 0, "Use arrows for control. Press 'q' for EXIT");
    timeout(0); // Отключаем таймаут после нажатия клавиши в цикле

    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER);

    int key_pressed = 0;
    _Bool collision = false;
    
    while(key_pressed != STOP_GAME) {
        key_pressed = getch(); // Считываем клавишу
        
        // Обновляем каждую змею
        for(int i = 0; i < NUM_SNAKES; i++) {
            if(go(&snakes[i])) {
                collision = true;
                break;
            }
            goTail(&snakes[i]);
            
            // Проверяем, съела ли змея еду
            if (haveEat(&snakes[i], food)) {
                addTail(&snakes[i]);
            }
            
            // Проверяем столкновение
            if (isCrush(&snakes[i])) {
                collision = true;
                break;
            }
        }
        
        if(collision) break;
        
        timeout(50); // Задержка при отрисовке (Скорость движения змеи)
        
        repairSeed(food, SEED_NUMBER, &snakes[0]);
        refreshFood(food, SEED_NUMBER);
        
        if(chekDirection(&snakes[0], key_pressed))
            changeAllDirections(snakes, key_pressed, NUM_SNAKES);
    }
    
    for(int i = 0; i < NUM_SNAKES; i++)
    {
        free(snakes[i].tail);
    }
    endwin();
    
    if(collision) 
        printf("COLLISION!!!\n");
    else 
        printf("END.\n");
    
    return 0;
}
