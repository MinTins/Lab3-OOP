

#include<iostream>
#include<stdlib.h>
#include<time.h>

#include "opengl.h"
#include "snakedefs.h"
#include "block.h"
#include "snake.h"
void InitGameObjects(Block borders[], Snake& snake, Block& food);
Direction processInput(OpenGL& window);
bool update(Snake& snake, Block borders[], Block& food, float deltaTime);
void render(OpenGL& window, Block borders[], Snake snake, Block food);
bool CollisionChecks(Snake snake, Block bordersp[]);

Block NextFood(Snake snake);


int main()
{
	srand(time(NULL));
	OpenGL window = OpenGL(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITTLE.c_str());
	Block borders[4];
	Snake snake = Snake();
	Block food; 

	InitGameObjects(borders, snake, food);
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	bool gameOver = false;
	while (!window.isClosing() && !gameOver)
	{
		float currentFrame = window.getTime();
		deltaTime += (currentFrame - lastFrame) * 1000;
		lastFrame = currentFrame;
		if (snake.getNext() == None)
			snake.setNext(processInput(window));
		if (snake.getNext() == Exit)
			gameOver = true;
		while (!gameOver && deltaTime >= FPS_RATE)
		{
			gameOver = update(snake, borders, food, deltaTime);
			deltaTime -= FPS_RATE;
		}
		render(window, borders, snake, food);

		window.pollEvents();
	}//end game loop

	std::cout << "Game Over!! \n press any key and enter to close..." << std::endl;

	char pause;
	std::cin >> pause;
	
	return 0;
}
void InitGameObjects(Block borders[], Snake& snake, Block& food)
{
	borders[TopWall] = Block(MARGIN,
							 MARGIN,
							 WINDOW_WIDTH - 2 * MARGIN,
							 BORDER_SIZE,
							 BORDER_COLOR);

	borders[BottomWall] = Block(MARGIN,
								 WINDOW_HEIGHT - MARGIN - BORDER_SIZE,
								 WINDOW_WIDTH - 2 * MARGIN,
								 BORDER_SIZE,
								 BORDER_COLOR);

	borders[LeftWall] = Block(MARGIN,
							  MARGIN,
							  BORDER_SIZE,
							  WINDOW_HEIGHT - 2 * MARGIN,
							  BORDER_COLOR);

	borders[RightWall] = Block(WINDOW_WIDTH - MARGIN - BORDER_SIZE,
							   MARGIN,
							   BORDER_SIZE,
							   WINDOW_HEIGHT - 2 * MARGIN,
							   BORDER_COLOR);

	
	food = NextFood(snake);	


}
Direction processInput(OpenGL& window)
{
	Direction myDirection = None;

	GL_KEY key = window.GetKey();

	switch (key)
	{
	case GL_KEY::KEY_ESC:
	case GL_KEY::KEY_X:
		myDirection = Direction::Exit;
		break;

	case GL_KEY::KEY_W:
	case GL_KEY::KEY_UP_ARROW:
		myDirection = Direction::Up;
		break;

	case GL_KEY::KEY_S:
	case GL_KEY::KEY_DOWN_ARROW:
		myDirection = Direction::Down;
		break;

	case GL_KEY::KEY_A:
	case GL_KEY::KEY_LEFT_ARROW:
		myDirection = Direction::left;
		break;

	case GL_KEY::KEY_D:
	case GL_KEY::KEY_RIGHT_ARROW:
		myDirection = Direction::right;
	}

	return myDirection;
}
bool update(Snake& snake, Block borders[], Block& food, float deltaTime)
{
	bool collision = false;
	if (!snake.isMoving())
	{
		if (snake.getNext() == None)
			return false;
		else
			snake.turn();
	}
	float distance = SNAKE_VELOCITY / deltaTime;
	snake.move(distance);
	if (snake.hasMoved())
	{
		snake.turn();

		snake.reset();
		if (snake.hitBlock(&food))
		{
			snake.grow();
			food = NextFood(snake);

		}
		else
		{
			collision = CollisionChecks(snake, borders);
		}

	}//end distance check

	return collision;
}//end update


void render(OpenGL& window, Block borders[], Snake snake, Block food)
{
	window.clearWindow();
	snake.draw(window);
	food.draw(window);
	borders[TopWall].draw(window);
	borders[BottomWall].draw(window);
	borders[LeftWall].draw(window);
	borders[RightWall].draw(window);
	window.paintWindow();
}
bool CollisionChecks(Snake snake, Block borders[])
{
	bool collide = false;
	if (snake.getCurrent() == Up && snake.hitBlock(&borders[TopWall]))
		collide = true;
	else if (snake.getCurrent() == Down && snake.hitBlock(&borders[BottomWall]))
		collide = true;
	else if (snake.getCurrent() == left && snake.hitBlock(&borders[LeftWall]))
		collide = true;
	else if (snake.getCurrent() == right && snake.hitBlock(&borders[RightWall]))
		collide = true;
	else
	{ 
		collide = snake.hitSelf();
	}

	return collide;
}//end collision
Block NextFood(Snake snake)
{
	Block food = Block(0, 0, BLOCK_SIZE, BLOCK_SIZE, FOOD_COLOR);

	bool placed = false;
	while (!placed)
	{
		int xCoord = rand() % (GAME_WIDTH - BLOCK_SIZE);
		int yCoord = rand() % (GAME_HEIGHT - BLOCK_SIZE);
		xCoord += (MARGIN + BORDER_SIZE);
		yCoord += (MARGIN + BORDER_SIZE);

		food.setX(xCoord);
		food.setY(yCoord);

		placed = !snake.hitBlock(&food, true);								


	}//while not placed

	return food;
}//end NextFood