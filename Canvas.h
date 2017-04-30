#pragma once

#include "main.h"

class Canvas
{
public:
	Canvas();
	~Canvas();

	int input();
	void draw();
private:
	void setCurrentColor(int color);
	void drawPoint(int x, int y, int color);

	int currentColor;
	CHAR_INFO screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT] = { 0 };

	HANDLE hInput;
	HANDLE hOutput;
	SMALL_RECT canvasRect;
	COORD writeStart;
	COORD bufferSize;
};

