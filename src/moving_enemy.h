#ifndef MOVING_ENEMY_H
#define MOVING_ENEMY_H

#include <vector>
#include <nlohmann/json.hpp>
#include <memory>
#include <map>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>

using namespace std;
using json = nlohmann::json;

class moving_enemy {
private:
    vector<vector<short>> travel_path_points;
    unsigned short enemy_speed;
    unsigned short movable_frames; // enemy can only move on certain frames (=1 move every frame; =15 move every 15th frame)
    SDL_Rect enemy_rect;
    short current_path_point;
    short after_path_completion;
    
    short error_x;
    short error_y;
    
    enum moving_enemy_move_types_after_last_point {
        go_back = 0, teleport_back, stop
    };
    
    double current_sub_axis_val;
    char dom_axis;  // dominant axis; used in non-straight movement; represents the axis, on which the enemy will move every time
    double sub_axis_move_value;  // stores value, which will be added to current_sub_axis_val every dom_axis increase
public:
    const void var_test();

    const SDL_Rect get_enemy_rect();

    void move_enemy(unsigned int frame);

    void non_straight_line_move();

    void set_movement_type();

    void straight_line_move();

    void enemy_insert_path_point(short insert_idx, vector<short> path_point);

    void enemy_set_speed(short new_speed);

    short get_enemy_x_pos();

    short get_enemy_y_pos();

    void clear_path_points();

    void teleport_enemy(vector<short> point);

    void set_size(vector<short> new_size);

    void enemy_remove_path_point(short remove_idx);

    void enemy_set_movable_frames(short new_movable_frames);

    moving_enemy(const vector<vector<short>>&& points, const short&& speed, const short&& move_frames, const vector<short>&& size, short after_path_move_type);

};



#endif