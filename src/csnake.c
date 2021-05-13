/* See LICENSE file for copyright and license details.
 *
 * Csnake is a Snake clone written in C.
 * It uses the Ncurses library to display
 * graphics, such as:
 * 	- The snake;
 * 	- The snake's food;
 * 	- User score;
 * 	- Food (apples) eaten.
 *
 * Made by Salonia Matteo <saloniamatteo@pm.me>
 *
 */

#define _GNU_SOURCE
#define _FORTIFY_SOURCE 2
#define _POSIX_C_SOURCE 200809L

#include <stdbool.h>
#include <stdlib.h>
#include <libgen.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <unistd.h>
#include <ncurses.h>

/* Csnake versions (VER: string, NVER: number) */
#define __CSNAKE_MAJ_VER	"1"
#define __CSNAKE_MAJ_NVER	1
#define __CSNAKE_MIN_VER	"3"
#define __CSNAKE_MIN_NVER	3
#define __CSNAKE_VERSION	__CSNAKE_MAJ_VER "." __CSNAKE_MIN_VER

#define SNAKESIZE 100		/* Maximum snake size	*/
#define TAIL_SIZE 3		/* Default tail length	*/
#define FOOD_CHAR "@"		/* Which character to use for food */
#define SNAKECHAR ACS_CKBOARD	/* Which character to use for snake */

#define DIFF_EASY 90		/* Easy difficulty */
#define DIFF_MEDI 65		/* Medium difficulty */
#define DIFF_HARD 50		/* Hard difficulty */
#define DIFF_XTRM 35		/* Extreme difficulty */

#define RAND(min, max) ((rand() % (max - min + 1)) + min)

/* Custom types */
typedef unsigned int		uint32_ct;
typedef short unsigned int	uint8_ct;

typedef enum DIRECTION {
	LEFT	= 0,	/* h, KEY_LEFT	*/
	DOWN	= 1,	/* j, KEY_DOWN	*/
	UP	= 2,	/* k, KEY_UP	*/
	RIGHT	= 3,	/* l, KEY_RIGHT	*/
} DIRECTION;

typedef struct COORD {
	int x;
	int y;
} COORD;

typedef struct FOOD {
	int x;
	int y;
	int count;
} FOOD;

/* Global variables */
uint32_ct x = 0, y = 0;				/* Screen size */
uint32_ct max_x = 0, max_y = 0;			/* Max screen size */
uint32_ct nx = 0, ny = 0;			/* Next coordinates */
uint32_ct tail_len = TAIL_SIZE;			/* Current tail length */
DIRECTION direction = RIGHT;			/* Current snake direction */
COORD snake[SNAKESIZE] = {0};			/* Snake object */
FOOD food = {0};				/* Food object */
uint32_ct score = 0;				/* Player score */
uint8_ct diff = 1;				/* Difficulty */
uint8_ct win_timeout = 65;			/* Window timeout in ms */
uint32_ct usr_x = 0;				/* User-defined x */
uint32_ct usr_y = 0;				/* User-defined y */
bool borders = false;				/* Show borders? Default: no */
char filename[100] = {0};			/* Default file used to save scores */
bool savescore = false;				/* Save scores? Default: no */

/* Function prototypes */
WINDOW *newsubwin(int, int, int, int, char *);	/* Create new sub-window with borders */
void diffToWinTimeout(uint8_ct);		/* Convert difficulty to window timeout */
char *diffStr(void);				/* Return difficulty as a string */
int  setDiff(char *);				/* Convert string to difficulty */
void printHelp(char *);				/* Print help & usage, and exit */
void newFood(void);				/* Create food */
int  checkFile(char *, char *);			/* Check if we can write to file */
void saveScore(void);				/* Save scores to file */
void endSnk(WINDOW *);				/* End session */
uint32_ct randScore(void);			/* Return Random number between 1 and 10 */
void pauseMenu(void);				/* Create pause menu */
void exitConfirm(WINDOW *);			/* Check if user really wants to exit */
void scrUpd(WINDOW *);				/* Update screen */

