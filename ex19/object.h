/*
 * =====================================================================================
 *
 *       Filename:  object.h
 *
 *    Description:  ex19
 *
 *        Version:  1.0
 *        Created:  2015年08月06日 22时58分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  shiyan (), shiyan233@hotmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _object_h
#define _object_h
typedef enum{
	NORTH, SOUTH, EAST, WEST
}Direction;

typedef struct{
	char *description;
	int (*init)(void *self);
	void (*describe)(void *self);
	void (*destroy)(void *self);
	void *(*move)(void *self, Direction direction);
	int (*attack)(void *self, int damage);
}Object;
#define NEW(T,N) Object_new(sizeof(T),T##Proto, N)
#define _(N) proto.N

#endif
