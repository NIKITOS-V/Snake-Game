#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <stdbool.h>

#include "Settings.h"
#include "Vector.h"
#include "GameEntity.h"
#include "AppData.h"
#include "Snake.h"

typedef GameEntity (GameFieldPtr)[FIELD_WIDTH][FIELD_HEIGHT];

/* ------ Макросы-конструкции ------ */

#define compare_keys_1(key, key_1)\
( (key) == (key_1) )

#define compare_keys_2(key, key_1, key_2)\
( compare_keys_1(key, key_1) || compare_keys_1(key, key_2) )

#define calc_ps_delay(fps)\
( 1000 / (fps) )\

#define calc_array_size(arr)\
( sizeof(arr) / sizeof(arr)[0] )

#define calc_middle(left, right)\
( ((left) + (right)) / 2 )

#define calc_correct_pos(a)\
( (a) + 1)

#define cycle_pos(a, border)\
( (a) % (border) < 0 ? (border) - 1 : (a) % (border) )

/* ------ Идентификация нажатой клавиши ------ */

bool is_move_up_key(int key) {
	return key == MOVE_UP_KEY_US;
}

bool is_move_down_key(int key) {
	return key == MOVE_DOWN_KEY_US;
}

bool is_move_right_key(int key) {
	return key == MOVE_RIGHT_KEY_US;
}

bool is_move_left_key(int key) {
	return key == MOVE_LEFT_KEY_US;
}

bool is_exit_key(int key) {
	return key == EXIT_KEY_US;
}

/* ------ Контроль змейки & поля ------ */

void correct_direction(Int8Vector* direct) {
	if (direct->x > 1) {
		direct->x = 1;
	} else if (direct->x < -1) {
		direct->x = -1;
	}

	if (direct->y > 1) {
		direct->y = 1;
	} else if (direct->y < -1) {
		direct->y = -1;
	}
}

void update_direction(int pressed_key, Int8Vector* old_direct) {
	Int8Vector new_direct = *old_direct;

	if (is_move_down_key(pressed_key) && old_direct->x) {
		new_direct.y += 1;
	} else if (is_move_up_key(pressed_key) && old_direct->x) {
		new_direct.y += -1;
	}

	if (is_move_right_key(pressed_key) && old_direct->y) {
		new_direct.x += 1;
	} else if (is_move_left_key(pressed_key) && old_direct->y) {
		new_direct.x += -1;
	}

	correct_direction(&new_direct);

	*old_direct = new_direct;
}

void put_on_field(GameFieldPtr field, Int8Vector pos, GameEntity entity) {
	field[pos.x][pos.y] = entity;
}

Int8Vector calc_new_head_pos(Snake* snake) {
	return to_Int8Vector(
		cycle_pos(snake->head_vec.x + snake->head_pos.x, FIELD_WIDTH),
		cycle_pos(snake->head_vec.y + snake->head_pos.y, FIELD_HEIGHT)
	);
}

Int8Vector calc_new_tail_pos(Snake* snake, GameFieldPtr field) {
	Int8Vector tail_direct = field[snake->tail_pos.x][snake->tail_pos.y].vec;

	return to_Int8Vector(
		cycle_pos(tail_direct.x + snake->tail_pos.x, FIELD_WIDTH),
		cycle_pos(tail_direct.y + snake->tail_pos.y, FIELD_HEIGHT)
	);
}

void update_head_pos(
	Snake* snake,
	GameFieldPtr field,
	Int8Vector new_head_pos
) {
	put_on_field(field, new_head_pos, new_SnakeHead(snake->head_vec));
	put_on_field(field, snake->head_pos, new_SnakeBody(snake->head_vec));
}

void update_tail_pos(
	Snake* snake,
	GameFieldPtr field
) {
	put_on_field(field, snake->tail_pos, new_AirEntity());
}

GameEntityType check_collision(GameFieldPtr field, Int8Vector new_pos) {
	return field[new_pos.x][new_pos.y].type;
}

/* ------ Вывод (вспомогательное) ------ */

void do_ps_delay(int ps) {
	Sleep(calc_ps_delay(ps));
}

void endl(void) {
	putchar('\n');
}

void set_cursor_pos(int x, int y) {
	printf("\033[%d;%dH", y + 1, x + 1);
}

void return_cursor(void) {
	set_cursor_pos(0, FIELD_FULL_HEIGHT + 1);
}

void clear_cmd(void) {
	printf("\033[2J");
}

/* ------ Вывод (основное) ------ */

void prepare_cmd(void) {
	clear_cmd();
	set_cursor_pos(0, 0);
}

void hide_cursor(void) {
	printf("\033[?25l");
}

void print_line(size_t size, char texture) {
	for (size_t i = 0; i < size; i++) {
		putchar(texture);
	}
}

void print_field(GameFieldPtr field) {
	print_line(FIELD_FULL_WIDTH, FIELD_BORDER_TEXTURE);

	for (size_t y = 0; y < FIELD_HEIGHT; y++) {
		endl();
		putchar(FIELD_BORDER_TEXTURE);

		for (size_t x = 0; x < FIELD_WIDTH; x++) {
			putchar(field[x][y].texture);
		}

		putchar(FIELD_BORDER_TEXTURE);
	}

	endl();
	print_line(FIELD_FULL_WIDTH, FIELD_BORDER_TEXTURE);
}

void update_console(GameFieldPtr field, Int8Vector pos) {
	set_cursor_pos(calc_correct_pos(pos.x), calc_correct_pos(pos.y));
	putchar(field[pos.x][pos.y].texture);
}