/* Create new sub-window with borders */
WINDOW
*newsubwin(int height, int width, int starty, int startx, char *title)
{
	WINDOW *win = newwin(height, width, starty, startx);
	box(win, 0, 0);
	mvwprintw(win, 0, width / 2.5, title);
	wrefresh(win);
	return win;
}

/* Convert difficulty to Window Timeout */
void
diffToWinTimeout(uint8_ct diff)
{
	switch(diff) {
	case 0:
		win_timeout = DIFF_EASY;
		break;

	case 1: default:
		win_timeout = DIFF_MEDI;
		break;

	case 2:
		win_timeout = DIFF_HARD;
		break;

	case 3:
		win_timeout = DIFF_XTRM;
		break;
	}
}

/* Return a string containing difficulty */
char *
diffStr(void)
{
	switch(win_timeout) {
	case DIFF_EASY: default:
		return "Easy";
		break;

	case DIFF_MEDI:
		return "Medium";
		break;

	case DIFF_HARD:
		return "Hard";
		break;
	
	case DIFF_XTRM:
		return "Extreme";
		break;
	}
}

/* Set difficulty from string */
int
setDiff(char *arg)
{
	char diff = arg[0];

	switch(diff) {
	/* Easy */
	case 0: case 'e': case 'E': default:
		return 0;
		break;

	/* Medium */
	case 1:	case 'm': case 'M':
		return 1;
		break;

	/* Hard */
	case 2:	case 'h': case 'H':
		return 2;
		break;
	
	/* Extreme */
	case 3:	case 'x': case 'X':
		return 3;
		break;
	}
}

/* Print help and usage */
void
printHelp(char *progname)
{
	/* Reset ncurses settings */
	curs_set(1);	/* Show cursor */
	nocbreak();	/* Enable line buffering */
	endwin();	/* Delete ncurses window */

	printf("Csnake by Salonia Matteo v%s\n\n", __CSNAKE_VERSION);
	printf("Usage:\n\
%s	[-b | --borders ] [ -d | --difficulty <difficulty> ]\n\
	[ -h | --help ]\n\
	[ -x | --scr-x <maximum x> ] [ -y | --scr-y <maximum y> ]\n\
\n\
	-b, --borders		Draw screen borders.\n\
				Note: screen may flicker so, unless you have a\n\
				screen with a high refresh rate (>60Hz ?), you should\n\
				only use this option to test the -x & -y options.\n\
\n\
	-d, --difficulty diff	Set difficulty to diff, where diff is an integer\n\
				between 0 and 3, or a letter corresponding to\n\
				'e' (Easy), 'm' (Medium), 'h' (Hard), 'x' (Extreme).\n\
				0 is Easy, 1 is Medium, 2 is Hard, 3 is Extreme.\n\
\n\
	-h, --help		Display this help screen.\n\
\n\
	-s, --save savefile	Save scores to file savefile.\n\
\n\
	-x, --scr-x x_val	Modify maximum screen x to x_val.\n\
\n\
	-y, --scr-y y_val	Modify maximum screen y to y_val.\n\
\n\
Submit any bugs or issues to Matteo Salonia <saloniamatteo@pm.me>.\n\
\n\
If you're a developer and want to know more about csnake, read the\n\
documentation installed in /usr/share/doc/csnake/csnake-doc.rst, or on GitHub:\n\
https://github.com/saloniamatteo/csnake/blob/master/csnake-doc.rst\n\
",
basename(progname));

	exit(0);
}

/* Create food */
void
newFood(void)
{
	/* If score is 0, always start at the screen center */
	if (score == 0) {
		food.x = x / 2;
		food.y = y / 2;
	} else {
		/* Set seed for pseudo-random generator */
		srand(time(NULL));

		/* Store a pseudo-random integer */
		uint32_ct rand_x, rand_y = {0};

		/* Check if user has enabled borders, in which case
		 * we have to restrict maximum possible coordinates */
		const int start_x = borders == true ? 3 : 2;
		const int start_y = borders == true ? 4 : 3;
		const int end_x = max_x - (borders == true ? 2 : 1);
		const int end_y = max_y - (borders == true ? 2 : 1);

		/* Generate random values, never allowing food to spawn behind text */
		rand_x = RAND(start_x, end_x);
		rand_y = RAND(start_y, end_y);

		/* Set food coordinates to pseudo-random integers */
		food.y = rand_y;	/* y */
		food.x = rand_x;	/* x */
	}
}

