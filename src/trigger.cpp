#include "trigger.h"


SDL_Rect trigger::get_trigger_rect() { return trigger_rect; }

vector <short> trigger::get_trigger_color() { return color_rgba; }

void trigger::evaluate_functions(char triggering_object, string triggerer_id, const json& triggers_data, int frame_counter, player_data & player_dat, map <string, moving_enemy>& moving_enemies_map, map <string, trigger>& triggers_map) {

    if (triggering_object == 'p') {  // if player triggered the trigger

        for (json trigger_functions : triggers_data[trigger_arr_position]["on_player_trigger"]) {  // iterate through specified functions
            string function_name = trigger_functions["function_id"];
            function_handler(function_name, "", trigger_functions["arguments"],frame_counter,player_dat ,moving_enemies_map, triggers_map);
        }
    }
        
    else if (triggering_object == 'e') {  // if moving_enemy triggered the trigger
            
        for (json trigger_functions : triggers_data[trigger_arr_position]["on_enemy_trigger"]) {
            string function_name = trigger_functions["function_id"];
            function_handler(function_name, triggerer_id, trigger_functions["arguments"],frame_counter, player_dat, moving_enemies_map, triggers_map);
        }   
    }
        
    else if (triggering_object == 'a') {  // if action_button triggered the trigger
            
        for (json trigger_functions : triggers_data[trigger_arr_position]["on_action_trigger"]) {  // iterate through specified functions
            string function_name = trigger_functions["function_id"];
            function_handler(function_name, "", trigger_functions["arguments"],frame_counter,player_dat ,moving_enemies_map, triggers_map);
        }
    } 
}


void  trigger::on_trigger(char triggering_object, string triggerer_id, const json& triggers_data, int frame_counter, player_data & player_dat, map <string, moving_enemy>& moving_enemies_map, map <string, trigger>& triggers_map) {
    if (!exists) {

        if ((re_enable_rate != -1) && ((frame_counter - frames_elapsed_on_disable) >= re_enable_rate)) {
            exists = 1;
            evaluate_functions(triggering_object, triggerer_id, triggers_data, frame_counter, player_dat, moving_enemies_map, triggers_map);
        }
    }
    else {
        evaluate_functions(triggering_object, triggerer_id, triggers_data, frame_counter, player_dat, moving_enemies_map, triggers_map);
    }
}


