#ifndef __APP_DATA_H__
#define __APP_DATA_H__

#include "stdbool.h"

/* ------ Структура приложения ------ */

typedef struct {
	int pressed_key;
	bool is_run;
} AppData;

/* ------ Строитель ------ */

#define new_AppData()\
( (AppData) {\
	.pressed_key = 0,\
	.is_run = 0,\
} )

/* ------ END ------ */

#endif // __APP_DATA_H__
