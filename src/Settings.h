#ifndef __SETTINGS_H__
#define __SETTINGS_H__

/* ------ Макросы-константы ------ */

// Приложение

#define READ_KEYBOARD_PS		15

#define GAME_OVER_STR           "Game Over!"
#define SCORE_STR               "Score"

// Поле

#define FIELD_WIDTH 			40
#define FIELD_HEIGHT 			20

#define FIELD_FULL_WIDTH        FIELD_WIDTH + 2
#define FIELD_FULL_HEIGHT       FIELD_HEIGHT + 2

#define FIELD_BORDER_TEXTURE	'#'

// Змейка

#define SNAKE_INIT_LEN 			4
#define SNAKE_INIT_VEC_X        -1
#define SNAKE_INIT_VEC_Y        0

#define SNAKE_SPEED 			3

#define SNAKE_BODY_TEXTURE 		'0'
#define SNAKE_HEAD_TEXTURE		'@'

// Яблоко

#define APPLE_TEXTURE			'A'

// Пустое пространство

#define AIR_TEXTURE				' '

// Привязка клавиш

#define MOVE_UP_KEY_US 			'w'
#define MOVE_DOWN_KEY_US 		's'
#define MOVE_RIGHT_KEY_US		'd'
#define MOVE_LEFT_KEY_US		'a'

#define EXIT_KEY_US 			'q'

/* ------ END ------ */

#endif // __SETTINGS_H__
