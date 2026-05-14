#ifndef __VECTOR_H__
#define __VECTOR_H__

/* ------ Структура Вектор ------ */

typedef struct {
	char x, y;
} Int8Vector;

/* ------ Строители ----- */

#define to_Int8Vector(_x, _y)\
( (Int8Vector) {\
	.x = (char) (_x),\
	.y = (char) (_y)\
} )

#define zero_Vector(T)\
( (T) {.x = 0, .y = 0} )

/* ------ Геттеры ------ */

#define get_Vector_x(vec)\
( (vec)->x )

#define get_Vector_y(vec)\
( (vec)->y )

/* ------ Сеттеры ------ */

#define set_Vector_x(vec, _x)\
( (vec)->x = (char) (_x) )

#define set_Vector_y(vec, _y)\
( (vec)->y = (char) (_y) )

/* ------ END ------ */

#endif // __VECTOR_H__

