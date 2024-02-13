#include <panel.h>
#include <string.h>
#include <signal.h>
#include "arborescence.c"

int execute(arbre* previous, char name[], char absolute_path[]);
void init_wins(WINDOW **wins, arbre **dirs, arbre **files, int taille_dirs, int taille_files);
void win_show(WINDOW *win, int n, char *label, arbre **dirs, arbre **files, int taille_dirs, int taille_files, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
char *substr(char *src, int len);

/*
Handler for sigint
*/
void sigintHandler(int signal) {
	// Cleanup of windows
	endwin();
  exit(0);
}

int main() {
	// Setup of sigint signal trap
	signal(SIGINT, sigintHandler);
	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);

	// Call to 'execute' function on the path you want
	execute(NULL, "polytech", "/home/roussalex/polytech");
}

/*
Main function for the key stroke processing, windows init/update, and full logic
Parameters :
  previous (arbre*) : previous node of the one we want to execute
  name (char[]) : name of the dir we want to execute
  absolute_path (char[]) : absolute_path of the dir we want to execute
Returns :
	res (int) : 1 for stopping and cd or open current file/dir, 0 for going back one step in the file system
*/
int execute(arbre* previous, char name[], char absolute_path[])
{
	// Variables for windows and panels
	WINDOW *my_wins[2];
	PANEL  *my_panels[2];
	PANEL  *top;
	int n_top;
	int ch;

	// Creation of the first node with 'execute' parameters
	arbre* head = creer_arbre(previous, 1, name, absolute_path);
	arbre** dirs = head->next_dirs;
	arbre** files = head->next_files;
	int n_dirs = head->n_dirs;
	int n_files = head->n_files;

	// Init of windows and display of files and dirs
	init_wins(my_wins, dirs, files, n_dirs, n_files);
	
	/* Attach a panel to each window */
	my_panels[0] = new_panel(my_wins[0]);
	my_panels[1] = new_panel(my_wins[1]);

	/* Set up the user pointers to the next panel */
	set_panel_userptr(my_panels[0], my_panels[1]);
	set_panel_userptr(my_panels[1], my_panels[0]);

	/* Update the stacking order. Directories (0) panel will be on top */
	update_panels();

	/* Show it on the screen */
	attron(COLOR_PAIR(4));
	mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
	attroff(COLOR_PAIR(4));

	// Init of cursor position on the first dir
	int cursorY = 3;
	// Top panel variable
	n_top = 0;
	top = my_panels[0];
	update_panels();
	// Calculation of cursor x position
	int length = strlen(substr(dirs[cursorY-3]->name, COLS/2-4));
	// Positionning of cursor
	if (length % 2 == 0) {
		wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
	} else {
		wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
	}
	// Refresh of windows to update the changes
	wrefresh(my_wins[n_top]);
	doupdate();

	// Scan of key strokes (F1 stops the program)
	while((ch = getch()) != KEY_F(1))
	{	
		switch(ch)
		{	
			case 9:
				// 9 is ASCII code of tabulation key, used to switch between files/dirs panels
				top = (PANEL *)panel_userptr(top);
				top_panel(top);
				n_top = n_top == 1 ? 0 : 1;
				cursorY = 3;
				if (n_top == 0) {
					// Positionning of cursor for dirs
					length = strlen(substr(dirs[cursorY-3]->name, COLS/2-4));
					if (length % 2 == 0) {
						wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
					} else {
						wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
					}
				} else {
					// Positionning of cursor for files
					length = strlen(substr(files[cursorY-3]->name, COLS/2-4));
					if (length % 2 == 0) {
						wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
					} else {
						wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
					}
				}
				// Refresh of windows to update the changes
				wrefresh(my_wins[n_top]);
				update_panels();
				break;
			case 10:
				// 10 is ASCII code of enter key, used to 'cd' if cursor on dir, 'open' if cursor on files
				// 'cd' and 'open' is performed by bash script named 'navigation'
				if (n_top == 0 && n_dirs > 0) {
					// Bash code for cd to dir is '1:/absolute/path/to/directory'
					printf("1:%s\n", dirs[cursorY-3]->absolute_path);
				} else if (n_top == 1 && n_files > 0) {
					// Bash code for open file is '2:/absolute/path/to/file'
					printf("2:%s\n", files[cursorY-3]->absolute_path);
				}
				// Cleanup of windows
				endwin();
				// End of program
				return 1;
			case KEY_RIGHT:
				// Used to enter in a dir
				if (n_top == 0 && n_dirs > 0) {
					// Prevents key right on a file
					// execute on the sons of head and storing res in variable
					int res = execute(head, dirs[cursorY-3]->name, dirs[cursorY-3]->absolute_path);
					if (res == 1) {
						// Condition to stop the programm recursively
						return 1;
					}
					// Init of windows to update windows after going back from 'execute' function above
					init_wins(my_wins, dirs, files, n_dirs, n_files);
					update_panels();
					// Positionning of cursor
					length = strlen(substr(dirs[cursorY-3]->name, COLS/2-4));
					if (length % 2 == 0) {
						wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
					} else {
						wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
					}
					// Refresh of windows to update the changes
					wrefresh(my_wins[n_top == 0 ? 1 : 0]);
					wrefresh(my_wins[n_top]);
					doupdate();
				}
				break;
			case KEY_LEFT:
				// Used to go back from a dir
				if (n_top == 0 && head->previous != NULL) {
					return 0;
				}
				break;
			case KEY_UP:
				// Used to browse in dirs/files
				if (cursorY > 3) {
					cursorY--;
					if (n_top == 0) {
						// Positionning of cursor for dirs
						length = strlen(substr(dirs[cursorY-3]->name, COLS/2-4));
						if (length % 2 == 0) {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
						} else {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
						}
					} else {
						// Positionning of cursor for files
						length = strlen(substr(files[cursorY-3]->name, COLS/2-4));
						if (length % 2 == 0) {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
						} else {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
						}
					}
					// Refresh of windows to update the changes
					wrefresh(my_wins[n_top]);
				}
				break;
			case KEY_DOWN:
				// Used to browse in dirs/files
				if ((n_top == 0 && cursorY < n_dirs+2) || (n_top == 1 && cursorY < n_files+2)) {
					cursorY++;
					if (n_top == 0) {
						// Positionning of cursor for dirs
						length = strlen(substr(dirs[cursorY-3]->name, COLS/2-4));
						if (length % 2 == 0) {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
						} else {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
						}
					} else {
						// Positionning of cursor for files
						length = strlen(substr(files[cursorY-3]->name, COLS/2-4));
						if (length % 2 == 0) {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-1);
						} else {
							wmove(my_wins[n_top], cursorY, COLS/4-length/2-2);
						}
					}
					// Refresh of windows to update the changes
					wrefresh(my_wins[n_top]);
				}
				break;
		}
		doupdate();
	}
	endwin();
	return 1;
}

