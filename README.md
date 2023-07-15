# The_game-SDL2
## A simple game made in C++ using SDL2
Welcome to The_Game! In this game, you play as a black square trying to get to the green square to proceed to the next level. However, there will be red and orange rectangles getting in your way (red are stationary, orange are moving). As the game progresses, "triggers" will show up. Trigger is a rectangle of any color that can do almost anything when you or a moving enemy collides with it. Can you make it through all 15 challenging levels? I made this game mainly to learn about the SDL2 library and to practice C++.
![game_menu_image](https://github.com/coconut41/The_game-SDL2/blob/main/gameplay_pictures/menu_1.png?raw=true)
### for more images see [image directory](https://github.com/coconut41/The_game-SDL2/tree/main/gameplay_pictures)
## controls
- **W** move up (in the menu selection or player square)
- **A** move left (in the menu selection or player square)
- **S** move down (in the menu selection or player square)
- **D** move right (in the menu selection or player square)
- **SPACE** in menu: confirm selection; in game: action button<br>
this game doesn't support mouse (yet).
- **R** restart level
- **ESCAPE** in game: opens pause menu
- **P** activates noclip cheat (will set your death counter to 999999)
## install && setup

**1. Install required 3rd party libraries:**
- SDL2
- SDL2_ttf
- SDL2_image
- nlohmann-json<br>
On Linux you would need to install those through your package manager.<br>
Arch Linux: sudo pacman -S sdl2 sdl2_ttf sdl2_image nlohmann-json<br>
Debian Linux: sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev nlohmann-json3-dev

**2. Clone this repository**<br>
for HTTPS clone (Linux):<br>
git clone https://github.com/coconut41/The_game-SDL2.git <br>
for HTTPS clone (Windows):<br>
Click the green "Code" button at the repository page and Download ZIP

**3. Compile the code**
- Specify the compiler command (g++, clang...)
- Link the sdl2 libraries: -lSDL2 -lSDL2_ttf -lSDL2_image
- Compile all 4 .cpp files present in src directory: mainf.cpp menus.cpp moving_enemy.cpp trigger.cpp
- After the compilation is complete (it may take several minutes) just launch the output file. If you didn't specify the -o option, it will be named a.out (on linux)<br>
For g++, the minimalistic compilation command will look like this:<br>
g++ -lSDL2 -lSDL2_ttf -lSDL2_image mainf.cpp menus.cpp moving_enemy.cpp trigger.cpp

## contributions
If you find any issues or something that could have been done better, prehaps want to add new features. Feel free to create an issue or pull request. In case you would like to make levels, just create an issue, and I will put together some kind of documentation on how to do so.