void trigger::function_handler(string function_id, string triggerer_id, json arguments,const unsigned int & frame_counter,player_data & player_dat ,map <string, moving_enemy>& moving_enemies_map, map <string, trigger> &triggers_map) {
    vector <short> point;  // used for vector parameters (point position)
        
    if (function_id.rfind("yourself", 0) == 0) {  // if function_name starts with "yourself"

        if (function_id == "yourself_disable") {
            frames_elapsed_on_disable = frame_counter;
            exists = 0;
        }

        else if (function_id == "yourself_resize") {
                trigger_rect.w = pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map,triggerer_id);
                trigger_rect.h = pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map,triggerer_id);
        }  
    }
        
    else if (function_id.rfind("moving_enemy", 0) == 0) {  // if function_name starts with "moving_enemy"
        
        if (arguments[0] == "triggerer_id") {
            arguments[0] = triggerer_id;
        }

        if (function_id == "moving_enemy_insert_path_point") {
            point = {pre_eval(arguments[2][0], player_dat, moving_enemies_map, triggers_map,triggerer_id),  pre_eval(arguments[2][1], player_dat, moving_enemies_map, triggers_map,triggerer_id)};
            moving_enemies_map.at(arguments[0]).enemy_insert_path_point(pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map,triggerer_id), point);
        }
        else if (function_id == "moving_enemy_clear_path_points") { moving_enemies_map.at(arguments[0]).clear_path_points(); }

        else if (function_id == "moving_enemy_set_speed") { moving_enemies_map.at(arguments[0]).enemy_set_speed(pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map,triggerer_id)); }

        else if (function_id == "moving_enemy_remove_path_point") { moving_enemies_map.at(arguments[0]).enemy_remove_path_point(pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map,triggerer_id)); }

        else if (function_id == "moving_enemy_set_movable_frames") { moving_enemies_map.at(arguments[0]).enemy_set_movable_frames(pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map,triggerer_id)); }

        else if (function_id == "moving_enemy_set_movement_type") { moving_enemies_map.at(arguments[0]).set_movement_type(); }

        else if (function_id == "moving_enemy_teleport") {
            point = {pre_eval(arguments[1][0], player_dat, moving_enemies_map, triggers_map,triggerer_id),  pre_eval(arguments[1][1], player_dat, moving_enemies_map, triggers_map ,triggerer_id)};
            moving_enemies_map.at(arguments[0]).teleport_enemy(point);
        }

        else if (function_id == "moving_enemy_set_size") { moving_enemies_map.at(arguments[0]).set_size(arguments[1]); }

    }
    
    else if (function_id.rfind("trigger", 0) == 0) {  // if function_name starts with "trigger"
        if (function_id == "trigger_relocate") {
            point = {pre_eval(arguments[1][0], player_dat, moving_enemies_map, triggers_map,triggerer_id),  pre_eval(arguments[1][1], player_dat, moving_enemies_map, triggers_map,triggerer_id)};
            triggers_map.at(arguments[0]).relocate_trigger(point);
        }
        
        else if (function_id == "trigger_set_state") {
            triggers_map.at(arguments[0]).set_trigger_state(pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map, triggerer_id), frame_counter);
        }

        else if (function_id == "trigger_set_size") {
            point= { pre_eval(arguments[1][0], player_dat, moving_enemies_map, triggers_map ,triggerer_id),  pre_eval(arguments[1][1], player_dat, moving_enemies_map, triggers_map,triggerer_id) };
            triggers_map.at(arguments[0]).set_trigger_size(point);
        }

        else if (function_id == "trigger_set_re_enable_rate") {
            triggers_map.at(arguments[0]).set_trigger_reenable_rate(pre_eval(arguments[1], player_dat, moving_enemies_map, triggers_map, triggerer_id), frame_counter);
        }
        else if (function_id == "trigger_set_color") {
            vector <short> color = arguments[1];
            color[0] = pre_eval(color[0], player_dat, moving_enemies_map, triggers_map, triggerer_id);
            color[1] = pre_eval(color[1], player_dat, moving_enemies_map, triggers_map, triggerer_id);
            color[2] = pre_eval(color[2], player_dat, moving_enemies_map, triggers_map, triggerer_id);
            color[3] = pre_eval(color[3], player_dat, moving_enemies_map, triggers_map, triggerer_id);
            
            triggers_map.at(arguments[0]).set_color(color);
        }
    }
    else if (function_id.rfind("player", 0) == 0) {
        
        if (function_id == "player_teleport") {
            point = {pre_eval(arguments[0][0], player_dat, moving_enemies_map, triggers_map,triggerer_id),  pre_eval(arguments[0][1], player_dat, moving_enemies_map, triggers_map,triggerer_id)};
            player_dat.player.x = point[0];
            player_dat.player.y = point[1];

        }
        
        else if (function_id == "player_resize") {
            point = {pre_eval(arguments[0][0], player_dat, moving_enemies_map, triggers_map,triggerer_id),  pre_eval(arguments[0][1], player_dat, moving_enemies_map, triggers_map,triggerer_id)};
            player_dat.player.w = point[0];
            player_dat.player.h = point[1];

        }
        
        else if (function_id == "player_set_god_mode") {
            if (pre_eval(arguments[0], player_dat, moving_enemies_map, triggers_map,triggerer_id) != 0) {
                player_dat.god_mode = 1;
            }
            else { player_dat.god_mode = 0; }

        }
        
        else if (function_id == "player_set_speed_per_second") {
            player_dat.speed_per_second = pre_eval(arguments[0], player_dat, moving_enemies_map, triggers_map, triggerer_id);

        }
        
    }

}


