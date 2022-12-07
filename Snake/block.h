
#ifndef BLOCK_H
#define BLOCK_H

#include "snakedefs.h"
#include "opengl.h"

class Block
{
public:
	Block();
	Block(float x, float y, float width, float height, Color color,
		  float velocityX = 0, float velocityY = 0,
		  Block* before = nullptr, Block* after = nullptr);
	Block(Block* block);
	float getX();
	void setX(float value);
	float getY();
	void setY(float value);
	int getWidth();
	void setWidth(int value);
	int getHeight();
	void setHeight(int value);
	Color getColor();
	void setColor(Color value);
	float getVelocityX();
	void setVelocityX(float value);
	float getVelocityY();
	void setVelocityY(float value);
	Block* getBefore();
	void setBefore(Block* ptrBlock);
	Block* getAfter();
	void setAfter(Block* ptrBlock);
	void draw(OpenGL& window);
	bool intersects(Block* other);
	bool isMoving();
	void move(float distance);
	void append(Block* other);


private:
	float x;
	float y;
	int width;
	int height;
	Color color;
	float velocityX;
	float velocityY;
	Block* before;
	Block* after;
};






#endif
