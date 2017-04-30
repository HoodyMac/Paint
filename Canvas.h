#pragma once

#include "main.h"

class Canvas
{
public:
	Canvas();
	~Canvas();

	bool input();
	void draw();
private:
	void setCurrentColor(int color);
	void drawPoint(int x, int y, int color);

	HANDLE hInput;
	int currentColor;
	HANDLE hOutput;
	SMALL_RECT canvasRect;
	COORD writeStart;
	CHAR_INFO screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT] = { 0 };
	COORD bufferSize;
};