void move_snake_head(
	Snake* snake,
	GameFieldPtr field,
	Int8Vector new_head_pos
) {
	update_console(field, new_head_pos);
	update_console(field, snake->head_pos);
}

void move_snake_tail(Snake* snake, GameFieldPtr field) {
	update_console(field, snake->tail_pos);
}

void move_apple(GameEntity* apple, GameFieldPtr field) {
	update_console(field, apple->vec);
}

void put_new_apple(GameEntity* apple, GameFieldPtr field) {
	Int8Vector pos = zero_Vector(Int8Vector);

	do {
		pos.x = rand() % FIELD_WIDTH;
		pos.y = rand() % FIELD_HEIGHT;
	} while (field[pos.x][pos.y].type != Air_T);

	apple->vec = pos;

	put_on_field(field, pos, new_AppleEntity());
}

void update_and_print_score(Snake* snake) {
	set_cursor_pos(7, FIELD_FULL_HEIGHT);

	printf("%d", snake->len - SNAKE_INIT_LEN);
}

/* ------ Чтение ввода ------ */

bool is_read_input_thread_run(AppData* app) {
	return app->is_run && !is_exit_key(app->pressed_key);
}

void* read_keyboard(void* args) {
	AppData* app = (AppData*) args;

	while (is_read_input_thread_run(app)) {
		app->pressed_key = _getch();
		do_ps_delay(READ_KEYBOARD_PS);
	}

	return NULL;
}

/* ------ Init ------ */

void init_read_keyboard_thread(pthread_t* read_keyboard_thread, AppData* app) {
	pthread_create(read_keyboard_thread, NULL, read_keyboard, app);
}

void init_filed(GameFieldPtr field) {
	for (size_t x = 0; x < FIELD_WIDTH; x++) {
		for (size_t y = 0; y < FIELD_HEIGHT; y++) {
			field[x][y] = new_AirEntity();
		}
	}
}

void init_snake(Snake* snake) {
	snake->len = SNAKE_INIT_LEN;
	snake->head_vec = to_Int8Vector(SNAKE_INIT_VEC_X, SNAKE_INIT_VEC_Y);

	char x = cycle_pos(calc_middle(0, FIELD_WIDTH), FIELD_WIDTH);
	char y = cycle_pos(calc_middle(0, FIELD_HEIGHT), FIELD_HEIGHT);

	char tx = x - (snake->len - 1) * snake->head_vec.x;
	char ty = y - (snake->len - 1) * snake->head_pos.y;

	snake->head_pos = to_Int8Vector(x, y);
	snake->tail_pos = to_Int8Vector(
		cycle_pos(tx, FIELD_WIDTH),
		cycle_pos(ty, FIELD_HEIGHT)
	);
}

void init_snake_on_field(Snake* snake, GameFieldPtr field) {
	put_on_field(field, snake->head_pos, new_SnakeHead(snake->head_vec));

	for (char i = 1; i < snake->len; i++) {
		char x = snake->head_pos.x - i * snake->head_vec.x;
		char y = snake->head_pos.y - i * snake->head_vec.y;

		put_on_field(
			field,
			to_Int8Vector(
				cycle_pos(x, FIELD_WIDTH),
				cycle_pos(y, FIELD_HEIGHT)
			),
			new_SnakeBody(snake->head_vec)
		);
	}
}

void print_init_score(void) {
	set_cursor_pos(0, FIELD_FULL_HEIGHT);

	printf(SCORE_STR ": %d", 0);
}

/* ------ Main ------ */

int main(void) {
	srand((unsigned int) time(NULL));

	pthread_t read_keyboard_thread;
	AppData app = new_AppData();
	GameFieldPtr field;
	Snake snake = new_Snake();
	GameEntity apple = new_AppleEntity();
	bool is_game_over = false;

	init_filed(field);
	init_snake(&snake);
	init_snake_on_field(&snake, field);
	put_new_apple(&apple, field);
	init_read_keyboard_thread(&read_keyboard_thread, &app);

	app.is_run = true;
	prepare_cmd();
	print_field(field);
	move_apple(&apple, field);
	print_init_score();
	hide_cursor();

	while (app.is_run) {
		if (is_exit_key(app.pressed_key)) {
			app.is_run = false;
			return_cursor();

			break;
		}

		update_direction(app.pressed_key, &snake.head_vec);

		Int8Vector new_head_pos = calc_new_head_pos(&snake);
		GameEntityType ent_type = check_collision(field, new_head_pos);

		if (ent_type == SnakeBody_T) {
			app.is_run = false;
			return_cursor();

			is_game_over = true;

			break;
		} else if (ent_type == Apple_T) {
			snake.len++;
			update_and_print_score(&snake);

			put_new_apple(&apple, field);
			move_apple(&apple, field);
		} else {
			Int8Vector new_tail_pos = calc_new_tail_pos(&snake, field);

			update_tail_pos(&snake, field);
			move_snake_tail(&snake, field);

			snake.tail_pos = new_tail_pos;
		}

		update_head_pos(&snake, field, new_head_pos);
		move_snake_head(&snake, field, new_head_pos);

		app.pressed_key = ' ';
		snake.head_pos = new_head_pos;

		fflush(stdout);

		do_ps_delay(SNAKE_SPEED);
	}

	if (is_game_over) {
		printf("\n" GAME_OVER_STR "\n\n");
	}

	return 0;
}
