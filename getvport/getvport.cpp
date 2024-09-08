#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>  
using namespace std;

// flag for window resizing
volatile sig_atomic_t window_resized = 0;

// catch SIGWINCH
void handle_winch(int sig) {
  window_resized = 1;				// set the flag
  signal(SIGWINCH, handle_winch); 	// reinstall the signal handler
}

WINDOW* showbox(WINDOW* parent) {
  // initialise
  WINDOW* child;
  char* pos_str;

  // child window initial parameters
  int width = 12, height = 5;
  int x = (COLS - width) / 2;
  int y = (LINES - height) / 2;

  // create child window (aka the box)
  child = subwin(parent, height, width, y, x);
  box(child, 0, 0);

  // string with viewport size
  asprintf(&pos_str, " %d x %d", COLS, LINES);
    
  // print the string into the box
  mvwaddstr(child, 2, 1, pos_str);
  wrefresh(child);

  // deallocate the string
  free(pos_str);

  return child;
}

WINDOW* makeparent() {
  WINDOW *parent;
  if ((parent = initscr()) == NULL) {
	fprintf(stderr, "Error starting ncurses!\n");
	exit(EXIT_FAILURE);
  }
  noecho();
  keypad(parent, TRUE);
  return parent;
}

int main(void) {
  /* initialisation */
  WINDOW *parent, *child;
  int ch;

  // handle SIGWINCH signal (window resize)
  signal(SIGWINCH, handle_winch);

  //
  parent = makeparent();

  /**/
  nodelay(stdscr, TRUE);
  timeout(100);

  /**/
  child = showbox(parent);

  while (1) {
	ch = getch();
	if (ch == 'q' || ch == 27) {
		break;
	}

	if (window_resized == 1) {
		endwin();
		parent = makeparent();
		clear();
		child = showbox(parent);
		refresh();
		window_resized = 0;
	}
  }

  delwin(child);
  delwin(parent);
  endwin();

  return EXIT_SUCCESS;
}
