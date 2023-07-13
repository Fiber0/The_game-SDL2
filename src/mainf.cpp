// version 1.0 released* in 17.3.2023
// made by Oliver Horský
#include "menus.h"
#include "mainf.h"
#include "moving_enemy.h"
#include "trigger.h"


void not_so_clean_exit() {
    cout << "exiting quite safely...\n";
    int threadReturnValue;
    TTF_Quit();
    SDL_Quit();
    exit(1);
}


double inline round_to_decimals(int decimal_points, double num) { return floor(num * pow(10, decimal_points) + 0.5) / pow(10, decimal_points); }

  // evaluates string expression to number (something like calculator)
    // supported operators: + ; - ; * ; / ; ^ (power) ; 2v (square root) ; r (random number in range: 2r6) ; 0a (absolute value: 0a-8)
    // for example, this is a valid expression: -((3+6*12)/8^2-(3(-3))+15), which evaluates to -16, because it can't calculate with floating points
    // note that 3(-3) is taken as 3-3 not 3*(-3)
    
string eval(string exp) {
 
    for (int i = 0; i < exp.size(); i++) {  // find opening bracket
        if (exp[i] == '(') {
            short brackets_to_skip = 0;
            
            for (int o = i + 1; o < exp.size(); o++) {  // find it's closing bracket
              
                if (exp[o] == '(') brackets_to_skip++;
                else if (exp[o] == ')') {
                    if (brackets_to_skip == 0) {
                        string bracket_exp = exp.substr(i + 1, o - (i + 1));
                        exp.erase(i, o - (i -1));

                        string result = eval(bracket_exp);
                        exp.insert(i, result);
                        break;
                    }
                    else {brackets_to_skip--;}
                }
            }    
        }
    }
    
    map <char, short> operation_priority_map = {
        
        {'+', 3},
        {'-', 3},
        {'*', 2},
        {'/', 2},
        {'^', 1},
        {'v', 1},
        {'r', 1},
        {'a', 1}
    };


    vector <int> top_priority_operator_vec;  // stores operator order
    vector <int> medium_priority_operator_vec;
    // not using small_priority_vec, because at the point, where all operators with higher priority have been evaluated, we can simply iterate through the remaining operators (all of them will have smallest priority)

    string operators;

    vector <float> number_vec; 

    if (exp.empty()) {
        return "";
    }

    // inicialize maps
    bool is_previous_char_operator = 1;
    int operator_counter = 0;
    for (int i = 0; i < exp.size(); i++) {
        
        short number_end = exp.size();
        int o = i;
        for (;o < exp.size();o++) {  // search for operators
                
            if (operation_priority_map[exp[o]] && !is_previous_char_operator) {  // if digit is operator

                if (operation_priority_map[exp[o]] == 1) {
                    top_priority_operator_vec.emplace_back(operator_counter - top_priority_operator_vec.size());
                }
                else if (operation_priority_map[exp[o]] == 2) {
                    medium_priority_operator_vec.emplace_back(operator_counter - medium_priority_operator_vec.size());  
                }
                
                is_previous_char_operator = 1;
                operator_counter++;
                number_end = o;
                break;
            }
            else {
                is_previous_char_operator = 0;
            }
        }
        
        operators += exp[o];
        if (o || i){  // if number was specified
            number_vec.emplace_back(stof(exp.substr(i, number_end - i)));
        }
        i = o;
    }
    
    float num1;
    float num2;
    
    for (int i = 0; i < top_priority_operator_vec.size();i++) {
        num1 = number_vec[top_priority_operator_vec[i]];
        num2 = number_vec[top_priority_operator_vec[i] + 1];
        
        switch (operators[top_priority_operator_vec[i]]) {
            
        case '^':
            number_vec[top_priority_operator_vec[i]] = pow(num1, num2);
            break;
        case 'v':
            number_vec[top_priority_operator_vec[i]] = sqrt(num2);
            break;
        case 'r':
            number_vec[top_priority_operator_vec[i]] = (rand() % (int)round((num2 - num1 + 1))) + num1;
            break;
        case 'a':
            number_vec[top_priority_operator_vec[i]] = num2 < 0 ? num2*-1 : num2;
            break;

        default:
            cout << "!!! fatal error- invalid operator specified: " << operators[top_priority_operator_vec[i]] << '\n';
            not_so_clean_exit();
            break;
        }
        number_vec.erase(number_vec.begin() + top_priority_operator_vec[i] + 1);
        operators.erase(top_priority_operator_vec[i], 1);  // remove operator entry from pos_to_operator, so there won't be a null ptr
        top_priority_operator_vec.erase(top_priority_operator_vec.begin() + i);
        i--;
    }
    
    for (short i = 0; i < medium_priority_operator_vec.size(); i++) {
        
        num1 = number_vec[medium_priority_operator_vec[i]];
        num2 = number_vec[medium_priority_operator_vec[i] + 1];
        
        switch (operators[medium_priority_operator_vec[i]]) {
            
        case '*':
            number_vec[medium_priority_operator_vec[i]] = num1 * num2;
            break;

        case '/':
            number_vec[medium_priority_operator_vec[i]] = num1 / (num2*1.0);
            break;
            
        default:
            cout << "!!! fatal error- invalid operator specified" << operators[top_priority_operator_vec[i]] << '\n';
            not_so_clean_exit();
            break;
        }
        number_vec.erase(number_vec.begin() + medium_priority_operator_vec[i] + 1);
        operators.erase(medium_priority_operator_vec[i], 1);  // remove operator entry from pos_to_operator, so there won't be a null ptr
        medium_priority_operator_vec.erase(medium_priority_operator_vec.begin() + i);
        i--;
    }
    
    for (int i = 0; i < operators.size()-1; i++) {
        
        num1 = number_vec[0];
        num2 = number_vec[1];
        
        switch (operators[0]) {
            
        case '+':
            number_vec[0] = num1 + num2;   
            break;
            
        case '-':
            number_vec[0] = num1 - num2;   
            break;
            
        default:
            cout << "!!! fatal error--invalid operator specified: " << operators[0] << '\n';
            not_so_clean_exit();
            break;
        }
        number_vec.erase(number_vec.begin() + 1);
        operators.erase(0, 1);  // remove operator entry from pos_to_operator, so there won't be a null ptr
        i--;
    }
    return to_string(number_vec[0]);
}


Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;