/* Check if we can write to file */
int
checkFile(char *file, char *name)
{
	/* Check if we are trying to write to ourselves */
	if (!strcmp(file, name) || !strcmp(file, basename(name)))
		return -1;

	/* Check if file can be opened for writing */
	FILE *tmp = fopen(file, "a+w");

	if (file == NULL)
		return 0;
	else {
		fclose(tmp);
		return 1;
	}
}

/* Save scores to file */
void
saveScore(void)
{
	/* Open file */
	FILE *tmp = fopen(filename, "a+w");

	/* Get current time */
	time_t cur_time = time(NULL);
	struct tm *tm = localtime(&cur_time);

	/* Write to file */
	fprintf(tmp, "\
----------------------------------------\n\
Date and time:	%s\
Score:		%04d\n\
Apples eaten:	%04d\n\
Difficulty:	%s\n\
----------------------------------------\n\
",
asctime(tm), score, food.count, diffStr());

	/* Close file */
	fclose(tmp);
}

/* End session */
void
endSnk(WINDOW *win)
{
	/* Clear screen */
	clear();
	refresh();

	/* Save scores, if requested by user */
	if (savescore)
		saveScore();

	/* Show final score */
	WINDOW *fwin = newsubwin(4, 65, y / 2, x / 4, "Csnake");
	mvwprintw(fwin, 1, 2, "You lose! Score: %04d; Apples eaten: %04d; Difficulty: %s", score, food.count, diffStr());
	mvwaddstr(fwin, 2, 15, "Press any key to exit...");
	wrefresh(fwin);

	/* Wait for user input to exit */
	for (;;) {
		/* Key was pressed */
		if (wgetch(win) != ERR) {
			/* Reset ncurses settings */
			curs_set(1);	/* Show cursor */
			nocbreak();	/* Enable line buffering */
			keypad(win, 0);	/* Disable keypad mode */

			/* Exit cleanly */
			delwin(fwin);
			endwin();
			exit(0);
		}
	}
}

/* Return a number between 1 and 10 */
uint32_ct
randScore(void)
{
	/* Set seed for pseudo-random generator */
	srand(time(NULL));

	/* Store random value */
	uint32_ct val = rand();

	/* If value is above 10, divide it by 2 */
	while (val > 10)
		val /= 2;

	/* Return value */
	return val;
}

/* Create new pause menu window */
void
pauseMenu(void)
{
	clear();
	refresh();

	/* Show pause menu window */
	WINDOW *pmenuwin = newsubwin(3, 30, y / 2, x / 3, "Csnake");
	mvwprintw(pmenuwin, 1, 2, "Press any key to resume...");

	/* Show pause menu */
	while (getch() == ERR) {
		wrefresh(pmenuwin);
	}

	/* Delete pause menu window */
	delwin(pmenuwin);
}

/* Check if user really wants to exit */
void
exitConfirm(WINDOW *win)
{
	clear();
	refresh();

	/* Show exit confirmation window */
	WINDOW *exitwin = newsubwin(4, 30, y / 2, x / 3, "Csnake");
	mvwprintw(exitwin, 1, 2, "Do you really want to exit?");
	mvwprintw(exitwin, 2, 2, "Press 'y' or 'Y' to exit.");
	wrefresh(exitwin);

	for (;;) {
		char ch = getch();
		if (ch != ERR && (ch == 'y' || ch == 'Y'))
			endSnk(win);
		else if (ch != ERR)
			break;
	}

	delwin(exitwin);
}

