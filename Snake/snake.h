
#ifndef SNAKE_H
#define SNAKE_H

#include "block.h"
#include "snakedefs.h"

class Snake
{
public:
	Snake();
	Block* getHead();
	Block* getTail();
	int    getSize();
	float  getDistance();

	Direction getCurrent();
	Direction getNext();
	void	  setNext(Direction value);
	bool isMoving();
	void turn();
	void move(float distance);
	bool hasMoved();
	void reset();
	bool hitSelf();
	bool hitBlock(Block* block, bool all = false);
	void grow();
	void draw(OpenGL& window);

private:
	Block head;
	Block* tail;
	int size;
	Direction current;
	Direction next;
	float distance;
};






#endif
