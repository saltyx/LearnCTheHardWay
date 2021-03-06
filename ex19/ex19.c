
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ex19.h"

int Monster_attack(void *self, int damage){
	
	Monster *monster = self;
	printf("you attack %s\n", monster->_(description));
	monster->hit_point -=damage;
	
	if(monster->hit_point >0){
		printf("it still alive\n");
		return 0;
	}
	else{
		printf("it's dead\n");
		return 1;
	}
}
int Monster_init(void *self){
	Monster *monster = self;
	monster->hit_point = 10;
	return 1;
}
Object MonsterProto = {
	.init = Monster_init,
	.attack = Monster_attack

};
void *Room_move(void *self , Direction direction){
	Room *room = self;
	Room *next = NULL;

	if(direction == NORTH && room->north){
		printf("you can go north , into:\n");
		next = room->north;
	}else if (direction == SOUTH && room->south){
		printf("you can go south , into:\n");
		next = room->south;
	}else if (direction == EAST && room->east){
		printf("you can go east , into:\n");
		next = room->east;
	}else if (direction == WEST &&room->west){
		printf("you can go west , into:\n");
		next = room->west;
	}else {
		printf("you can't go that direction\n");
		next = NULL;
	}
	if (next == NULL){
		next->_(describe)(next);

	}

	return next;
}
int Room_attack(void *self, int damage){

	Room *room = self;
	Monster *monster = room->bad_guy;
	if (monster){
		monster->_(attack)(monster, damage);
		return 1;
	}else {
		printf("you flail in the air at nothing.\n");
		return 0;
	}
}

Object RoomProto = {
	.move = Room_move,
	.attack = Room_attack
};

void *Map_move(void *self, Direction direction){
	Map *map = self;
	Room *location =map->location;
	Room *next = NULL;

	next = location->_(move)(location, direction);

	if(next){
		map->location = next;
	}
	return next;
}
int Map_attack(void *self, int damage){
	Map *map = self;
	Room *location = map->location;
	return location->_(attack)(location, damage);
}

int Map_init(void *self){
	Map *map = self;
	Room *hall = NEW(Room, "the great hall");
	Room *throne = NEW(Room, "the throne hall");
	Room *arena = NEW(Room, "the arena");
	Room *kitchen = NEW(Room, "the kitchen, you have a knife now");

	arena->bad_guy = NEW(Monster, "the evil");

	hall->north = throne;
	throne->west = arena;
	throne->east = kitchen;
	throne->south = hall;

	arena->east = throne;
	kitchen->west = throne;
	
	map->start = hall;
	map->location = hall;

	return 1;
}

Object MapProto = {
	.init = Map_init,
	.move = Map_move,
	.attack = Map_attack
};

int process_input(Map *game){
	
	printf("\n>");
	char ch = getchar();

	int damage = rand() % 4;

	switch(ch){
		case -1:
			printf("Giving up?you suck.\n");
			return 0;
			break;
		case 'n':
			game->_(move)(game, NORTH);
			break;
		case 's':
			game->_(move)(game, SOUTH);
			break;
		case 'w':
			game->_(move)(game, WEST);
			break;
		case 'e':
			game->_(move)(game, EAST);
			break;
		case 'a':
			game->_(attack)(game, damage);
			break;
		case 'l':
			printf("you can go\n");
			if (game->location->north)printf("north\n");
			if (game->location->west)printf("WEST\n");
			if (game->location->east)printf("east\n");
			if (game->location->south) printf("SOUTH\n");
			break;
		default:
			printf("nothing :%c\n", ch);
	}

	return 1;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	srand(time(NULL));
	Map *game = NEW(Map, "the hall of Minotaur.");

	printf("you enter the ");
	game->location->_(describe)(game->location);
	while(process_input(game)){

	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */




