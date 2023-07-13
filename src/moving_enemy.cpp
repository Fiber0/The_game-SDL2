#include "moving_enemy.h"

inline int abs_int(int x) { return (x ^ (x >> 31)) - (x >> 31); } // get absolute value of integer

const SDL_Rect moving_enemy::get_enemy_rect() { return enemy_rect; }

void moving_enemy::move_enemy(unsigned int frame) {

    if ((frame % movable_frames == 0) && (enemy_speed != 0)) {
        // error == delta
        error_x = travel_path_points[current_path_point][0] - enemy_rect.x; // calculate the x and y axis distances required to get to the next point
        error_y = travel_path_points[current_path_point][1] - enemy_rect.y;

        if (!error_x || !error_y) {

            straight_line_move();  // called when an enemy can get to path_point only on 1 axis (dimension)
        }
        else {

            non_straight_line_move();  // called when an enemy must get to path_point on 2 axis (dimensions)
        }
    }
}


void moving_enemy::non_straight_line_move() {

    // check if the shortest path to next point is smaller than the distance enemy can move (so it wont move past the point)
    if (((max(abs_int(error_x), abs_int(error_y))) - enemy_speed) < 0) {
        enemy_rect.x = travel_path_points[current_path_point][0];
        enemy_rect.y = travel_path_points[current_path_point][1];
        return;
    }

    for (int i = 0; i < enemy_speed; i++) {

        if (dom_axis == 'x') {

            enemy_rect.x += error_x < 0 ? -1 : 1; // check if point is on the left side of enemy and apply operations accordingly
            if (current_sub_axis_val >= 1) {  // if the move value of sub_dimension exceeded 1

                enemy_rect.y += error_y < 0 ? -1 : 1;  // add or subtract 1 accordingly to path_point's position
                current_sub_axis_val--;
            }
        }

        else {

            enemy_rect.y += error_y < 0 ? -1 : 1;
            if (current_sub_axis_val >= 1) {

                enemy_rect.x += error_x < 0 ? -1 : 1;
                current_sub_axis_val--;
            }
        }
        current_sub_axis_val += sub_axis_move_value;
    }
}


void moving_enemy::set_movement_type() {
    
    if (current_path_point == (travel_path_points.size() - 1)) {  // if enemy is at the last path_point
        switch (after_path_completion) {
            case (go_back):
                current_path_point = 0;
                break;
            case (teleport_back):
                enemy_rect.x = travel_path_points[0][0];
                enemy_rect.y = travel_path_points[0][1];
                current_path_point = 0;
                break;
            case (stop):
                
                break;
        }
    }
    else {  // get the next path_point
        current_path_point += 1;
    }
    current_sub_axis_val = 0;
    // in case the next path_point will require diagonal movement

    error_x = travel_path_points[current_path_point][0] - enemy_rect.x; // calculate the x and y axis distances required to get to the next point
    error_y = travel_path_points[current_path_point][1] - enemy_rect.y;

    if (abs_int(error_x) > abs_int(error_y)) { // if main dimension is x
        dom_axis = 'x';
        sub_axis_move_value = (double)error_y / error_x;
    }

    else {  // if main dimension is y
        dom_axis = 'y';
        sub_axis_move_value = (double)error_x / error_y;
    }

    if (sub_axis_move_value < 0) { sub_axis_move_value *= -1; }  // sub_axis_move_value must never be negative (in case it is, convert it to positive)
    
}


void moving_enemy::straight_line_move() {

    if (!error_x && !error_y) { // if we met our target point
        set_movement_type();
    }

    if ((abs_int(error_x) - enemy_speed) > 0) {  // check if the distance to next point is bigger than the distance enemy can move (so it wont move past the point)
        enemy_rect.x = error_x < 0 ? enemy_rect.x - enemy_speed : enemy_rect.x + enemy_speed; // check if point is on the left side of enemy and apply operations accordingly      
    }

    else {
        enemy_rect.x = travel_path_points[current_path_point][0];
    }  // if distance to the point is smaller, just set enemy_position = to target point 

    if ((abs_int(error_y) - enemy_speed) > 0) {
        enemy_rect.y = error_y < 0 ? enemy_rect.y - enemy_speed : enemy_rect.y + enemy_speed;
    }

    else {
        enemy_rect.y = travel_path_points[current_path_point][1];
    }
}


void moving_enemy::enemy_insert_path_point(short insert_idx, vector<short> path_point) {
    travel_path_points.insert(travel_path_points.begin() + insert_idx, path_point);
}

void moving_enemy::enemy_set_speed(short new_speed) {
    enemy_speed = new_speed;
}

short moving_enemy::get_enemy_x_pos() {
    return enemy_rect.x;
}

short moving_enemy::get_enemy_y_pos() {
    return enemy_rect.y;
}

void moving_enemy::clear_path_points() {

    travel_path_points.clear();
    current_path_point = 0;
}

void moving_enemy::teleport_enemy(vector<short> point) {
    enemy_rect.x = point[0];
    enemy_rect.y = point[1];
}

void moving_enemy::set_size(vector<short> new_size) {
    enemy_rect.w = new_size[0];
    enemy_rect.h = new_size[1];
}

void moving_enemy::enemy_remove_path_point(short remove_idx) {
    travel_path_points.erase(travel_path_points.begin() + remove_idx);
}

void moving_enemy::enemy_set_movable_frames(short new_movable_frames) {
    movable_frames = new_movable_frames;
}

moving_enemy::moving_enemy(const vector<vector<short>>&& points, const short&& speed, const short&& move_frames, const vector<short>&& size, short after_path_move_type):
    travel_path_points(points),
    enemy_speed(speed),
    movable_frames(move_frames),
    enemy_rect({ points[0][0], points[0][1], size[0], size[1] }),
    after_path_completion(after_path_move_type),
    current_path_point(0)
    
{}