#ifndef TRIGGER_H
#define TRIGGER_H

#include <vector>
#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>
#include "mainf.h"

using namespace std;
using json = nlohmann::json;

struct player_data;

class trigger {
private:
    SDL_Rect trigger_rect;
    vector <short> color_rgba;
    const short trigger_arr_position;  // represents trigger's position in json "triggers" array
    bool exists;
    int re_enable_rate;  // after how much frames should trigger re-enable
    int frames_elapsed_on_disable;  // stores number of frame, at which the trigger was disabled
public:
    void var_test();

    SDL_Rect get_trigger_rect();
    
    vector <short> get_trigger_color();

    void evaluate_functions(char triggering_object, string triggerer_id, const json& triggers_data, int frame_counter, player_data& player_dat, map <string, moving_enemy>& moving_enemies_map, map <string, trigger>& triggers_map);

    void on_trigger(char triggering_object, string triggerer_id, const json& triggers_data, int frame_counter, player_data& player_dat, map <string, moving_enemy>& moving_enemies_map, map <string, trigger>& triggers_map);

    void function_handler(string function_id, string triggerer_id, json arguments, const unsigned int& frame_counter, player_data& player_dat, map <string, moving_enemy>& moving_enemies_map, map <string, trigger>& triggers_map);

    short pre_eval(json expression, player_data& player_dat, map <string, moving_enemy>& moving_enemies_map, map <string, trigger>& triggers_map, string triggerer_id);

    void relocate_trigger(vector <short> new_pos);

    void set_color(vector<short> color);

    void set_trigger_state(int state, const unsigned int& frame_counter);

    void set_trigger_reenable_rate(int new_reenable_rate, const unsigned int& frame_counter);

    short get_trigger_x_pos();

    short get_trigger_y_pos();

    void set_trigger_size(vector <short> size);

    trigger(bool exist, const int trigger_array_position, const vector <short>&& trigger_pos, const vector <short>&& trigger_size, const vector <short> color, int reenable_rate);

};

#endif