int delta_update(void* data) { // calculate movement speed based on game preformance(speed)
        while (1) {
            LAST = NOW;
            NOW = SDL_GetPerformanceCounter();
            SDL_Delay(10);
        }
        return 0;
}


void Game_core::clean_exit() {
    cout << "safely exiting...\n";
    
    save_death_counter_to_settings_file(death_counter);
    save_level_to_settings_file(available_levels_vect[current_level_idx]);
    SDL_DetachThread(thread);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    exit(1);
}


void Game_core::increment_frame_counter() { frame_counter++; }


string Game_core::get_random_tip() {
    unsigned short pick = random() % 1000;
    vector <string> tips = { "you can enable god mode by pressing the 'magic button'. keep in mind that this will set your death counter to 99999.",
                            "you can always make your own level and load it into the game.",
                            "don't try to break the game, because there is a great chance you will.",
                            "try not to collide with anything bad",
                            "difficulties:                                   \ngreen - free                       \nblue - easy                         \nyellow - medium              \norange - harder\nred - hard\npurple - ultrakill",   // adding spaces, so that the text size will exceed character limit an get resized
                            "remember, it's just a game. Quite easy one.",
                            "you have no idea how lucky you are to read this.",
    };
    
    if (pick == 120) {
        return tips[6];
    }
    else if (pick < 300) {
        return tips[0];
    }
    else if (pick < 500) {
        return tips[1];
    }
    else if (pick < 700) {
        return tips[2];
    }
    else if (pick < 800) {
        return tips[3];
    }
    else if (pick < 900) {
        return tips[4];
    }
    else if (pick < 990) {
        return tips[5];
    }
    else {
        return "No tip for you"; 
    }
    
}

bool Game_core::tolerative_collision_check(SDL_Rect player, const SDL_Rect* rect2) {
    player.x += 3;
    player.y += 3;
    player.w -= 6;
    player.h -= 6;
    
    return SDL_HasIntersection(&player, rect2);

}

