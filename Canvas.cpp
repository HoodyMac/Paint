#include "Canvas.h"

Canvas::Canvas()
{
	SMALL_RECT screenRect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(hOutput, TRUE, &screenRect);
	hInput = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };

	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			if (y > CANVAS_HEIGHT && x < 16) {
				screenBuffer[x + y * SCREEN_WIDTH].Attributes = (x % 16) << 4;
			}
			screenBuffer[x + y * SCREEN_WIDTH].Char.AsciiChar = ' ';
		}
	}

	currentColor = BACKGROUND_GREEN;
	setCurrentColor(currentColor);

	canvasRect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	writeStart = { 0, 0 };

	WriteConsoleOutput(hOutput, screenBuffer, bufferSize, writeStart, &canvasRect);
}


Canvas::~Canvas()
{
}

void Canvas::draw() {
	WriteConsoleOutput(hOutput, screenBuffer, bufferSize, writeStart, &canvasRect);
}

void Canvas::drawPoint(int x, int y, int color) {
	screenBuffer[x + y * SCREEN_WIDTH].Attributes = color;
}

void Canvas::setCurrentColor(int color) {
	screenBuffer[(currentColor >> 4) + 50 * SCREEN_WIDTH].Char.AsciiChar = ' ';
	currentColor = color;
	screenBuffer[(currentColor >> 4) + 50 * SCREEN_WIDTH].Char.AsciiChar = 'V';
	screenBuffer[(currentColor >> 4) + 50 * SCREEN_WIDTH].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
}

int Canvas::input() {
	INPUT_RECORD inputRecord;
	DWORD events = 0;

	ReadConsoleInput(hInput, &inputRecord, 1, &events);

	if (inputRecord.EventType == MOUSE_EVENT) {
		int x = inputRecord.Event.MouseEvent.dwMousePosition.X;
		int y = inputRecord.Event.MouseEvent.dwMousePosition.Y;
		if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			if (y > CANVAS_HEIGHT && x < 16) {
				setCurrentColor(inputRecord.Event.MouseEvent.dwMousePosition.X << 4);
			}
			else {
				drawPoint(x, y, currentColor);
			}
			return REDRAW;
		}
		else if (inputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
			drawPoint(x, y, 0);
			return REDRAW;
		}
		else if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED) {
			setCurrentColor(screenBuffer[x + y * SCREEN_WIDTH].Attributes);
			return REDRAW;
		}
	}
	else if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
		if (inputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
			return EXIT;
		}
	}

	return NO_ACTION;
}