short trigger::pre_eval(json expression, player_data & player_dat, map <string, moving_enemy>& moving_enemies_map, map <string, trigger>& triggers_map,string triggerer_id) {
    
    if ((string)(expression.type_name()) == "string") {
        
        string modified_expression = expression;
        modified_expression.erase(remove(modified_expression.begin(), modified_expression.end(), ' '), modified_expression.end());  // remove spaces
        // evaluate variables
        int variable_start_idx;
        string txt_variable;
        string specified_enemy;
        string specified_trigger;
        
        for (int i = 0; i < modified_expression.size(); i++) {
            variable_start_idx = modified_expression.find('<');
            if (variable_start_idx != -1) {
                txt_variable = modified_expression.substr(variable_start_idx + 1, modified_expression.find('>') - (variable_start_idx + 1));
                modified_expression.erase(variable_start_idx, txt_variable.size() + 2);
                
                if (txt_variable == "player_x_pos") {
                    txt_variable = to_string(player_dat.player.x);
                }
                else if (txt_variable == "player_y_pos") {
                    txt_variable = to_string(player_dat.player.y);
                }
                else if (txt_variable.rfind("enemy_x_pos", 0) == 0) {
                    specified_enemy = txt_variable.substr(12, txt_variable.size() - 13);
                    if (specified_enemy == "triggerer_id") {
                        specified_enemy = triggerer_id;
                    }
                    txt_variable = to_string(moving_enemies_map.at(specified_enemy).get_enemy_x_pos());
                }
                else if (txt_variable.rfind("enemy_y_pos", 0) == 0) {
                    specified_enemy = txt_variable.substr(12, txt_variable.size() - 13);
                    if (specified_enemy == "triggerer_id") {
                        specified_enemy = triggerer_id;
                    }
                    txt_variable = to_string(moving_enemies_map.at(specified_enemy).get_enemy_y_pos());
                }
                else if (txt_variable.rfind("trigger_y_pos", 0) == 0) {
                    specified_trigger = txt_variable.substr(14, txt_variable.size() - 15);
                    if (specified_trigger == "triggerer_id") {
                        specified_trigger = triggerer_id;
                    }
                    txt_variable = to_string(triggers_map.at(specified_trigger).get_trigger_y_pos());
                }
                else if (txt_variable.rfind("trigger_x_pos", 0) == 0) {
                    specified_trigger = txt_variable.substr(14, txt_variable.size() - 15);
                    if (specified_trigger == "triggerer_id") {
                        specified_trigger = triggerer_id;
                    }
                    txt_variable = to_string(triggers_map.at(specified_trigger).get_trigger_x_pos());
                }

                modified_expression.insert(variable_start_idx, txt_variable);

            }
            else {
                break;
            }    
        }
        string func_res = eval(modified_expression);
        return round(stoi(func_res));
        
    }
    else {
        return expression;
    }
}

void trigger::relocate_trigger(vector <short> new_pos) {
    trigger_rect.x = new_pos[0];
    trigger_rect.y = new_pos[1];
}

void trigger::set_color(vector<short> color) {
    color_rgba[0] = color[0];
    color_rgba[1] = color[1];
    color_rgba[2] = color[2];
    color_rgba[3] = color[3];

}

void trigger::set_trigger_state(int state, const unsigned int & frame_counter) {
    
    frames_elapsed_on_disable = frame_counter;
    exists = state ? 1 : 0;
}

void trigger::set_trigger_reenable_rate(int new_reenable_rate, const unsigned int & frame_counter) {
    if (re_enable_rate == -1) {
        frames_elapsed_on_disable = frame_counter;
    }
    re_enable_rate = new_reenable_rate;
}

short trigger::get_trigger_x_pos() {
    return trigger_rect.x;
}

short trigger::get_trigger_y_pos() {
    return trigger_rect.y;
}


void trigger::set_trigger_size(vector <short> size) {
    trigger_rect.w = size[0];
    trigger_rect.h = size[1];
}

trigger::trigger(bool exist, const int trigger_array_position, const vector <short>&& trigger_pos, const vector <short>&& trigger_size, const vector <short> color, int reenable_rate):
    trigger_rect({ trigger_pos[0], trigger_pos[1], trigger_size[0], trigger_size[1] }),
    color_rgba(color),
    trigger_arr_position(trigger_array_position),
    re_enable_rate(reenable_rate),
    exists(exist),
    frames_elapsed_on_disable(0)

{}