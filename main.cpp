#include "main.h"

int main() {
	Canvas canvas;

	while (1) {
		if (canvas.input()) {
			canvas.draw();
		}
	}

	return 0;
}