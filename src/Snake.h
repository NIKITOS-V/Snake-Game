#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "Vector.h"

typedef struct {
    Int8Vector head_pos;
    Int8Vector tail_pos;
    Int8Vector head_vec;
    unsigned short len;
} Snake;

/* ------ Строители ------ */

#define to_Snake(_head_coord, _tail_coord, _head_vec, _len)\
( (Snake) { .head_pos = (_head_coord),\
    .tail_pos = (_tail_coord),\
    .head_vec = (_head_vec),\
    .len = (char) (_len)\
} )

#define new_Snake()\
to_Snake(zero_Vector(Int8Vector), zero_Vector(Int8Vector), zero_Vector(Int8Vector), 0)

/* ------ END ------ */

#endif // __SNAKE_H__
