#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "Vector.h"

/* ------ Игровые объекты ------ */

typedef enum {
	SnakeBody_T,
	SnakeHead_T,
	Apple_T,
	Air_T
} GameEntityType;

typedef struct {
	char type, texture;
	Int8Vector vec;
} GameEntity;

#define to_Entity(_type, _vec, _texture)\
( (GameEntity) {.type = (char) (_type), .vec = (_vec), .texture = (_texture)} )

#define new_SnakeBody(_vec)\
to_Entity(SnakeBody_T, _vec, SNAKE_BODY_TEXTURE)

#define new_SnakeHead(_vec)\
to_Entity(SnakeHead_T, _vec, SNAKE_HEAD_TEXTURE)

#define new_AirEntity()\
to_Entity(Air_T, zero_Vector(Int8Vector), AIR_TEXTURE)

#define new_AppleEntity()\
to_Entity(Apple_T, zero_Vector(Int8Vector), APPLE_TEXTURE)

#endif // __GAME_ENTITY_H__
