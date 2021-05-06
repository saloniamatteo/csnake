.. SPDX-License-Identifier: GPL-3.0

====================
Csnake Documentation
====================

Introduction
-------------
Welcome to **Csnake's Development Documentation!**

In this document, you will find: 

- License
- Code Structure
- Defines
        - #define name
        - Description
        - Default Value
- Custom Types
        - Type Name
        - Corresponds To
- Custom Objects
        - Object Name
        - Object Description
        - Items contained in Object
- Functions
        - Function Type
        - Function Name
        - Function Description
        - Arguments
- Global Variables
        - Type
        - Variable Name
        - Description
        - Default Value

License
--------
This project is licensed under the GNU General Public License version 3,
abbreviated GNU GPLv3.

Project originally made by Matteo Salonia <saloniamatteo@pm.me>

Code Structure
------------------
csnake.c is structured in the following way:

- Comment describing what the project is, who made it, etc.
- "#include"s
- "#define"s
- Custom types
        - typedefs
        - enums
        - structs
- Global Variables
- Function Prototypes
- Function Declarations
- Main Function (See below)

Overrall, the code is around 400 Single Lines Of Code (SLOC).

Defines
-----------
The following are "#define"d in Csnake:

==================    ================================  =====================================
#define Name          Description                       Default Value
==================    ================================  =====================================
_FORTIFY_SOURCE       Fortify source                    2
_POSIX_C_SOURCE       Which POSIX C standard to follow  200809L

__CSNAKE_MAJ_VER      Csnake Major Version (Str)        "1"
__CSNAKE_MAJ_NVER     Csnake Major Version (Num)        1
__CSNAKE_MIN_VER      Csnake Minor Version (Str)        "3"
__CSNAKE_MIN_NVER     Csnake Minor Version (Num)        3
__CSNAKE_VERSION      Csnake Version (MAJ + MIN)        "1.2" (MAJ "." MIN)

SNAKESIZE             Maximum Snake size                100
TAIL_SIZE             Default Tail Length               3
FOOD_CHAR             Which Character to use            "@"
                      to display Food
SNAKECHAR             Which Character to use            ACS_CKBOARD
                      to display Snake

DIFF_EASY             Window Timeout in ms for          90
                      Easy Difficulty
DIFF_MEDI             Window Timeout in ms for          65
                      Medium Difficulty
DIFF_HARD             Window Timeout in ms for          50
                      Hard Difficulty
DIFF_XTRM             Window Timeout in ms for          35
                      Extreme Difficulty

RAND(min, max)        Macro to generate random          ((rand() % (max - min + 1)) + min)
                      number between min and max 
==================    ================================  =====================================

Custom Types
-------------
Csnake has the following local custom types:
(denoted with the "_ct" suffix)

============    ===================
Type Name       Corresponds to
============    ===================
uint32_ct       unsigned int
uint8_ct        short unsigned int
============    ===================

Custom Objects
--------------
Csnake has the following local custom objects:

===========     ==================      =========================
Object Name     Object Description      Items contained in Object
===========     ==================      =========================
COORD           Coordinate object       int x, int y
FOOD            Food object             int x, int y, int count
===========     ==================      =========================

Functions
---------
Csnake has the following functions:

=============   ================                ===============================        ====================================
Function Type   Function Name                   Function Description                   Arguments
=============   ================                ===============================        ====================================
WINDOW *        newsubwin                       Create a new sub-window                int height, int width,
                                                with default borders,                  int startx, int starty, char \*title
                                                and return it.
void            diffToWinTimeout                Convert difficulty to                  uint8_ct diff
                                                ncurses window timeout.
char *          diffStr                         Convert diffuculty to                  void
                                                human-readable string,
                                                and return it.
int             setDiff                         Set difficulty from                    char \*arg
                                                string, and return the
                                                corresponding integer.
void            printHelp                       Print help and usage.                  char \*progname
void            newFood                         Generate new food                      void
                                                coordinates.
int             checkFile                       Check if we can write to file,         char \*file, char \*name
                                                and return -1 if we are trying
                                                to write to ourselves, 0 if
                                                the file cannot be opened, 1
                                                if it can be opened.
void            saveScore                       Save scores to file, according         void
                                                to ``savefile`` variable.
void            endSnk                          Exit Csnake cleanly,                   WINDOW \*win
                                                showing total score,
                                                total apples eaten, and
                                                showing difficulty.
uint32_ct       randScore                       Generate a new random                  void
                                                integer between 1 and 10,
                                                and return the value. (Used
                                                for random score generation)
void            pauseMenu                       Create a new pause menu                void
                                                window, to pause the game.
void            exitConfirm                     Check if user really wants             WINDOW \*win
                                                to exit game.
void            scrUpd                          Update the screen, drawing             WINDOW \*win
                                                the food, and the snake,
                                                on the screen.
=============   ================                ===============================        ====================================

Global Variables
----------------
Csnake has the following global variables:

==========      ===================      ===================================    =============
Type            Variable Name            Description                            Default Value
==========      ===================      ===================================    =============
uint32_ct       x                        Screen width                           0
uint32_ct       y                        Screen height                          0
uint32_ct       max_x                    Max Screen width                       0
uint32_ct       max_y                    Max Screen height                      0
uint32_ct       nx                       Next x coordinate                      0
uint32_ct       ny                       Next y coordinate                      0
uint32_ct       tail_len                 Tail Length                            TAIL_SIZE
DIRECTION       direction                Current snake direction                RIGHT
COORD           snake[SNAKESIZE]         Snake Object                           {0}
FOOD            food                     Food Object                            {0}
uint32_ct       score                    Player Score                           0
uint8_ct        diff                     Difficulty                             1
uint8_ct        win_timeout              Window Timeout                         65
uint32_ct       usr_x                    User-defined x                         0
uint32_ct       usr_y                    User-defined y                         0
bool            borders                  Are borders enabled?                   false
char            filename[100]		 Default file used to save scores       {0}
bool            savescore		 Save scores?                           false
==========      ===================      ===================================    =============

Program Operation
------------------
The actual core of the program is located in the ``main()`` function.
The ``main()`` function is structured like this:

- Locale Initialisation (C locale)
- Command-line argument handling
        - Loop to parse each argument
- Ncurses Initialisation
        - Window Initialisation
        - Screen coordinate saving
        - Coordinate Adjusting
        - Various option finalisations
- Show program info
- Game Section
        - Snake initialisation
        - Food creation
        - (Optional) Show Borders
        - Direction Key handling (hjkl or arrow keys)
        - Food handling
        - Snake movement handling

Firstly, the locale is initialised with the ``setlocale()`` function,
located in the ``locale.h`` header.

Then, we have to parse command-line options/arguments, using ``getopt_long``,
which lets us parse both short and long options.

Following that, we initialise the ncurses window, and we check if the user
coordinates have been passed: if they aren't 0, that means the user has
provided custom coordinates, so we set ``max_x`` and/or ``max_y`` to the custom
coordinates, which correspond to the width (x) and height (y) of the screen.

We then finalise various Ncurses options, such as:

- Do not echo keypresses (``noecho()``)
- Disable line buffering (``cbreak()``)
- Don't display cursor (``curs_set(0)``)
- Enable keypad, allowing us to use cursor keys (``keypad(win, 1)``)
- Set window timeout, according to difficulty chosen by user (``wtimeout(win, win_timeout)``)

Next, we briefly show ``cnake``'s info, showing the current version,
and who made the program (Matteo Salonia).

Finally, we enter the **game section**, where we have to actually compute the game logic:

- Initialise snake position, setting the default position
- Set new food coordinates (default coordinates are screen center)
- Endless loop
        - Always draw score & difficulty on top left corner
        - Get key press from user
                - Handle key press
                        - Checking if a direction key (hjkl or arrow keys) was pressed
                                - Handle the snake movement
                        - Check if ESC (written as ``^[``) was pressed
                                - Pause the game
                        - Check if q was pressed
                                - Check if the user actually wants to exit or not.
                                  Obviously, the user is not forced to press "q" to quit, in fact they can just
                                  press ``CTRL+C`` to exit, but their final scores won't be shown.
                                  (Note: this might change in a future version, where an option may be passed
                                  to save scores to a file)
        - Set the snake's next coordinates, which always correspond to snake's head
        - Handle direction, changing the next coordinates
        - Check if snake has eaten food
                - Update food count
                - Set new tail position, increasing it by 1
                - Add a random value between 1 and 10 to user score
        - If the snake has not eaten food, redraw the snake
                - Check if the snake has collided into itself
                - Check if the snake has hit screen edges
        - Redraw Screen
- If the endless loop somehow fails, end game cleanly
