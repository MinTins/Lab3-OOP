
#ifndef SNAKE_DEFS_H
#define SNAKE_DEFS_H

#include <string>
const float FPS_RATE = 1000 / 60.0f;
const int GAME_WIDTH{ 400 };
const int GAME_HEIGHT{ 200 };
const int MARGIN{ 10 };
const int BORDER_SIZE{ 2 };
const int BLOCK_SIZE{ 10 };


const int WINDOW_WIDTH = GAME_WIDTH + 2 * MARGIN + 2 * BORDER_SIZE;
const int WINDOW_HEIGHT = GAME_HEIGHT + 2 * MARGIN + 2 * BORDER_SIZE;
const std::string  WINDOW_TITTLE = "Snake Game";

enum Borders
{
	TopWall,
	BottomWall,
	LeftWall,
	RightWall,
};

enum Color
{
	White = 0xFFFFFF00,
	Silver = 0xC0C0C000,
	Gray = 0x80808000,
	Black = 0x00000000,
	Red = 0xFF000000,
	Maroon = 0x80000000,
	Yellow = 0xFFFF0000,
	Olive = 0x80800000,
	Lime = 0x00FF0000,
	Green = 0x00800000,
	Aqua = 0x00FFFF00,
	Teal = 0x00808000,
	Blue = 0x0000FF00,
	Navy = 0x00008000,
	Fuchsia = 0xFF00FF00,
	Purple = 0x80008000
};

const Color BORDER_COLOR{ Black };
const Color SNAKE_COLOR{ Green };
const Color FOOD_COLOR{ Blue };
const int SNAKE_HEAD{ 0 };
const float SNAKE_VELOCITY{ BLOCK_SIZE * 5 };

enum Direction
{
	Exit = -1,
	None = 0,
	Up = 1,
	Down = 2,
	left = 3,
	right = 4
};


#endif	