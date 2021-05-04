.. SPDX-License-Identifier: GPL-2.0

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
- Main Function
        - Ncurses initialisation
        - Command-line argument options
        - Pre-game section (show program info)
        - Game section
                - Snake initialisation
                - Food creation
                - Direction Key handling (hjkl or arrow keys)
                - Food handling
                - Snake movement handling

Overrall, the code is around 500 lines of code, even less if we talk about SLOC.

Defines
-----------
The following are "#define"d in Csnake:

==================    ===========================      =============
#define Name          Description                      Default Value
==================    ===========================      =============
__CSNAKE_MAJ_VER      Csnake Major Version (Str)       "1"
__CSNAKE_MAJ_NVER     Csnake Major Version (Num)       1
__CSNAKE_MIN_VER      Csnake Minor Version (Str)       "1"
__CSNAKE_MIN_NVER     Csnake Minor Version (Num)       1
__CSNAKE_VERSION      Csnake Version (MAJ + MIN)       "1.1"

SNAKESIZE             Maximum Snake size               100
TAIL_SIZE             Default Tail Length              3
FOOD_CHAR             Which Character to use           "@"
                      to display Food
SNAKECHAR             Which Character to use           ACS_CKBOARD
                      to display Snake

DIFF_EASY             Window Timeout in ms for         90
                      Easy Difficulty
DIFF_MEDI             Window Timeout in ms for         65
                      Medium Difficulty
DIFF_HARD             Window Timeout in ms for         50
                      Hard Difficulty
DIFF_XTRM             Window Timeout in ms for         35
                      Extreme Difficulty

RAND(min, max)        Macro to generate random         ((rand() % (max - min + 1)) + min)
                      number between min and max 
==================    ===========================      =============

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

=============   ================           ===========================            ====================================
Function Type   Function Name              Function Description                   Arguments
=============   ================           ===========================            ====================================
WINDOW *        newsubwin                  Create a new sub-window                int height, int width,
                                           with default borders,                  int startx, int starty, char \*title
                                           and return it.
void            diffToWinTimeout           Convert difficulty to                  uint8_ct diff
                                           ncurses window timeout.
char *          diffStr                    Convert diffuculty to                  void
                                           human-readable string,
                                           and return it.
int             setDiff                    Set difficulty from                    char \*arg
                                           string, and return the
                                           corresponding integer.
void            printHelp                  Print help and usage.                  char \*progname
void            newFood                    Generate new food                      void
                                           coordinates.
void            endSnk                     Exit Csnake cleanly,                   WINDOW \*win
                                           showing total score,
                                           total apples eaten, and
                                           showing difficulty.
uint32_ct       randScore                  Generate a new random                  void
                                           integer between 1 and 10,
                                           and return the value.
                                           (used for score generation)
void            pauseMenu                  Create a new pause menu                void
                                           window, to pause the game.
void            scrUpd                     Update the screen, drawing             WINDOW \*win
                                           the food, and the snake,
                                           on the screen.
=============   ================           ===========================            ====================================

Global Variables
----------------
Csnake has the following global variables:

==========      ===================      ===========================    =============
Type            Variable Name            Description                    Default Value
==========      ===================      ===========================    =============
uint32_ct       x                        Screen width                   0
uint32_ct       y                        Screen height                  0
uint32_ct       max_x                    Max Screen width               0
uint32_ct       max_y                    Max Screen height              0
uint32_ct       nx                       Next x coordinate              0
uint32_ct       ny                       Next y coordinate              0
uint32_ct       tail_len                 Tail Length                    TAIL_SIZE
DIRECTION       direction                Current snake direction        RIGHT
COORD           snake[SNAKESIZE]         Snake Object                   {0}
FOOD            food                     Food Object                    {0}
uint32_ct       score                    Player Score                   0
uint8_ct        diff                     Difficulty                     1
uint8_ct        win_timeout              Window Timeout                 65
==========      ===================      ===========================    =============
