//a snake class for a snake game where the snake moves around and growths when it eats food.

#include "snake.h"
Snake::Snake()
{
	head = Block(WINDOW_WIDTH / 2 + BLOCK_SIZE / 2,
				 WINDOW_HEIGHT / 2 + BLOCK_SIZE / 2,
				 BLOCK_SIZE,
				 BLOCK_SIZE,
				 SNAKE_COLOR);
			
	tail = &head;
	size = 1;
	current = None;
	next = None;
	distance = 0;
}
Block* Snake::getHead() { return &head; }
Block* Snake::getTail() { return tail;  }
int    Snake::getSize() { return size; }
float  Snake::getDistance() { return distance; }

Direction Snake::getCurrent() { return current; }
Direction Snake::getNext() { return next; }
void	  Snake::setNext(Direction value) { next = value; }
bool Snake::isMoving()
{
	return head.isMoving();
}
void Snake::turn()
{

	switch (next)
	{
	case Up:
		if (current != Down)
		{
			head.setVelocityX(0);
			head.setVelocityY(-1);
			current = next;
		}
		break;
	case Down:
		if (current != Up)
		{
			head.setVelocityX(0);
			head.setVelocityY(1);
			current = next;
		}
		break;
	case left:
		if (current != right)
		{
			head.setVelocityX(-1);
			head.setVelocityY(0);
			current = next;
		}
		break;
	case right:
		if (current != left)
		{
			head.setVelocityX(1);
			head.setVelocityY(0);
			current = next;
		}
	}//end direction switch
	next = None;
 
}//end turn
void Snake::move(float distance)
{
	if (this->distance + distance > BLOCK_SIZE)
		distance = BLOCK_SIZE - this->distance;
	this->distance += distance;


	Block* part = &head;
	while(part)
	{
		part->move(distance);
		part = part->getAfter();
	}

}//end move
bool Snake::hasMoved()
{
	return distance >= BLOCK_SIZE;

}
void Snake::reset()
{
	distance = 0;

	Block* part = tail;
	while(part->getBefore())
	{
		part->setVelocityX(part->getBefore()->getVelocityX());
		if (part->getVelocityX())
			part->setY( part->getBefore()->getY() );
		part->setVelocityY( part->getBefore()->getVelocityY() );
		if (part->getVelocityY())
			part->setX( part->getBefore()->getX() );
			
		part = part->getBefore();
	}//end loop tail to head

}
bool Snake::hitSelf()
{
	bool collide = false;

	Block* part = head.getAfter();
	int skip = 3;
	while (part && !collide)
	{
		if (skip)
			skip--;
		else
			collide = head.intersects(part);
		part = part->getAfter();
	}

	return collide;

}
bool Snake::hitBlock(Block* block, bool all)
{
	bool collide = head.intersects(block);
	if (all && !collide)
	{

		Block* part = head.getAfter();
		while (part && !collide)
		{
				collide = block->intersects(part);
			part = part->getAfter();
		}

	}

	return collide;
}//end of hit block
void Snake::grow()
{
	tail->append(tail);

	tail = tail->getAfter();
	tail->setX(tail->getX() + tail->getVelocityX() * -1 * BLOCK_SIZE);
	tail->setY(tail->getY() + tail->getVelocityY() * -1 * BLOCK_SIZE);
	size++;
}//end of grow method
void Snake::draw(OpenGL& window)
{ 
	Block* part = &head;
	while (part)
	{
		part->draw(window);
		part = part->getAfter();
	}

}//end draw
