## Csnake

`csnake` is a Snake clone written in C.

+ It uses the Ncurses library to display graphics, such as:
	- The snake;
	- The snake's food;
	- User score;
	- Food (apples) eaten.

You can move the snake with Vim keys (hjkl), and the arrow keys.

Initially written on April 9, 2021.
Current version can be found in `csnake.c`, or by running `make version`

Support this project: https://saloniamatteo.top/donate.html

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
left of the screen, at the same y-coordinate of the food. (snake_x = 0, snake_y = screen_y / 2)

You can also pause the game by pressing <kbd>Esc</kbd>, or quit it by pressing <kbd>q</kbd>
(You will be prompted for confirmation)

You can adjust the difficulty by running `./csnake -d <diff>`, where diff is between 0 and 3.
For more details about flags & difficulty values, run `./csnake -h`.

Also, if you're a developer, and/or you want to know more about the insides of `csnake`,
read Csnake's Developer Documentation in `csnake-doc.rst`.
Alternatively, you can read it by running `[sudo/doas] make install-doc`,
then `vim /usr/share/doc/csnake/csnake-doc.rst`.

## Command-line arguments
`csnake` accepts the following parameters:

| Flag | Long Flag      | Parameter               | Description                      |
|------|----------------|-------------------------|----------------------------------|
| `-b` | `--borders`    | (none)                  | Draw Screen Borders              |
| `-d` | `--difficulty` | 0-3, 'e', 'm', 'h', 'x' | Set difficulty (see [1])         |
| `-h` | `--help`       | (none)                  | Print help & usage               |
| `-s` | `--save`       | savefile                | Save scores to "savefile"        |
| `-x` | `--scr-x`      | x_val                   | Modify maximum screen x to x_val |
| `-y` | `--scr-y`      | y_val                   | Modify maximum screen y to y_val |

[1]: The difficulty values correspond to:

| Diff value | Actual Difficulty |
|------------|-------------------|
| 0, e, E    | Easy              |
| 1, m, M    | Medium            |
| 2, h, H    | Hard              |
| 3, x, X    | Extreme           |

So, for example, if I want to play on "Extreme" difficulty, I'll run `csnake -dx`,
or `csnake -d3`, `csnake -d X`, `csnake -d=x`, etc. All variations of this are valid.

## Saving scores
Optionally, users can save scores to a file, which they have access to.
If the user is trying to overwrite the `csnake` executable, it will stop
before doing anything dangerous (example: `./csnake -s csnake`).

`csnake` is also smart and recognises if a file is not in its current directory,
but has the same name (example: `./csnake -s /tmp/csnake)`

For example, the command `./csnake -s csnake.sav`
(or `csnake -s csnake.sav`, if it was installed using `make install`)
is perfectly valid, and will save scores to `csnake.sav`, in the current directory.

The resulting file will always be a plaintext file, no matter the format
(png, zip, tiff, 7z, etc.)

## Installation
`csnake` uses GNU AutoTools, to increase portability.

Normally, users should only need to run `./configure && make && make install`,
running the last command `make install` as root, or by prepending
`doas` (or `sudo`) before `make` (`doas make install`).

On occasions where this is somehow not working, run `autoreconf --install`,
to re-generate the configuration script, Makefile, etc.

It's also possible to install documentation to any directory.
For example, if I want to install documentation to the default
location (/usr/share/doc/csnake), I would run the following:

```bash
# Replace "doas" with "sudo",
# if you have the latter
./configure && doas make install-data
```

NOTE: this only installs the documentation!
To install both `csnake` and its documentation, run:

```bash
# Replace "doas" with "sudo",
# if you have the latter
./configure && make && \
doas make install && doas make install-data
```

To uninstall documentation, simply run:

```
# Replace "doas" with "sudo",
# if you have the latter
doas make uninstall-doc
```

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
