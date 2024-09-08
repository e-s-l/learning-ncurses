#include <ncurses.h>
#include <stdlib.h>
using namespace std;

int main(void) {

	// initialise
	WINDOW * parent, * child;
	int ch;

	char* pos_str;

	// start ncurses
	if ((parent = initscr()) == NULL) {
		fprintf(stderr, "Error starting ncurses!\n");
		exit(EXIT_FAILURE);
	}

	// child window initial parameters
	int width = 10, height = 5;
	int rows = LINES, cols = COLS;
	int x = (cols - width)/2;
	int y = (rows - height)/2;

	// enable arrow keys on main window
	keypad(parent, TRUE);
	noecho();

	// create child window aka the box
	child = subwin(parent, height, width, y, x);
	box(child, 0, 0);
	mvwaddstr(child, 2, 1, pos_str);
	refresh();


	// start loop that exits if q is pressed
	while((ch=getch())!='q') {

		// a switch for the key directions

		switch (ch) {
			case KEY_UP:
				if (y > 0)
					--y;
					break;
			case KEY_DOWN:
				if (y < (rows - height))
					++y;
					break;
			case KEY_LEFT:
				if (x > 0)
					--x;
					break;		
			case KEY_RIGHT:
				if (x < (cols-width))
					++x;
					break;		
		}

		//movr
		mvwin(child, y, x);

		// reload string with new coordinates
		asprintf(&pos_str, " %d, %d", y, x);
		// print the string into the box
		mvwaddstr(child, 2, 1, pos_str);
		// refresh box with new string and position
		wrefresh(child);

		// if the borders are hit, restart
		if ((y == 0) || (x == 0) || (y + height == rows) || (x + width == cols)) {
			x = (cols - width)/2;
			y = (rows - height)/2;
			clear();
			box(child, 0, 0);
		}

		// if x == y, do something special

	}

	// clear the window
	delwin(child);
	delwin(parent);
	endwin();

	// deallocate the strings memory
	free(pos_str);

	return EXIT_SUCCESS;
}
    