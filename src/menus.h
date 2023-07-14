#ifndef MENUS_H
#define MENUS_H
 
#include <vector>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include "mainf.h"

using namespace std;
using json = nlohmann::json;

class Game_core;

class menus {

public:
    void all_levels_completed_screen();
    
    void pause_menu();

    short scan_for_levels_and_report();

    void level_rescan();

    void get_level_name_and_difficulty(short level_idx, string& level_name, short& level_difficulty);

    void update_level_name_texture(string selected_level_name, SDL_Texture*& level_name_texture, SDL_Rect& level_name_text_rect);

    void update_level_number_texture(short new_level_idx, SDL_Texture*& level_number_texture, SDL_Rect& level_number_rect);
    
    void level_select();

    void menu();

    void single_level_completed_screen();

    void level_completed_screen();

    
};



#endif