void Game_core::render_all_objects_and_check_for_collisions() {

    // render background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderClear(renderer);

    // render starting_pad borders
    SDL_SetRenderDrawColor(renderer, 144, 245, 133, 255);
    SDL_RenderFillRect(renderer, &starting_pad_border);

    // render starting_pad center
    SDL_SetRenderDrawColor(renderer, 255, 239, 0, 255);
    SDL_RenderFillRect(renderer, &starting_pad);

    // render goal_pad border
    SDL_SetRenderDrawColor(renderer, 144, 245, 133, 255);
    SDL_RenderFillRect(renderer, &goal_pad_border);  // using starting pad variables, so goal_pad and start_pad will have the same size

    // render goal_pad
    SDL_SetRenderDrawColor(renderer, 41, 190, 21, 255);
    SDL_RenderFillRect(renderer, &goal_pad);  // using starting pad variables, so goal_pad and start_pad will be the same 
    
    if (player_died) {
        on_death_animation();
    }
    
    // render player
    if (player_dat.god_mode) {
        SDL_SetRenderDrawColor(renderer, 30, 255, 30, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    
    SDL_RenderFillRect(renderer, &player_dat.player);
    SDL_RenderDrawRect(renderer, &player_dat.player);

    // render all moving enemies and check if any collides with player
    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
    for (auto& moving_enemy : moving_enemies_map) {
        SDL_Rect moving_enemy_rect = moving_enemy.second.get_enemy_rect();
        
        SDL_RenderDrawRect(renderer, &moving_enemy_rect);
        SDL_RenderFillRect(renderer, &moving_enemy_rect);
        moving_enemy.second.move_enemy(frame_counter);

        if (tolerative_collision_check(player_dat.player, &moving_enemy_rect)) { on_death(); }
    }

    // render all triggers
    for (auto& triggerr : triggers_map) {
        
        vector <short> c = triggerr.second.get_trigger_color();
        SDL_Rect trigger_rect = triggerr.second.get_trigger_rect();
        
        SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], c[3]);
        SDL_RenderDrawRect(renderer, &trigger_rect);
        SDL_RenderFillRect(renderer, &trigger_rect);
        
        
        if (SDL_HasIntersection(&player_dat.player, &trigger_rect)) { triggerr.second.on_trigger('p', "" ,data["triggers"],frame_counter, player_dat ,moving_enemies_map, triggers_map); }
        
        if (trigger_and_enemy_collisions) {  // if collision-checking for triggers and enemies is enabled
            for (auto& moving_enemy : moving_enemies_map) {
                SDL_Rect enemy_rect = moving_enemy.second.get_enemy_rect();
                
                if (SDL_HasIntersection(&trigger_rect, &enemy_rect)) { triggerr.second.on_trigger('e', moving_enemy.first, data["triggers"], frame_counter, player_dat ,moving_enemies_map, triggers_map); }   
            }
        }
    }
    
    // render all non-moving enemies and check if any collides with player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (SDL_Rect enemy : enemies) {
        
        SDL_RenderFillRect(renderer, &enemy);
        SDL_RenderDrawRect(renderer, &enemy);
        
        if (tolerative_collision_check(player_dat.player, &enemy)) { on_death(); }
    }    
    
    // render border
    SDL_SetRenderDrawColor(renderer, 134, 1, 175, 255);
    SDL_RenderDrawRect(renderer, &window_border);
    
    if (SDL_HasIntersection(&player_dat.player, &goal_pad)) {
        if (game_mode == classic) {
            on_classic_mode_level_completion();
        }
        else if (game_mode == single_level) {
            on_single_level_mode_level_completion();
        }
    }
}

void Game_core::present_renderer() { SDL_RenderPresent(renderer); }


