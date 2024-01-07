#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handle_resize(int signum) {
    endwin(); // End ncurses mode
    printf("Window resizing is not allowed. Please restart this program.\n");
    exit(EXIT_FAILURE);
}

int main() {
    // Initialize ncurses
    initscr(); // Initialize the library
    cbreak(); // Disable line buffering
    noecho(); // Disable character echoing
    keypad(stdscr, TRUE); // Enable special keys like function keys and arrow keys
    curs_set(FALSE); // Hide the cursor

    signal(SIGWINCH, handle_resize);

    // Create a window
    WINDOW* win = newwin(5, 20, 10, 30); // (height, width, starty, startx)
    box(win, 0, 0); // Draw a border around the window

    // Print "Hello, ncurses!" inside the window
    mvwprintw(win, 2, 5, "Hello, ncurses!");

    // Refresh the screen to display changes
    refresh();
    wrefresh(win);

    // Wait for user input
    getch();

    // Clean up and exit
    delwin(win); // Delete the window
    endwin(); // Clean up ncurses
    return 0;
}