/* Update screen */
void
scrUpd(WINDOW *win)
{
	/* Clear screen */
	clear();

	/* Draw snake */
	for (int i = 0; i < tail_len; i++)
		mvwaddch(win, snake[i].y, snake[i].x, SNAKECHAR);

	/* Draw food */
	mvwprintw(win, food.y, food.x, FOOD_CHAR);

	/* Refresh screen */
	refresh();
}

int
main(int argc, char **argv)
{
	/* Set locale */
	setlocale(LC_ALL, "C");

	/* Parse commandline arguments */
	static struct option longopts[] = {
		{"borders",	no_argument,		NULL, 'b'},
		{"difficulty",	required_argument,	NULL, 'd'},
		{"help",	no_argument,		NULL, 'h'},
		{"save",	optional_argument,	NULL, 's'},
		{"scr-x",	required_argument,	NULL, 'x'},
		{"scr-y",	required_argument,	NULL, 'y'},
	};

	int optind = 0;

	while ((optind = getopt_long(argc, argv, ":bd:hs:x:y:", longopts, &optind)) != 1) {
		switch (optind) {

		/* Show screen borders */
		case 'b':
			borders = true;
			fprintf(stderr, "[Showing borders]\n");
			break;

		/* Adjust difficulty */
		case 'd':
			diff = setDiff(optarg);
			diffToWinTimeout(diff);
			fprintf(stderr, "[Using difficulty %s]\n", diffStr());
			break;

		/* Print help & usage */
		case 'h': case '?': case '-': case ':':
			printHelp(argv[0]);
			break;

		/* Save scores to file */
		case 's':
			/* Check if we can write to file */
			int file_stat = checkFile(optarg, argv[0]);

			switch(file_stat) {
			case -1:
				fprintf(stderr, "You are trying to overwrite this file! Exiting.\n");
				return 1;
			case 0:
				fprintf(stderr, "File %s cannot be opened! Exiting.\n", optarg);
				return 1;
			default:
				/* Check if argument is too long */
				int arg_len = strnlen(optarg, sizeof(filename));

				if (arg_len == sizeof(filename)) {
					fprintf(stderr, "Filename is too big (%d)! Exiting.\n", arg_len);
					return 1;

				} else {
					strncpy(filename, optarg, arg_len);
					savescore = true;
					fprintf(stderr, "[Using %s to save scores]\n", filename);
				}
			}

			break;

		/* Modify maximum screen x */
		case 'x':
			if (atoi(optarg) > 0) {
				usr_x = atoi(optarg);
				fprintf(stderr, "[Using x = %d]\n", usr_x);
			} else
				fprintf(stderr, "[x Value not Valid, Using Default]\n");
			break;

		/* Modify maximum screen y */
		case 'y':
			if (atoi(optarg) > 0) {
				usr_y = atoi(optarg);
				fprintf(stderr, "[Using y = %d]\n", usr_y);
			} else
				fprintf(stderr, "[y Value not Valid, Using Default]\n");
			break;

		default:
			break;
		}

		if (optind <= 0)
			break;
	}

	/* Initialise ncurses screen */
	WINDOW *win = initscr();

	/* Save screen size */
	getmaxyx(win, y, x);
	getmaxyx(win, max_y, max_x);

	max_x = usr_x != 0 ? usr_x : max_x;	/* If x was passed, set max_x to usr_x */
	max_y = usr_y != 0 ? usr_y : max_y;	/* If y was passed, set max_y to usr_y */

	noecho();			/* Don't echo keypresses */
	cbreak();			/* Disable line buffering, speeding up key input */
	curs_set(0);			/* Hide cursor (set visibility to 0) */
	keypad(win, 1);			/* Enable keypad for current window */
	wtimeout(win, win_timeout);	/* Set window timeout, according to difficulty */

	/* Show program info */
	WINDOW *introwin = newsubwin(3, 45, y / 2, x / 3, "Csnake");
	mvwprintw(introwin, 1, 2, "Csnake by Salonia Matteo v%s; Starting...", __CSNAKE_VERSION);
	wrefresh(introwin);

	/* Wait 1 second before starting */
	sleep(1);

	/* Clear screen */
	clear();
	delwin(introwin);
	refresh();

	/* Initial snake coordinates */
	for(int i = TAIL_SIZE, k = 0; i >= 0; i--, k++) {
		/* Default location */
		COORD cur_coord;
		cur_coord.x = i;
		cur_coord.y = y / 2;

		/* Set snake coordinates */
		snake[k] = cur_coord;
	}

	/* Create food */
	newFood();

	for (;;) {
		/* Draw score */
		mvwprintw(win, 0, 1, "(%s) Score: %04d; Apples eaten: %04d; Current Length: %04d",
				diffStr(), score, food.count, tail_len);

		/* Show screen borders, if option is enabled */
		if (borders) {
			/* Horizontal Borders */
			for (int i = 0; i <= max_x; i++) {
				mvwaddch(win, 1, i, '-');		/* Top Border */
				mvwaddch(win, max_y - 1, i, '-');	/* Bottom Border */
			}

			/* Vertical Borders */
			for (int i = 1; i <= max_y; i++) {
				mvwaddch(win, i, 0, '|');		/* Left Border */
				mvwaddch(win, i, max_x - 1, '|');	/* Right Border */
			}
		}

		/* Get key */
		uint8_ct key = wgetch(win);

		/* Check which key was pressed */
		if ((key == 'h' || key == KEY_LEFT) && (direction != LEFT && direction != RIGHT))	/* Left */
			direction = LEFT;
		else if ((key == 'j' || key == KEY_DOWN) && (direction != DOWN && direction != UP))	/* Down */
			direction = DOWN;
		else if ((key == 'k' || key == KEY_UP) && (direction != DOWN && direction != UP))	/* Up */
			direction = UP;
		else if ((key == 'l' || key == KEY_RIGHT) && (direction != LEFT && direction != RIGHT))	/* Right */
			direction = RIGHT;
		else if (key == '' || key == 'p')	/* "ESC" key or "p" key was pressed */
			pauseMenu();
		else if (key == 'q')			/* "q" key was pressed */
			exitConfirm(win);

		/* Set next coordinates to snake's head position */
		nx = snake[0].x;
		ny = snake[0].y;

		/* Check current direction and update next coordinates */
		switch (direction) {
		case LEFT:
			nx--;	/* (x - 1, y) */
			break;
		case DOWN:
			ny++;	/* (x, y + 1) */
			break;
		case UP:
			ny--;	/* (x, y - 1)*/
			break;
		case RIGHT:
			nx++;	/* (x + 1, y) */
			break;
		}

		/* If snake has eaten food */
		if (nx == food.x && ny == food.y) {
			/* Update food count */
			food.count++;

			/* Set tail position */
			COORD newtail;
			newtail.x = nx;
			newtail.y = ny;

			/* Set snake's tail position to newtail */
			snake[tail_len] = newtail;

			/* If tail length is above limit, roll back */
			if (tail_len <= SNAKESIZE)
				tail_len++;
			else
				tail_len = TAIL_SIZE;

			/* Increase score by a number between 1 and 10 */
			score += randScore();

			/* New food location */
			newFood();

		/* Snake did not eat food */
		} else {
			/* Draw snake */
			for(int i = 0; i < tail_len; i++) {
				/* Check if snake has collided into itself */
				if(snake[i].x == nx && snake[i].y == ny) {
					endSnk(win);
					break;
				}
			}

			/* Set the tail as the new head */
			snake[tail_len - 1].x = nx;
			snake[tail_len - 1].y = ny;
		}

		/* Move snake parts */
		COORD tmp = snake[tail_len - 1];
		for(int i = tail_len - 1; i > 0; i--) {
			snake[i] = snake[i-1];
		}
		snake[0] = tmp;

		/* If snake has hit screen edges, end game */
		if ((nx >= x || nx < 0) || (ny >= y || ny < 0))
			endSnk(win);

		/* Update Screen */
		scrUpd(win);
	}

	/* End game */
	endSnk(win);
	return 0;
}