void Game_core::handle_event(SDL_Event &event) {
    if (event.type == SDL_QUIT) // check if window close button was pressed
        clean_exit();
    
    else if (event.type == SDL_KEYDOWN) {  // if key was pressed occured
        switch (event.key.keysym.sym) {
            
        case SDLK_r:
            inicialize_level_related_variables();
            break;
            
        case SDLK_p:
            player_dat.god_mode = player_dat.god_mode ? 0 : 1;
            death_counter = 99999;
            break;
            
        case SDLK_ESCAPE:
            SDL_Delay(10);
            MENU->pause_menu();
            break;
            
        case SDLK_SPACE:
            if (action_button_enabled) {
                
                triggers_map.at(action_trigger_id).on_trigger('a', "", data["triggers"], frame_counter, player_dat ,moving_enemies_map, triggers_map); 

            }
            break;
        }
    }
}


void Game_core::handle_movement(const Uint8* keyboard_state) {
    delta_motion = player_dat.speed_per_second * (((NOW - LAST) / (double)SDL_GetPerformanceFrequency())); // calculate speed accordingly to current framerate

    if (!player_died) {  // block movement if player have died
        
        if (keyboard_state[SDL_SCANCODE_A]) { 
            player_dat.player.x -= delta_motion - 1;   // subtracting one, because the speed will get rounded up
        }
        if (keyboard_state[SDL_SCANCODE_D]) {
            player_dat.player.x += delta_motion;
        }
        if (keyboard_state[SDL_SCANCODE_S]) {
            player_dat.player.y += delta_motion;
        }
        if (keyboard_state[SDL_SCANCODE_W]) {
            player_dat.player.y -= delta_motion - 1;
        }
    }
}


void Game_core::keep_player_in_window() {
    if ((previous_tick_player_pos[0] != player_dat.player.x) || (previous_tick_player_pos[1] != player_dat.player.y)) {  // check if player has moved            
    
        player_dat.player.x = clamp(player_dat.player.x, 0, RESOLUTION_X - player_dat.player.w);
        player_dat.player.y = clamp(player_dat.player.y, 0, RESOLUTION_Y - player_dat.player.h);
        
        previous_tick_player_pos[0] = player_dat.player.x;
        previous_tick_player_pos[1] = player_dat.player.y;
    } 
}

void Game_core::on_single_level_mode_level_completion() {
    
    MENU->single_level_completed_screen();
    frame_counter = 0;
}

void Game_core::on_classic_mode_level_completion() {
    
    if (current_level_idx + 1 >= (available_levels_vect.size())) { // if all levels have been completed
        cout << "\n\n!! all levels have been completed !! \n\n";
        current_level_idx = 0;
        inicialize_level_related_variables();
        MENU->all_levels_completed_screen();

    }
    else {
        cout << available_levels_vect[current_level_idx] << " completed, moving on to " << available_levels_vect[current_level_idx + 1] << "\n\n";
        current_level_idx++;
        inicialize_level_related_variables();
    
        MENU->level_completed_screen();
    }
    
    frame_counter = 0;
    save_death_counter_to_settings_file(death_counter);
    save_level_to_settings_file(available_levels_vect[current_level_idx]);
}


void Game_core::on_death() {
    
    if (!player_died && !player_dat.god_mode) {  // if the player is not already dead and not in god mode
        cout << "! you died ! \n";
        player_died = 1;
        death_counter++;
        cout << "death_counter: " << death_counter << '\n';
        settings_data["deaths_so_far"] = death_counter;
    }
}


void Game_core::on_death_animation() {
    
    if (player_dat.player.h > 1) {
        // shring player to its center
        player_dat.player.x += 1;
        player_dat.player.y += 1;
        player_dat.player.w -= 2;
        player_dat.player.h -= 2;

        SDL_Delay(15);
    }
    
    else {
        
        // flash red color on screen
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
        
        //reset player's position and size to starting_pad's
        player_dat.player.x = starting_pad.x;
        player_dat.player.y = starting_pad.y;
        player_dat.player.w = starting_pad.w;
        player_dat.player.h = starting_pad.h;
        
        player_died = 0;
        if (rebuild_level) {
            // check if level should be rebuild, and do so if yes 
            reset_level();
        } 
    }
}

