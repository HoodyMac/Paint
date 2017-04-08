#include <windows.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 50

bool input(CHAR_INFO screenBuffer[]) {
	INPUT_RECORD inputRecord;
	DWORD events = 0;
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	ReadConsoleInput(hInput, &inputRecord, 1, &events);

	if (inputRecord.EventType == MOUSE_EVENT) {
		if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			int x = inputRecord.Event.MouseEvent.dwMousePosition.X;
			int y = inputRecord.Event.MouseEvent.dwMousePosition.Y;
			screenBuffer[x + y * SCREEN_WIDTH].Attributes = BACKGROUND_GREEN;
			return true;
		}
		else if (inputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
			int x = inputRecord.Event.MouseEvent.dwMousePosition.X;
			int y = inputRecord.Event.MouseEvent.dwMousePosition.Y;
			screenBuffer[x + y * SCREEN_WIDTH].Attributes = 0;
			return true;
		}
	}
	else if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
		if (inputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
			exit(0);
		}
	}

	return false;
}

int main() {
	SMALL_RECT screenRect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleWindowInfo(hOutput, TRUE, &screenRect);

	CHAR_INFO screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT] = { 0 };
	COORD bufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };

	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			screenBuffer[x + y * SCREEN_WIDTH].Char.AsciiChar = ' ';
			//screenBuffer[x + y * SCREEN_WIDTH].Attributes = BACKGROUND_GREEN;
		}
	}

	SMALL_RECT canvasRect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };
	COORD writeStart = { 0, 0 };

	while (1) {
		if (input(screenBuffer)) {
			WriteConsoleOutput(hOutput, screenBuffer, bufferSize, writeStart, &canvasRect);
		}
	}

	return 0;
}