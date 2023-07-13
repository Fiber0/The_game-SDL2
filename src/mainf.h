#ifndef MAINF_H
#define MAINF_H

#include <vector>
#include <iostream>  // comment out on production
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <memory>
#include <map>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include "menus.h"
#include "moving_enemy.h"
#include "trigger.h"

using namespace std;
using json = nlohmann::json;


struct player_data {
public:
    SDL_Rect player;
    bool god_mode = 0;
    int speed_per_second = 300; // defines player speed (100 is the minimum)
};


string eval(string expr);

class moving_enemy;
class trigger;
class menus;

class Game_core {
private:
    const int RESOLUTION_X = 800;
    const int RESOLUTION_Y = 600;
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Rect window_border{ 0, 0, RESOLUTION_X, RESOLUTION_Y };
    
    vector <string> available_levels_vect;
    vector <SDL_Rect> enemies;
    
    json data;  // current level json data
    json settings_data;  
    
    vector <int> previous_tick_player_pos = { -1,-1 }; // used for movement detecting

    // each entry consists of object id pointing to object reference
    map <string, trigger> triggers_map;
    map <string, moving_enemy> moving_enemies_map;
    
    int current_level_idx = -1;
    unsigned int frame_counter = 0;  // stores ammount of frames passed ! will overflow at about 11000th hour of gameplay ! (shouldn't really cause anything)
    double delta_motion;
    
    bool trigger_and_enemy_collisions = 0;  // if true, check if moving_enemies collide with triggers
    bool rebuild_level = 0;  // reduild entire level from json file  (should be optimized)
    bool player_died = 0;
    unsigned int death_counter = 0;

    SDL_Rect starting_pad;
    SDL_Rect starting_pad_border; // green border around starting pad
    SDL_Rect goal_pad;
    SDL_Rect goal_pad_border; // green border around goal pad

    TTF_Font* Operator_font;
    TTF_Font* Arcade_classic_font;
    SDL_Thread* thread = nullptr;
    
    bool action_button_enabled = 0;  // if action button was enabled in level file
    string action_trigger_id;  // id of action button
    player_data player_dat;

    enum game_modes { classic = 0, single_level, observation };
    short game_mode = 0;              
    
    menus *MENU;

public:
    
    void clean_exit();

    void increment_frame_counter();

    string get_random_tip();

    bool tolerative_collision_check(SDL_Rect player, const SDL_Rect* rect2);

    void render_all_objects_and_check_for_collisions();

    void present_renderer();

    void handle_event(SDL_Event& event);

    void handle_movement(const Uint8* keyboard_state);

    void keep_player_in_window();

    void on_single_level_mode_level_completion();

    void on_classic_mode_level_completion();

    void on_death();

    void on_death_animation();

    void load_data_from_settings();

    void look_for_settings_file();

    void save_death_counter_to_settings_file(int deaths);

    void save_level_to_settings_file(string level);

    void inicialize_level_related_variables();

    void reset_level();

    void inicialize_levels_vec(string dir_path);

    void on_construction();
    
    friend menus;
};

extern Game_core game;
#endif