void Game_core::load_data_from_settings() {  // search for saved level's index in available_levels
        
    if (settings_data.contains("last_level") && settings_data.contains("deaths_so_far")) {  // check if the settings file contains the stuff it should contain
            
        death_counter = settings_data["deaths_so_far"];
        string saved_level = settings_data["last_level"];
            
        for (int i = 0; i < available_levels_vect.size(); i++) {  // search for saved level's idx
            if (available_levels_vect[i] == saved_level) {
                current_level_idx = i;
                return;
                }
        }
        cout << " !!! invalid level save !!! --> starting from first\n";
        current_level_idx = 0;
    }
    
    else {
        cout << "!!! settings file corrupted -> Don't mess with the settings file !!! -- recreating \n";
        remove("settings.json");
        look_for_settings_file();
    }
}


void Game_core::look_for_settings_file() {  // check if settings.json file exists
    
    if (FILE* file = fopen("settings.json", "r")) {
        // load its contents
        ifstream settings_file("settings.json");
        settings_file >> settings_data;
        fclose(file);
    }
    else {  // if not, create it and set current_level to first level in available_levels and deaths_so_far to death_counter
        
            fstream settings_file;
            settings_data["deaths_so_far"] = death_counter;
            settings_data["last_level"] = available_levels_vect[0];
            settings_file.open("settings.json", ios::out);
            settings_file << settings_data;
            settings_file.close();
            
            current_level_idx = 0;
    }
}


void Game_core::save_death_counter_to_settings_file(int deaths) {
    if (game_mode == classic){
        fstream settings_file;
        settings_data["deaths_so_far"] = deaths;
        
        settings_file.open("settings.json", ios::out);
        settings_file << settings_data;
        settings_file.close();
    }
}


void Game_core::save_level_to_settings_file(string level) {
    if (game_mode == classic){
        fstream settings_file;
        settings_data["last_level"] = level;
        
        settings_file.open("settings.json", ios::out);
        settings_file << settings_data;
        settings_file.close();
    }
}


