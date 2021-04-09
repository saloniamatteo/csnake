## Csnake

`csnake` is a Snake clone written in C.

+ It uses the Ncurses library to display graphics, such as:
	- The snake;
	- The snake's food;
	- User score;
	- Food (apples) eaten.

You can move the snake with Vim keys (hjkl), and the arrow keys.

Initially written on April 9, 2021. Current version can be found in `csnake.c`

## Features
`csnake` shows three values in the top-left corner:
	- Current score;
	- How many apples have been eaten;
	- Current snake length;

Every time you eat an apple, `csnake` uses a pseudo-random number generator to randomize
the new location of the food, it increases the user score by another random value
between 1 and 10, and it increases the snake's tail length by 1.

Every game starts with the food in the center of the screen, and the snake is at the
left of the screen, at the same y-coordinate of the food. (x = 0, y = screen_y / 2)

You can also pause the game by pressing <kbd>Esc</kbd>.

## Screenshots
Start Screen
![Start Screen](https://raw.githubusercontent.com/saloniamatteo/csnake/master/pics/1-start-screen.png)

Initial Position
![Initial Position](https://raw.githubusercontent.com/saloniamatteo/csnake/master/pics/2-initial-position.png)

New Food Position
![New Food Position](https://raw.githubusercontent.com/saloniamatteo/csnake/master/pics/3-new-food-pos.png)

Pause Menu
![Pause Menu](https://raw.githubusercontent.com/saloniamatteo/csnake/master/pics/4-pause-menu.png)

You Lose
![You Lose](https://raw.githubusercontent.com/saloniamatteo/csnake/master/pics/5-you-lose.png)

## Help
If you need help, you can:
- Create an issue;
- Open a pull request;
- Send me an email [(saloniamatteo@pm.me](mailto:saloniamatteo@pm.me) or [matteo@mail.saloniamatteo.top)](mailto:matteo@mail.saloniamatteo.top).
