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

#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

/* Csnake versions (VER: string, NVER: number) */
#define __CSNAKE_MAJ_VER	"1"
#define __CSNAKE_MAJ_NVER	1
#define __CSNAKE_MIN_VER	"0"
#define __CSNAKE_MIN_NVER	0
#define __CSNAKE_VERSION	__CSNAKE_MAJ_VER "." __CSNAKE_MIN_VER

#define SNAKESIZE 100		/* Maximum snake size	*/
#define TAIL_SIZE 3		/* Default tail length	*/
#define W_TIMEOUT 65		/* Window Timeout, Snake Speed */
#define FOOD_CHAR "@"		/* Which character to use for food */
#define SNAKECHAR ACS_CKBOARD	/* Which character to use for snake */

/* Window borders */
/* Use ASCII borders? (Default: No (0)) */
#define BORDER_USEASCII 0

#if BORDER_USEASCII == 0
#define BORDER_LU "┌"		/* Upper-Left	(Left-Up)	border */
#define BORDER_LD "└"		/* Lower-Left	(Left-Down)	border */
#define BORDER_RU "┐"		/* Upper-Right	(Right-Up)	border */
#define BORDER_RD "┘"		/* Lower-Right	(Right-Down)	border */
#define BORDER_SB "│"		/* Lateral	(Side-Border)	border */
#else
#define BORDER_LU "+"		/* Upper-Left	(Left-Up)	border */
#define BORDER_LD "+"		/* Lower-Left	(Left-Down)	border */
#define BORDER_RU "+"		/* Upper-Right	(Right-Up)	border */
#define BORDER_RD "+"		/* Lower-Right	(Right-Down)	border */
#define BORDER_SB "|"		/* Lateral	(Side-Border)	border */
#endif

/* Shorten "unsigned int" to "uint32_ct" */
typedef unsigned int uint32_ct;

/* Structures and Enums */
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

/* Screen size */
uint32_ct x = 0, y = 0;

/* Next coordinates */
uint32_ct nx = 0, ny = 0;

/* Current tail length */
uint32_ct tail_len = TAIL_SIZE;

/* Current snake direction */
DIRECTION direction = RIGHT;

/* Snake object */
COORD snake[SNAKESIZE] = {0};

/* Food object */
FOOD food = {0};

/* Player score */
uint32_ct score = 0;

/* Function prototypes */
void newFood(void);		/* Create food */
void endSnk(WINDOW *);		/* End session */
uint32_ct randScore(void);	/* Return Random number between 1 and 10 */
void pauseMenu(WINDOW *);	/* Create pause menu */
void scrUpd(WINDOW *);		/* Update screen */

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

		/* Start with small numbers */
		rand_x = (random() >> 5);
		rand_y = (random() >> 5);

		/* Check if random values are greater than screen size;
		 * if they are, right-shift the random values by 3 bits,
		 * getting a smaller number without wasting too much time. */
		while (rand_x >= x)
			rand_x = rand_x >> 3;
		while (rand_y >= y)
			rand_y = rand_y >> 3;

		/* If random values are <= 0, get a new random number */
		if (rand_x <= 0 || rand_y <= 0)
			newFood();

		/* Set food coordinates to pseudo-random integers */
		food.y = rand_y;	/* y */
		food.x = rand_x;	/* x */
	}
}

/* End session */
void
endSnk(WINDOW *win)
{
	/* Reset ncurses settings */
	curs_set(1);	/* Show cursor */
	nocbreak();	/* Enable line buffering */
	keypad(win, 0);	/* Disable keypad mode */

	/* Clear screen */
	clear();
	refresh();

	/* Show final score */
	mvwprintw(win, (y / 2) + 0, x / 2.5, "%s-------------------Csnake------------------%s\n", BORDER_LU, BORDER_RU);
	mvwprintw(win, (y / 2) + 1, x / 2.5, "%s You lose! Score: %04d; Apples eaten: %04d %s\n", BORDER_SB, score, food.count, BORDER_SB);
	mvwprintw(win, (y / 2) + 2, x / 2.5, "%s-------------------------------------------%s\n", BORDER_LD, BORDER_RD);
	refresh();

	/* Wait for user input to exit */
	mvwaddstr(win, y - 2, 0, "Press any key to exit...\n");
	refresh();
	for (;;) {
		/* Key was pressed */
		if (wgetch(win) != ERR) {
			/* Exit */
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
pauseMenu(WINDOW *win)
{
	/* Clear screen */
	clear();
	refresh();

	/* Show window */
	while (wgetch(win) == ERR) {
		mvwprintw(win, (y / 2) + 0, x / 2.5, "%s-----------Csnake-----------%s", BORDER_LU, BORDER_RU);
		mvwprintw(win, (y / 2) + 1, x / 2.5, "%s Press any key to resume... %s", BORDER_SB, BORDER_SB);
		mvwprintw(win, (y / 2) + 2, x / 2.5, "%s----------------------------%s", BORDER_LD, BORDER_RD);
		refresh();
	}
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
main(void)
{
	/* Set locale */
	setlocale(LC_ALL, "");

	/* Initialise ncurses */
	WINDOW *win = initscr();
	noecho();			/* Don't echo keypresses */
	cbreak();			/* Disable line buffering, speeding up key input */
	curs_set(0);			/* Hide cursor (set visibility to 0) */
	keypad(win, 1);			/* Enable keypad for current window */
	wtimeout(win, W_TIMEOUT);	/* Set window timeout to 100ms */

	/* Save screen size */
	getmaxyx(win, y, x);

	/* Add program info */
	mvwprintw(win, (y / 2) + 0, x / 3, "%s-------------------Csnake-------------------%s\n", BORDER_LU, BORDER_RU);
	mvwprintw(win, (y / 2) + 1, x / 3, "%s Csnake by Salonia Matteo v%s; Starting... %s\n", BORDER_SB,__CSNAKE_VERSION, BORDER_SB);
	mvwprintw(win, (y / 2) + 2, x / 3, "%s--------------------------------------------%s\n", BORDER_LD, BORDER_RD);
	refresh();

	/* Wait 1 second before starting */
	sleep(1);

	/* Clear screen */
	clear();
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
		/* Update screen size, if screen is updated */
		getmaxyx(win, y, x);

		/* Draw score */
		mvwprintw(win, 0, 1, "Score: %04d; Apples eaten: %04d; Current Length: %04d", score, food.count, tail_len);

		/* Get key */
		uint32_ct key = wgetch(win);

		/* Check which key was pressed */
		if ((key == 'h' || key == KEY_LEFT) && (direction != LEFT && direction != RIGHT))	/* Left */
			direction = LEFT;
		else if ((key == 'j' || key == KEY_DOWN) && (direction != DOWN && direction != UP))	/* Down */
			direction = DOWN;
		else if ((key == 'k' || key == KEY_UP) && (direction != DOWN && direction != UP))	/* Up */
			direction = UP;
		else if ((key == 'l' || key == KEY_RIGHT) && (direction != LEFT && direction != RIGHT))	/* Right */
			direction = RIGHT;
		/* Pressed key is esc */
		else if (key == '')
			pauseMenu(win);

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