void Game_core::inicialize_level_related_variables() {
    cout << available_levels_vect[current_level_idx] << "\n";
    cout << current_level_idx << "\n";

    // clear previous level objects
    enemies = {};
    moving_enemies_map = {};
    triggers_map = {};
    frame_counter = 0;
    player_dat.speed_per_second = 300;

    ifstream f(available_levels_vect[current_level_idx]);
    f >> data;

    if (data.contains("trigger_and_enemy_collisions") && data["trigger_and_enemy_collisions"]) {  // check if "trigger_and_enemy_collisions" is specified and set it accordingly
        trigger_and_enemy_collisions = 1;
    }
    else { trigger_and_enemy_collisions = 0; }
    
    if (data.contains("rebuild_level_on_death") && data["rebuild_level_on_death"]) {  // check if level should be rebuild on players death and set it accordingly
        rebuild_level = 1;
    }
    else{rebuild_level = 0;}

    
    if (data.contains("action_button") && data["action_button"]) {  // check if level works with action button (space)
        action_button_enabled = 1;
    }
    else{action_button_enabled = 0;}

    // inicialize starting pad
    starting_pad.x = data["starting_pad"][0];
    starting_pad.y = data["starting_pad"][1];
    starting_pad.w = data["starting_pad"][2];
    starting_pad.h = data["starting_pad"][3];

    // inicialize starting pad border
    starting_pad_border = starting_pad;
    starting_pad_border.x -= (int)(starting_pad.w * 0.33);
    starting_pad_border.y -= (int)(starting_pad.h * 0.33);
    starting_pad_border.h += starting_pad.h * 0.66;
    starting_pad_border.w += starting_pad.w * 0.66;

    // inicialize goal pad
    goal_pad.x = data["goal_pad"][0];
    goal_pad.y = data["goal_pad"][1];
    goal_pad.w = data["goal_pad"][2];
    goal_pad.h = data["goal_pad"][3];

    // inicialize goal pad border
    goal_pad_border = goal_pad;
    goal_pad_border.x -= (int)(goal_pad.w * 0.33);
    goal_pad_border.y -= (int)(goal_pad.h * 0.33);
    goal_pad_border.h += goal_pad.h * 0.66;
    goal_pad_border.w += goal_pad.w * 0.66;

    player_dat.player = starting_pad; // set player coordinates and size to starting_pad
    
    SDL_Rect dummy;
    for (int i = 0; i < data["enemies"].size(); i++) {
    
        dummy.x = data["enemies"][i][0]; // x coordinate
        dummy.y = data["enemies"][i][1]; // y coordinate
        dummy.w = data["enemies"][i][2]; // width
        dummy.h = data["enemies"][i][3]; // height
        enemies.emplace_back(dummy);
    }

    json moving_enemies_data = data["moving_enemies"];
    string moving_enemy_id;
    short after_path_move_type ;
    for (int i = 0; i < moving_enemies_data.size(); i++) {
        
        if (moving_enemies_data[i].contains("id")) {  // check if id is specified
            moving_enemy_id = moving_enemies_data[i]["id"];
        }
        else{
            moving_enemy_id = to_string(i);
        }
        
        if (moving_enemies_data[i].contains("after_path_completion")) {  // check if after_path_completion is specified
            after_path_move_type = moving_enemies_data[i]["after_path_completion"];
        }
        else{
            after_path_move_type = 0;
        }
        // remove unique_ptr for better readability and preformance (don't forget to release memory !!)
        moving_enemies_map.insert({ moving_enemy_id,  moving_enemy(moving_enemies_data[i]["path_points"], moving_enemies_data[i]["speed"], moving_enemies_data[i]["move_every_n-th_frame"], moving_enemies_data[i]["size"], after_path_move_type) });
    }

    json triggers_data = data["triggers"];
    string trigger_id;
    int re_enable_rate;
    bool evaluate_trigger_function_params;
    for (int i = 0; i < triggers_data.size(); i++) {
        
        if (triggers_data[i].contains("id")) {  // check if id is specified
            trigger_id = triggers_data[i]["id"];
        }
        else{
            trigger_id = to_string(i);
        }

        if (triggers_data[i].contains("re_enable_rate")) {  // check if "re_enable_rate" is specified
            re_enable_rate = triggers_data[i]["re_enable_rate"];
        }
        else{
            re_enable_rate = -1;
        }
        
        // remove unique_ptr for better readability and preformance (don't forget to release memory !!)
        triggers_map.insert({ trigger_id, trigger(triggers_data[i]["exists"], i, triggers_data[i]["position"], triggers_data[i]["size"], triggers_data[i]["color_rgba"], re_enable_rate  )});
    }

    // action button
    if (action_button_enabled) {
        if (!data.contains("action_button_call")) {  // check if level should be rebuild on players death and set it accordingly
            cout << "!!! fatal error at level !!! - action_button is enabled, however action_button_call is not specified\n";
            clean_exit();
        }

        action_trigger_id = data["action_button_call"];
    }
    
    f.close();
}


void Game_core::reset_level() {
    
    // clear previous level objects
    moving_enemies_map = {};
    triggers_map = {};
    frame_counter = 0;
    player_dat.speed_per_second = 300;

    player_dat.player = starting_pad; // set player coordinates and size to starting_pad

    json moving_enemies_data = data["moving_enemies"];
    string moving_enemy_id;
    short after_path_move_type ;
    for (int i = 0; i < moving_enemies_data.size(); i++) {
        
        if (moving_enemies_data[i].contains("id")) {  // check if id is specified
            moving_enemy_id = moving_enemies_data[i]["id"];
        }
        else{
            moving_enemy_id = to_string(i);
        }
        
        if (moving_enemies_data[i].contains("after_path_completion")) {  // check if after_path_completion is specified
            after_path_move_type = moving_enemies_data[i]["after_path_completion"];
        }
        else{
            after_path_move_type = 0;
        }
        
        moving_enemies_map.insert({ moving_enemy_id, (*(unique_ptr<moving_enemy>(new moving_enemy(moving_enemies_data[i]["path_points"], moving_enemies_data[i]["speed"], moving_enemies_data[i]["move_every_n-th_frame"], moving_enemies_data[i]["size"], after_path_move_type)))) });
    }

    json triggers_data = data["triggers"];
    string trigger_id;
    int re_enable_rate;
    bool evaluate_trigger_function_params;
    for (int i = 0; i < triggers_data.size(); i++) {
        
        if (triggers_data[i].contains("id")) {  // check if id is specified
            trigger_id = triggers_data[i]["id"];
        }
        else{
            trigger_id = to_string(i);
        }

        if (triggers_data[i].contains("re_enable_rate")) {  // check if "re_enable_rate" is specified
            re_enable_rate = triggers_data[i]["re_enable_rate"];
        }
        else{
            re_enable_rate = -1;
        } 

        triggers_map.insert({ trigger_id, (*(unique_ptr<trigger>(new trigger(triggers_data[i]["exists"], i, triggers_data[i]["position"], triggers_data[i]["size"], triggers_data[i]["color_rgba"], re_enable_rate  ))))});
    }

    // action button
    if (action_button_enabled) {
        if (!data.contains("action_button_call")) {  // check if level should be rebuild on players death and set it accordingly
            cout << "!!! fatal error at level !!! - action_button is enabled, however action_button_call is not specified\n";
            clean_exit();
        }

        action_trigger_id = data["action_button_call"];
    }
}