/*
Init and displaying of the windows for files/dirs
Parameters :
  wins (WINDOW**) : Windows for files and dirs
  dirs (arbre**) : arbres that are directories
  files (arbre**) : arbres that are files
  taille_dirs (int) : number of dirs in dirs
  taille_files (int) : number of files in files
Returns :
  void
*/
void init_wins(WINDOW **wins, arbre **dirs, arbre **files, int taille_dirs, int taille_files) {
	char label[80];
	wins[0] = newwin(LINES/1.25, COLS/2, 2, 0);
	sprintf(label, "Directories");
	win_show(wins[0], 0, label, dirs, files, taille_dirs, taille_files, 1);

	wins[1] = newwin(LINES/1.25, COLS/2, 2, COLS/2);
	sprintf(label, "Files");
	win_show(wins[1], 1, label, dirs, files, taille_dirs, taille_files, 2);
}

/*
Prints data in parameters
Parameters :
  win (WINDOW*) : current window for printing
  n (int) : number of the panel, 0 for dirs, 1 for files
  label (char*) : label for the window top part
  dirs (arbre**) : arbres that are directories
  files (arbre**) : arbres that are files
  taille_dirs (int) : number of dirs in dirs
  taille_files (int) : number of files in files
  label_color (int) : color number for the window
Returns :
  void
*/
void win_show(WINDOW *win, int n, char *label, arbre **dirs, arbre **files, int taille_dirs, int taille_files, int label_color) {
	int startx, starty, height, width;

	// mysterious code that works ¯ \ _ (ツ) _ / ¯
	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);

	box(win, 0, 0);
	mvwaddch(win, 2, 0, ACS_LTEE); 
	mvwhline(win, 2, 1, ACS_HLINE, width - 2); 
	mvwaddch(win, 2, width - 1, ACS_RTEE); 
	
	print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));

	if ( n == 0) {
		for (int i=0; i<taille_dirs; i++) {
			print_in_middle(win, 3+i, 0, width, dirs[i]->name, COLOR_PAIR(label_color));
		}
	} else if (n == 1) {
		for (int i=0; i<taille_files; i++) {
			print_in_middle(win, 3+i, 0, width, files[i]->name, COLOR_PAIR(label_color));
		}
	}
}

/*
Takes a string and returns a substring
Parameters :
  src (char*) : source string
  len (int) : length of destinationn string
Returns :
  dest (char*) : substring of src
*/
char *substr(char *src, int len) { 
  char *dest=NULL;
  if (len>0) {
    /* allocation et mise à zéro */
    dest = calloc(len+1, 1);
    /* vérification de la réussite de l'allocation*/
    if(NULL != dest) {
        strncat(dest,src,len);
    }
  }
  return dest;
}

/*
Prints data in parameters in the middle of window
Parameters :
  win (WINDOW*) : current window for printing
  starty (int) : y position for printing
  startx (int) : x position for printing
  width (int) : width of the window
  string (char*) : string to be printed
  color (chtype) : color of the string to be printed
Returns :
  void
*/
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	// Calculation of the length of the string
	length = strlen(substr(string,COLS/2-4));
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	// Printing of string
	wattron(win, color);
	mvwprintw(win, y, x, "%s", substr(string,COLS/2-4));
	wattroff(win, color);
	refresh();
}