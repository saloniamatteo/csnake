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
`csnake` shows four values in the top-left corner:
	- Difficulty;
	- Current score;
	- How many apples have been eaten;
	- Current snake length;

Every time you eat an apple, `csnake` uses a pseudo-random number generator to randomize
the new location of the food, it increases the user score by another random value
between 1 and 10, and it increases the snake's tail length by 1.

Every game starts with the food in the center of the screen, and the snake is at the
left of the screen, at the same y-coordinate of the food. (x = 0, y = screen_y / 2)

You can also pause the game by pressing <kbd>Esc</kbd>.

You can adjust the difficulty by running `./csnake -d <diff>`, where diff is between 0 and 3.
For more details about flags & difficulty values, run `./csnake -h`.

Also, if you're a developer, and/or you want to know more about the insides of `csnake`,
read Csnake's Developer Documentation in `csnake-doc.rst`.
Alternatively, you can read it by running `[sudo/doas] make install-doc`,
then `vim /usr/share/doc/csnake/csnake-doc.rst`.

## Installation
Simply run `[sudo/doas] make install`.
`csnake` will be located under `${DESTDIR}${PREFIX}/usr/bin/csnake`:
Tipically, `${DESTDIR}${PREFIX}` will be blank but, if one isn't,
for example `PREFIX="~/Documents/Games/" make install`, the installation path
will be `~/Documents/Games/usr/bin/csnake`.

Additionally, you can modify the installation directory directly,
for example, if you want to install `csnake` under `~/Documents/Games`, without
the `/usr/bin` subdirectory, run the following command instead:
`INSTALLDIR="~/Documents/Games" make install`

The documentation, instead, will be installed under
`${DESTDIR}${PREFIX}/usr/share/doc/csnake`, and can be modified
with the `DOCDIR` environment variable.

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
- Send me an email. [(saloniamatteo@pm.me](mailto:saloniamatteo@pm.me) or [matteo@mail.saloniamatteo.top)](mailto:matteo@mail.saloniamatteo.top)
