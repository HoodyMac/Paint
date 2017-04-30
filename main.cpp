#include "main.h"

int main() {
	Canvas canvas;

	int status = NO_ACTION;

	while (status != EXIT) {
		status = canvas.input();
		if (status == REDRAW) {
			canvas.draw();
		}
	}

	return EXIT_SUCCESS;
}