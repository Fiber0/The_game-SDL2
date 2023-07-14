# The_game-SDL2
## a simple game made in C++ using SDL2
welcome to The_Game! In this game you play as a black square trying to get to green square to procced to the next level, however there will be red and orange rectangles getting in your way (red are stationary, orange are moving). As the game progresses "triggers" will show up. Trigger is a rectangle of any color, wich can do almost anything when you or moving enemy collides with them. Can you make it through all the 15 challenging levels? I made this game mainly to learn about the SDL2 library and to practice C++.

## controls
- **W** move up (in the menu selection or player square)
- **A** move left (in the menu selection or player square)
- **S** move down (in the menu selection or player square)
- **D** move right (in the menu selection or player square)
- **SPACE** in menu: confirm selection; in game: action button<br>
this game doesn't support mouse (yet).
## install && setup

**1. install required 3rd party libraries:**
- SDL2
- SDL2_ttf
- SDL2_image
- nlohmann-json<br>
on linux you would need to install those through your package manager.<br>
arch linux: sudo pacman -S sdl2 sdl2_ttf sdl2_image nlohmann-json

**2. clone this repository**
for HTTPS clone (linux):
git clone https://github.com/coconut41/The_game-SDL2.git <br>
for HTTPS clone (Windows):
click the green "Code" button at repository page and Download ZIP

**3. compile the code**
- specify the compilator command (g++, clang...)
- link the sdl2 libraries: -lSDL2 -lSDL2_ttf -lSDL2_image
- compile all 4 .cpp files present in src directory: mainf.cpp menus.cpp moving_enemy.cpp trigger.cpp
- after the compilation is completed (it may take several minutes) just launch the output file, if you didn't specify the -o option, it will be named a.out (on linux)<br>
for g++ the minimalistic compilation command will look like this:<br>
g++ -lSDL2 -lSDL2_ttf -lSDL2_image mainf.cpp menus.cpp moving_enemy.cpp trigger.cpp

## contributions
if you find any issues, something that could have been done better, prehaps want to add new features. feel free to create an issue or pull request. In case you would like to make levels, just create an issue and I will put together some kind of documentation on hoe to do so.