void Game_core::inicialize_levels_vec(string dir_path) { // leave blank string for path to be set to program's directory
    string filename;
    string directory;
    map <int, string> ordered_levels_map;
    available_levels_vect.clear();

    (dir_path == "") ? (directory = (std::filesystem::current_path()).string()) : (directory = dir_path);

    cout << "scanning directory for levels... : " << directory << "\n";

    for (const auto& entry : std::filesystem::directory_iterator(directory)){ // iterate throught all files in directory
        filename = (entry.path().filename()).string();
    
        if (filename.rfind("level", 0) == 0) {

            ordered_levels_map[stoi(filename.substr(5, filename.size() - 10))] = "../levels/"+filename;  // placing level_number into ordered_map, which will 'automatically sort' all placed keys(level numbers)
        } 
    }
    cout << "found levels: ";
    for (auto s : ordered_levels_map) {  // rewrite ordered_levels_map to levels_vector
        cout << s.first << "; ";
        available_levels_vect.emplace_back(s.second);

    }
    cout << "\n\n";
}


void Game_core::on_construction() {  // rename
    
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);

    // load fonts
    Operator_font = TTF_OpenFont("../fonts/8bitOperatorPlusSC-Bold.ttf", 32);
    Arcade_classic_font = TTF_OpenFont("../fonts/ArcadeClassic.ttf", 32);
    
    // test, if fonts were loaded correctly
    if (Operator_font == 0 || Arcade_classic_font == 0) {  
        cout << "!! Could not load font !!" << endl;
        clean_exit();
    }
    SDL_Thread* thread = SDL_CreateThread(delta_update, "delta_update", (void*)&data); // start background thread

    window = SDL_CreateWindow("The Game", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,  RESOLUTION_X, RESOLUTION_Y, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  // enable blending
    
    // load icon
    SDL_Surface* icon = IMG_Load("icon.png");
    SDL_SetWindowIcon(window, icon);
    
    // enter the main menu
    MENU->menu();
    
}
Game_core game;

int main(int argc, char* argv[]) {
    srand(time(0));
    
    SDL_Event e;
    const Uint8* keyboard_state;
    
    
    game.inicialize_levels_vec("../levels");  // 'construct' a vector of levels located in program's directory
    
    game.look_for_settings_file();  // check if settings file exists, if not create it
    
    game.load_data_from_settings();  // find level saved in settings file in levels_vec and save its index 
    
    game.inicialize_level_related_variables();
    
    game.on_construction();

    
    while (true) {  // main loop
        
        while (SDL_PollEvent(&e)) {  // if an event occured
            game.handle_event(e);
        }
        
        keyboard_state = SDL_GetKeyboardState(NULL);  // get state of all keyboard keys
        
        game.handle_movement(keyboard_state);
        
        game.keep_player_in_window();

        game.render_all_objects_and_check_for_collisions();
        
        game.present_renderer(); // render objects
        
        game.increment_frame_counter();
        
        SDL_Delay(10);   
    }
    
    return 0;
}