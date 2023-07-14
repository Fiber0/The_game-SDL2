
#include "menus.h"
#include "mainf.h"


void menus::all_levels_completed_screen() {

    bool running = 1;

    SDL_Rect praise_text_rect = { 200, 30, 400, 80 };
    SDL_Rect alert_text_rect = { 50, 130, 700, 80 };
    SDL_Rect deaths_text_rect = { 120, 270, 250, 60 };
    SDL_Rect back_to_menu_button_bg = { 230, 480, 300, 90 };
    SDL_Rect back_to_menu_button_border = { 228, 478, 304, 94 };
     
    SDL_Rect sum_up_border_bottom_rect = { 80, 250, 640, 200 };
    SDL_Rect sum_up_border_cover_rect = { 90, 260, 620, 180 };

    SDL_Surface* text_surface;
    string death_counter_str = to_string(game.death_counter);
    
    short deaths_count_rect_width = death_counter_str.size() * 25;  // calculate width for rectangle storing death count text, so the text will nicely fit
    SDL_Rect deaths_count_rect = { 370, 280, deaths_count_rect_width, 48 };
    
    text_surface = TTF_RenderText_Solid(game.Arcade_classic_font, "Good Work !", { 0, 0, 128 });
    SDL_Texture* praise_text_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    text_surface = TTF_RenderText_Solid(game.Operator_font, "all levels have been completed", { 0, 0, 0 });
    SDL_Texture* alert_text_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    text_surface = TTF_RenderText_Solid(game.Operator_font, "total deaths: ", { 207, 16, 32 });
    SDL_Texture* deaths_text_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    SDL_Rect selected_button_rect = { 188, 510, 30, 30 };
    
    text_surface = TTF_RenderText_Solid(game.Operator_font, "back to menu", { 0, 0, 0 });
    SDL_Texture* back_to_menu_button_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);
    SDL_Rect back_to_menu_button = {255, 480, 250, 80};

    text_surface = TTF_RenderText_Solid(game.Operator_font, ( death_counter_str.c_str() ), { 255, 0, 0 });
    SDL_Texture* deaths_counter_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    SDL_Event event;
    SDL_FreeSurface(text_surface);
    
    game.save_death_counter_to_settings_file(0);
    game.save_level_to_settings_file(game.available_levels_vect[0]);
    game.death_counter = 0;
    
    game.current_level_idx = 0;
    
    while (running) {  // finish screen main loop

        while (SDL_PollEvent(&event)) {  // handle event
            
            if (event.type == SDL_QUIT) // check if window close button was pressed
                game.clean_exit();
            
            else if (event.type == SDL_KEYDOWN) {  // if an event occured
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        running = 0;
                        menu();
                        break;
                    case SDLK_a:
                        if(selected_button_rect.x >= 188)
                            selected_button_rect.x -= 80;
                        break;
                }
            }
        }

        if (selected_button_rect.x < 188) {
            selected_button_rect.x += 10;
        }

        // render background 
        SDL_SetRenderDrawColor(game.renderer, 94, 240, 31, 255);
        SDL_RenderClear(game.renderer);

        // render borders
        SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &sum_up_border_bottom_rect);
        SDL_RenderFillRect(game.renderer, &back_to_menu_button_border);

        // render selected button_rect
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &selected_button_rect);

        // render sum_up tab border
        SDL_SetRenderDrawColor(game.renderer, 94, 240, 31, 255);
        SDL_RenderFillRect(game.renderer, &sum_up_border_cover_rect);
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(game.renderer, &back_to_menu_button_bg);

        // render text 
        SDL_RenderCopy(game.renderer, praise_text_texture, NULL, &praise_text_rect);
        SDL_RenderCopy(game.renderer, alert_text_texture, NULL, &alert_text_rect);
        SDL_RenderCopy(game.renderer, deaths_text_texture, NULL, &deaths_text_rect);
        SDL_RenderCopy(game.renderer, deaths_counter_texture, NULL, &deaths_count_rect);
        SDL_RenderCopy(game.renderer, back_to_menu_button_texture, NULL, &back_to_menu_button);
        
        // render underline
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(game.renderer, 60, 210, 740, 210);

        SDL_RenderPresent(game.renderer);
        SDL_Delay(20);
    }
}


void menus::pause_menu() {

    TTF_Font *bigger_operator_font = game.Operator_font;
    TTF_SetFontSize(bigger_operator_font, 36);

    SDL_Rect menu_background = { 0,0, 800, 600};
    SDL_Rect heading_rect = { 255, 150, 300, 120 };
    
    SDL_Rect button_background_rect = { 220, 0, 360, 50 };
    SDL_Surface* button_text_surface;
    
    SDL_Surface* heading_text_surface = TTF_RenderText_Solid(game.Operator_font, "Paused", { 0, 0, 0 });
    SDL_Texture* heading_texture = SDL_CreateTextureFromSurface(game.renderer, heading_text_surface);

    SDL_Rect selected_button_rect = { 160, 290, 30, 30 };
    
    SDL_Rect background_rect = { 150, 150, 500, 300 };
    SDL_Rect background_rect_borders = { 147, 147, 506, 306 };
    
    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "Continue", { 0, 0, 0 });
    SDL_Texture* button_continue_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_continue_text_rect = { 300, 275, button_text_surface->w, 50 };

    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "Back to menu", { 0, 0, 0 });
    SDL_Texture* button_back_to_menu_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_back_to_menu_text_rect = { 260, 350, button_text_surface->w, 50 };


    vector <SDL_Rect> menu_buttons = { button_continue_text_rect, button_back_to_menu_text_rect};
    short selected_button_idx = 0;

    SDL_Surface* last_game_frame_surface = SDL_CreateRGBSurface(0, game.RESOLUTION_X, game.RESOLUTION_Y, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(game.renderer, nullptr, last_game_frame_surface->format->format, last_game_frame_surface->pixels, last_game_frame_surface->pitch);

    // Create a texture from the surface
    SDL_Texture* last_game_frame_texture = SDL_CreateTextureFromSurface(game.renderer, last_game_frame_surface);

    SDL_Event event;
    SDL_FreeSurface(button_text_surface);
    SDL_FreeSurface(heading_text_surface);
    bool running = 1;
    
    while (running) { // menu main loop

        while (SDL_PollEvent(&event)) {  // handle event

            if (event.type == SDL_QUIT) // check if window close button was pressed
                game.clean_exit();

            else if (event.type == SDL_KEYDOWN) {  // if an event occured
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    if (selected_button_idx != 0) {
                        selected_button_idx--;
                    }
                    break;

                case SDLK_s:
                    if (selected_button_idx < menu_buttons.size()-1) {
                        selected_button_idx++;
                    }
                    break;    
                case SDLK_ESCAPE:
                    selected_button_idx = 0;
                    running = 0;
                    break;

                case SDLK_SPACE:
                    running = 0;
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderClear(game.renderer);
        
        SDL_RenderCopy(game.renderer, last_game_frame_texture, nullptr, nullptr);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 80);
        SDL_RenderFillRect(game.renderer, &menu_background);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 128, 255);
        SDL_RenderFillRect(game.renderer, &background_rect_borders);

        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(game.renderer, &background_rect);
        
        SDL_RenderCopy(game.renderer, heading_texture, NULL, &heading_rect);
        
        if (selected_button_idx == 0 && selected_button_rect.y > 290) {
            selected_button_rect.y -= 15;
        }
        else if (selected_button_idx == 1 && selected_button_rect.y != 365) {
            selected_button_rect.y += selected_button_rect.y > 365 ? -15: 15;
        }
        
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &selected_button_rect);

        // render button background
        for (int i = 0; i < menu_buttons.size(); i++) {
            if (i == selected_button_idx) {
                SDL_SetRenderDrawColor(game.renderer,255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(game.renderer, 191, 0, 255, 255 );
            }
            
            button_background_rect.y = menu_buttons[i].y + 5;  // adding 5 to center text
            SDL_RenderFillRect(game.renderer, &button_background_rect);

            // draw button borders different color
            SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(game.renderer, &button_background_rect);
        }

        // render button textures (their text) to predefined rectangles
        SDL_RenderCopy(game.renderer, button_continue_texture, NULL, &button_continue_text_rect);
        SDL_RenderCopy(game.renderer, button_back_to_menu_texture, NULL, &button_back_to_menu_text_rect);
        
        game.present_renderer();
        SDL_Delay(20);
    }
    
    if (selected_button_idx == 1) {
        menu();
    }
    
    SDL_DestroyTexture(last_game_frame_texture);
}


short menus::scan_for_levels_and_report() {
    short len_of_levels_before = game.available_levels_vect.size();
    game.inicialize_levels_vec("../levels");
    cout << "found " << game.available_levels_vect.size() - len_of_levels_before << " new levels\n";
    
    return game.available_levels_vect.size() - len_of_levels_before;
}

void menus::level_rescan() {

    short scan_result = scan_for_levels_and_report();
    
    string found_or_lost;
    SDL_Color num_of_found_lvls_color;
    if (scan_result >= 0) {  
        found_or_lost = "found: ";
        num_of_found_lvls_color = {30, 255, 30};
    }
    else {  // if we found less levels than we previously had
        found_or_lost = "lost: ";
        num_of_found_lvls_color = { 255, 30, 30 };
        scan_result *= -1;  // convert to positive
    }
    
    string new_levels = to_string(scan_result);
    SDL_Rect menu_background = { 0,0, 800, 600 };
    SDL_Rect heading_rect = { 205, 140, 400, 120 };
    SDL_Rect found_text_rect = { 205, 260, 120, 40 };
    int num_width = (new_levels.size() * 22);
    SDL_Rect new_levels_text_rect = { 325, 263, num_width, 37 };
    SDL_Rect levels_text_rect = { 330+num_width, 260, 215, 40 };

    SDL_Rect button_background_rect = { 300, 355, 200, 50 };
    SDL_Surface* button_text_surface;
    
    SDL_Surface* heading_text_surface = TTF_RenderText_Solid(game.Operator_font, "Scan Completed", { 0, 0, 0 });
    SDL_Texture* heading_texture = SDL_CreateTextureFromSurface(game.renderer, heading_text_surface);

    SDL_Surface* found_text_surface = TTF_RenderText_Solid(game.Operator_font, found_or_lost.c_str(), { 0, 0, 0 });
    SDL_Texture* found_texture = SDL_CreateTextureFromSurface(game.renderer, found_text_surface);

    SDL_Surface* new_levels_text_surface = TTF_RenderText_Solid(game.Operator_font, new_levels.c_str(), num_of_found_lvls_color);
    SDL_Texture* new_levels_texture = SDL_CreateTextureFromSurface(game.renderer, new_levels_text_surface);

    SDL_Surface* levels_text_surface = TTF_RenderText_Solid(game.Operator_font, " new level(s)", { 0, 0, 0 });
    SDL_Texture* levels_texture = SDL_CreateTextureFromSurface(game.renderer, levels_text_surface);

    SDL_Rect selected_button_rect = { 160, 290, 30, 30 };
    
    SDL_Rect background_rect = { 150, 150, 500, 300 };
    SDL_Rect background_rect_borders = { 147, 147, 506, 306 };
    
    button_text_surface = TTF_RenderText_Solid(game.Operator_font, "Back", { 0, 0, 0 });
    SDL_Texture* button_back_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_back_text_rect = { 350, 350, button_text_surface->w, 50 };
    


    SDL_Surface* last_game_frame_surface = SDL_CreateRGBSurface(0, game.RESOLUTION_X, game.RESOLUTION_Y, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(game.renderer, nullptr, last_game_frame_surface->format->format, last_game_frame_surface->pixels, last_game_frame_surface->pitch);

    // Create a texture from the surface
    SDL_Texture* last_game_frame_texture = SDL_CreateTextureFromSurface(game.renderer, last_game_frame_surface);

    SDL_Event event;
    SDL_FreeSurface(button_text_surface);
    bool running = 1;
    
    while (running) { // menu main loop

        while (SDL_PollEvent(&event)) {  // handle event

            if (event.type == SDL_QUIT) // check if window close button was pressed
                game.clean_exit();

            else if (event.type == SDL_KEYDOWN) {  // if an event occured
                switch (event.key.keysym.sym) {   
                case SDLK_ESCAPE:
                    running = 0;
                    break;

                case SDLK_SPACE:
                    running = 0;
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderClear(game.renderer);
        
        SDL_RenderCopy(game.renderer, last_game_frame_texture, nullptr, nullptr);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 80);
        SDL_RenderFillRect(game.renderer, &menu_background);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 128, 255);
        SDL_RenderFillRect(game.renderer, &background_rect_borders);

        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(game.renderer, &background_rect);
        
        SDL_RenderCopy(game.renderer, heading_texture, NULL, &heading_rect);
        SDL_RenderCopy(game.renderer, found_texture, NULL, &found_text_rect);
        SDL_RenderCopy(game.renderer, new_levels_texture, NULL, &new_levels_text_rect);
        SDL_RenderCopy(game.renderer, levels_texture, NULL, &levels_text_rect);


        // render button background
        SDL_SetRenderDrawColor(game.renderer,255, 255, 255, 255);
        SDL_RenderFillRect(game.renderer, &button_background_rect);
        
        // draw button borders different color
        SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(game.renderer, &button_background_rect);
        
        // render button textures (their text) to predefined rectangles
        SDL_RenderCopy(game.renderer, button_back_texture, NULL, &button_back_text_rect);
        
        game.present_renderer();
        SDL_Delay(20);
    }
    level_select();
    
    SDL_DestroyTexture(last_game_frame_texture);
}



void menus::get_level_name_and_difficulty(short level_idx, string& level_name, short& level_difficulty) {
    json level_data;
    ifstream f(game.available_levels_vect[level_idx]);
    
    try {  // in case the level.json file was moved and accesing it will cause an error
        f >> level_data;
    }
    catch(...){
        level_name = "! Error loading level data !";
        level_difficulty = 4;
        return;
    }
    
    if (level_data.contains("level_name")) {
        level_name = level_data["level_name"];
    }
    else {
        level_name = game.available_levels_vect[level_idx];
    }

    if (level_data.contains("difficulty")) {
        level_difficulty = level_data["difficulty"];
    }
    else {
        level_difficulty = 0;
    }
    
    f.close();

}

void menus::update_level_number_texture(short new_level_idx, SDL_Texture*& level_number_texture, SDL_Rect& level_number_text_rect) {
    SDL_Surface* level_number_surface;
    
    level_number_surface = TTF_RenderText_Solid(game.Operator_font, (to_string(new_level_idx+1) + "/" + to_string(game.available_levels_vect.size())).c_str(), { 0, 0, 0 });
    
    level_number_texture = SDL_CreateTextureFromSurface(game.renderer, level_number_surface);
    level_number_text_rect = {(game.RESOLUTION_X - level_number_surface->w) / 2, 520, level_number_surface->w, 60 };
    SDL_FreeSurface(level_number_surface);
    
}

void menus::update_level_name_texture(string selected_level_name, SDL_Texture *& level_name_texture, SDL_Rect& level_name_text_rect) {
    SDL_Surface* level_name_surface;
    short name_rect_width;
    
    if (selected_level_name.size() * 30 < 490) {
        name_rect_width = selected_level_name.size() * 30;
    }
    else {
        name_rect_width = selected_level_name.size() * (490 / selected_level_name.size());
    }
    level_name_surface = TTF_RenderText_Solid(game.Operator_font, selected_level_name.c_str(), { 255, 255, 255 });
    
    level_name_texture = SDL_CreateTextureFromSurface(game.renderer, level_name_surface);
    level_name_text_rect = { 125 + (550 - name_rect_width) / 2, 200, name_rect_width, 90 };
    SDL_FreeSurface(level_name_surface);
}


void menus::level_select() {
    
    TTF_Font *bigger_operator_font = game.Operator_font;
    TTF_SetFontSize(bigger_operator_font, 36);

    SDL_Rect top_section_rect = { 3, 3, 794, 100 };
    SDL_Rect top_section_rect_borders = { 0, 0, 800, 106 };
    
    SDL_Rect bottom_section_rect = { 3, 503, 794, 594 };
    SDL_Rect bottom_section_rect_borders = { 0, 500, 800, 600};
    
    SDL_Rect back_button_background_rect = { 20, 25, 200, 50 };
    SDL_Rect scan_button_background_rect = { 582, 25, 200, 50 };

    SDL_Rect heading_text_rect = { 250, 15, 300, 80 };
    SDL_Surface* button_text_surface;
    SDL_Surface* heading_surface;
    
    short selected_level_difficulty;
    string selected_level_name;

    short selected_level_idx = 0;

    get_level_name_and_difficulty(selected_level_idx, selected_level_name, selected_level_difficulty);
        
    SDL_Rect difficulty_border_rect = { 100, 175, 600, 250 };
    SDL_Rect selected_level_background = { 107, 180, 586, 240 };
    
    SDL_Rect arrow_right[5] = { {710, 290, 80, 15}, {770, 280, 10, 10}, {760, 270, 10, 10}, {770, 305, 10, 10}, {760, 315, 10, 10} };
    SDL_Rect arrow_left[5] = { {10, 290, 80, 15}, {20, 280, 10, 10}, {30, 270, 10, 10}, {20, 305, 10, 10}, {30, 315, 10, 10} };
    
    SDL_Rect background_rect = { 3, 3, 794, 594 };
    SDL_Rect background_rect_borders = { 0, 0, 800, 600 };
    
    SDL_Rect selection_underline = {130, 435, 540, 15 };
    
    heading_surface = TTF_RenderText_Solid(bigger_operator_font, "Level select", { 0, 0, 0 });
    SDL_Texture* heading_texture = SDL_CreateTextureFromSurface(game.renderer, heading_surface);

    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "back", { 0, 0, 0 });
    SDL_Texture* button_back_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_back_text_rect = { 70, 20, button_text_surface->w, 50 };

    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "re-scan", { 0, 0, 0 });
    SDL_Texture* button_scan_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_scan_text_rect = { 600, 20, button_text_surface->w, 50 };

    SDL_Rect level_name_text_rect;
    SDL_Texture* level_name_texture;

    SDL_Rect level_number_text_rect;
    SDL_Texture* level_number_texture;

    update_level_name_texture(selected_level_name, level_name_texture, level_name_text_rect);
    update_level_number_texture(selected_level_idx, level_number_texture, level_number_text_rect);
    
    vector <SDL_Rect> level_select_buttons = { button_back_text_rect,  button_scan_text_rect };
    vector <SDL_Rect> button_underlines = {{ 50, 80, 140, 10 }, { 610, 80, 140, 10 }};

    map <short, SDL_Color> difficulty_to_color_map{
        {0, {0,255,0, 255} },
        {1, { 0,205,255, 255 }},
        {2, {254,231,21,255} },
        {3, {242, 110, 1, 255} },
        {4, {207, 16, 32, 255} },
        {5, {164, 32, 208, 255} }

    };
    
    SDL_Color difficulty_color = { difficulty_to_color_map[selected_level_difficulty] };

    short selected_button_idx = 0;
    short selected_row = 1;
    SDL_Event event;
    
    bool running = 1;
    while (running) { // level_select main loop

        while (SDL_PollEvent(&event)) {  // handle event
            
            if (event.type == SDL_QUIT) // check if window close button was pressed
                game.clean_exit();
            
            else if (event.type == SDL_TEXTINPUT) {  // if an event occured
                switch (event.key.keysym.sym) {
                    case SDLK_d:
                        if (selected_row == 1) {
                            selected_level_idx = selected_level_idx == game.available_levels_vect.size() - 1 ? 0 : selected_level_idx + 1;
                            get_level_name_and_difficulty(selected_level_idx, selected_level_name, selected_level_difficulty);
                            update_level_name_texture(selected_level_name, level_name_texture, level_name_text_rect);
                            update_level_number_texture(selected_level_idx, level_number_texture, level_number_text_rect);
                            difficulty_color = { difficulty_to_color_map[selected_level_difficulty] };

                            arrow_right[0].x = 780;
                            arrow_right[1].x = 840;
                            arrow_right[2].x = 830;
                            arrow_right[3].x = 840;
                            arrow_right[4].x = 830;
                            
                        }
                        else if (selected_row == 0) {
                            if (selected_button_idx == 0) {
                                selected_button_idx = 1;
                                selection_underline = button_underlines[1];
                            }   
                        }
                        break;

                    case SDLK_a:
                        if (selected_row == 1) {
                            selected_level_idx = selected_level_idx == 0 ? game.available_levels_vect.size() - 1 : selected_level_idx - 1;
                            get_level_name_and_difficulty(selected_level_idx, selected_level_name, selected_level_difficulty);
                            update_level_name_texture(selected_level_name, level_name_texture, level_name_text_rect);
                            update_level_number_texture(selected_level_idx, level_number_texture, level_number_text_rect);
                            difficulty_color = { difficulty_to_color_map[selected_level_difficulty] };

                            arrow_left[0].x = -60;
                            arrow_left[1].x = -50;
                            arrow_left[2].x = -40;
                            arrow_left[3].x = -50;
                            arrow_left[4].x = -40;
                            
                        }
                            else if (selected_row == 0) {
                            if (selected_button_idx == 1) {
                                selected_button_idx = 0;
                                selection_underline = button_underlines[0];
                            }
                        }
                        break;
                        
                    case SDLK_w:
                        if (selected_row == 1) {
                            selected_row = 0;
                            selection_underline = button_underlines[selected_button_idx];   
                        }
                        break;
                        
                    case SDLK_s:
                        if (selected_row == 0) {
                            selected_row = 1;
                            selection_underline = { 130, 435, 540, 15};
                        }
                        break;

                    case SDLK_SPACE: 
                        running = 0;
                        break;
                }
            }
        }

        if (arrow_right[0].x > 710) {
            
            arrow_right[0].x -= 10;
            arrow_right[1].x -= 10;
            arrow_right[2].x -= 10;
            arrow_right[3].x -= 10;
            arrow_right[4].x -= 10;

        }
        
        if (arrow_left[0].x < 10) { 

            arrow_left[0].x += 10;
            arrow_left[1].x += 10;
            arrow_left[2].x += 10;
            arrow_left[3].x += 10;
            arrow_left[4].x += 10;

        }

        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderClear(game.renderer);
        
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 128, 255);
        SDL_RenderFillRect(game.renderer, &background_rect_borders);

        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(game.renderer, &background_rect);
        
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 128, 255);
        SDL_RenderFillRect(game.renderer, &top_section_rect_borders);
        SDL_RenderFillRect(game.renderer, &bottom_section_rect_borders);

        SDL_SetRenderDrawColor(game.renderer, min(difficulty_color.r + 60, 255), min(difficulty_color.g + 60, 255), min(difficulty_color.b + 0 , 255), difficulty_color.a);
        SDL_RenderFillRect(game.renderer, &top_section_rect);
        SDL_RenderFillRect(game.renderer, &bottom_section_rect);

        SDL_SetRenderDrawColor(game.renderer, difficulty_color.r, difficulty_color.g, difficulty_color.b, difficulty_color.a);
        SDL_RenderFillRect(game.renderer, &difficulty_border_rect);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(game.renderer, &selected_level_background);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRects(game.renderer, arrow_right, 5);
        SDL_RenderFillRects(game.renderer, arrow_left, 5);
        
        SDL_RenderCopy(game.renderer, heading_texture, NULL, &heading_text_rect);
        SDL_RenderCopy(game.renderer, level_name_texture, NULL, &level_name_text_rect);
        SDL_RenderCopy(game.renderer, level_number_texture, NULL, &level_number_text_rect);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(game.renderer, &selection_underline);

        SDL_SetRenderDrawColor(game.renderer, 191, 0, 255, 255);
        if (selected_row == 0) {
            if (selected_button_idx == 0 && selected_row == 0) {
                SDL_RenderFillRect(game.renderer, &scan_button_background_rect);
                
                SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(game.renderer, &back_button_background_rect);
                    
            }
            else if(selected_button_idx == 1 && selected_row == 0) {
                SDL_RenderFillRect(game.renderer, &back_button_background_rect);
                
                SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(game.renderer, &scan_button_background_rect);
            }
        }
        else {
            SDL_RenderFillRect(game.renderer, &scan_button_background_rect);
            SDL_RenderFillRect(game.renderer, &back_button_background_rect);
        }

        // draw button borders
        SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(game.renderer, &scan_button_background_rect);
        SDL_RenderDrawRect(game.renderer, &back_button_background_rect);

        // render button textures (their text) to predefined rectangles
        SDL_RenderCopy(game.renderer, button_scan_texture, NULL, &button_scan_text_rect);
        SDL_RenderCopy(game.renderer, button_back_texture, NULL, &button_back_text_rect);
        
        game.present_renderer();
        SDL_Delay(20);
    }
    switch (selected_row) {
        case 0:
            switch (selected_button_idx) {
                case 0:  // back button selected
                    menu();
                    break;
                case 1:  // back button selected
                    level_rescan();
                    break;
            }
            break;
        case 1:
            game.player_dat.god_mode = 0;
            game.death_counter = 0;
            game.current_level_idx = selected_level_idx;
            game.game_mode = game.single_level;
            game.inicialize_level_related_variables();
    }
}



void menus::menu() {

    TTF_Font* bigger_operator_font = game.Operator_font;
    cout << game.RESOLUTION_X << '\n';
    TTF_SetFontSize(bigger_operator_font, 36);

    game.load_data_from_settings();  
    game.inicialize_level_related_variables();
    
    SDL_Rect menu_background = { 0,0, game.RESOLUTION_X, game.RESOLUTION_Y };
    SDL_Rect heading_rect = { 150, 50, 500, 200 };
    SDL_Rect button_background_rect = { 200, 0, 400, 50 };
    
    SDL_Surface* button_text_surface;
    
    SDL_Surface* heading_text_surface = TTF_RenderText_Solid(game.Operator_font, "The Game", { 0, 0, 0 });
    SDL_Texture* heading_texture = SDL_CreateTextureFromSurface(game.renderer, heading_text_surface);

    SDL_Rect selected_button_rect = { 160, 265, 30, 30 };
    
    SDL_Rect background_rect = { 110, 80, 570, 450 };
    SDL_Rect background_rect_borders = { 107, 77, 576, 456 };
    
    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "continue", { 0, 0, 0 });
    SDL_Texture* button_continue_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_continue_text_rect = { 300, 250, button_text_surface->w, 50 };

    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "new game", { 0, 0, 0 });
    SDL_Texture* button_new_game_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_new_game_text_rect = { 300, 315, button_text_surface->w, 50 };

    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "level select", { 0, 0, 0 });
    SDL_Texture* button_level_select_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_level_select_text_rect = { 260, 385, button_text_surface->w, 50 };

    button_text_surface = TTF_RenderText_Solid(bigger_operator_font, "quit", { 0, 0, 0 });
    SDL_Texture* button_quit_texture = SDL_CreateTextureFromSurface(game.renderer, button_text_surface);
    SDL_Rect button_quit_text_rect = { 350, 445, button_text_surface->w, 50 };

    vector <SDL_Rect> menu_buttons = { button_continue_text_rect, button_new_game_text_rect, button_level_select_text_rect,button_quit_text_rect};
    short selected_button_idx = 0;

    SDL_Event event;

    game.player_dat.god_mode = 1;
    game.game_mode = game.classic;
    
    // reset player to starting pad
    game.player_dat.player.x = game.starting_pad.x;
    game.player_dat.player.y = game.starting_pad.y;
    
    SDL_FreeSurface(heading_text_surface);
    
    bool running = 1;
    while (running) { // menu main loop
        
        while (SDL_PollEvent(&event)) {  // handle event
            
            if (event.type == SDL_QUIT) // check if window close button was pressed
                game.clean_exit();
            
            else if (event.type == SDL_TEXTINPUT) {  // if an event occured
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (selected_button_idx != 0) {
                            selected_button_idx--;
                        }
                        break;

                    case SDLK_s:
                        if (selected_button_idx < menu_buttons.size()-1) {
                            selected_button_idx++;
                        }
                        break;
                        
                    case SDLK_SPACE: 
                        running = 0;
                        break;
                }
            }
        }
        

        game.render_all_objects_and_check_for_collisions();
        
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 50);
        SDL_RenderFillRect(game.renderer, &menu_background);
        
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 128, 255);
        SDL_RenderFillRect(game.renderer, &background_rect_borders);

        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(game.renderer, &background_rect);
        
        SDL_RenderCopy(game.renderer, heading_texture, NULL, &heading_rect);
        for (int i = 0; i < 2;i++){
            if (selected_button_idx == 0 && selected_button_rect.y > 265) {
                selected_button_rect.y -= 5;
            }
            else if (selected_button_idx == 1 && selected_button_rect.y != 330) {
                selected_button_rect.y += selected_button_rect.y > 330 ? -5: 5;
            }
            else if (selected_button_idx == 2 && selected_button_rect.y != 400) {
                selected_button_rect.y += selected_button_rect.y > 400 ? -5: 5;
            }
            else if (selected_button_idx == 3 && selected_button_rect.y < 460) {
                selected_button_rect.y += 5;
            }
        }
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &selected_button_rect);

        // render button borders
        for (int i = 0; i < menu_buttons.size(); i++) {
            if (i == selected_button_idx) {
                SDL_SetRenderDrawColor(game.renderer,255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(game.renderer, 191, 0, 255, 255 );
            }
            
            button_background_rect.y = menu_buttons[i].y + 5;  // adding 5 to center text
            SDL_RenderFillRect(game.renderer, &button_background_rect);

            // draw button borders different color
            SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(game.renderer, &button_background_rect);
        }

        // render button textures (their text) to predefined rectangles
        SDL_RenderCopy(game.renderer, button_continue_texture, NULL, &button_continue_text_rect);
        SDL_RenderCopy(game.renderer, button_new_game_texture, NULL, &button_new_game_text_rect);
        SDL_RenderCopy(game.renderer, button_level_select_texture, NULL, &button_level_select_text_rect);
        SDL_RenderCopy(game.renderer, button_quit_texture, NULL, &button_quit_text_rect);
        
        game.present_renderer();
        SDL_Delay(20);
    }
    switch (selected_button_idx) {
        case 0:  // continue button selected
            game.inicialize_level_related_variables();
            game.player_dat.god_mode = 0;
            break;
    
        case 1:  // new game button selected
            game.player_dat.god_mode = 0;
            game.death_counter = 0;
            game.current_level_idx = 0;
            game.inicialize_level_related_variables();
            break;
        case 2:  // new game button selected
            level_select();
            break;

        case 3:  // quit button selected
            game.clean_exit();
            break;
    }
}


void menus::single_level_completed_screen() {
    bool running = 1;
    
    SDL_Rect back_to_menu_button_bg = { 235, 390, 330, 90 };
    SDL_Rect back_to_menu_button_text_rect = { 243, 400, 315, 60 };
    SDL_Rect back_to_menu_button_border = { 233, 388, 334, 94 };

    SDL_Rect level_completed_text_rect = { 175, 50, 450, 150 };
    
    SDL_Surface* text_surface;
    
    text_surface = TTF_RenderText_Solid(game.Operator_font, "back to level select", { 0, 0, 0});
    SDL_Texture* back_to_menu_button_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    text_surface = TTF_RenderText_Solid(game.Operator_font, "Level completed", { 0, 0, 0});
    SDL_Texture* level_completed_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    SDL_Event event;
    
    SDL_FreeSurface(text_surface);
    while (running) {  // finish screen main loop

        while (SDL_PollEvent(&event)) {  // handle event
            
            if (event.type == SDL_QUIT) // check if window close button was pressed
                game.clean_exit();
            
            else if (event.type == SDL_TEXTINPUT) {  // if an event occured
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        running = 0;
                        break;
                }
            }
        }
        
        // render background 
        SDL_SetRenderDrawColor(game.renderer, 94, 240, 31, 255);
        SDL_RenderClear(game.renderer);

        // render borders
        SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &back_to_menu_button_border);
        
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(game.renderer, &back_to_menu_button_bg);

        // render text 
        SDL_RenderCopy(game.renderer, back_to_menu_button_texture, NULL, &back_to_menu_button_text_rect);
        SDL_RenderCopy(game.renderer, level_completed_texture, NULL, &level_completed_text_rect);
        SDL_RenderPresent(game.renderer);
        SDL_Delay(10);
    }
    // back to level select
    level_select();

}

void menus::level_completed_screen() {

    bool running = 1;
    string next_level_name;
    SDL_Color level_difficulty_color;

    TTF_SetFontSize(game.Operator_font, 32);
    
    map <short, SDL_Color> difficulty_to_color_map{
        {0, {0,160,0, 255} },
        {1, { 0, 155, 255, 255 }},
        {2, {234, 211,1,255} },
        {3, {255, 130, 21, 255} },
        {4, {207, 16, 32, 255} },
        {5, {164, 32, 208, 255} }

    };
    
    // check if level has specified name
    if (game.data.contains("level_name")) {
        next_level_name = game.data["level_name"];
    }
    // if not, use it's file name
    else {
        next_level_name = game.available_levels_vect[game.current_level_idx];
    }

    if (game.data.contains("difficulty")) {
        level_difficulty_color = difficulty_to_color_map.at( game.data["difficulty"]);
    }
    else {
        level_difficulty_color = { 255, 255, 255, 255 };
    }
    next_level_name = "\"" + next_level_name + "\"";
    
    SDL_Surface* text_surface;
    
    text_surface = TTF_RenderText_Solid(game.Operator_font, (next_level_name.c_str()), { 0,0,0 });
    SDL_Texture* level_name_text_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);
    
    SDL_Rect next_up_text_rect = { 220, 80, 360, 80 };
    int next_level_x = (840 - text_surface->w) / 2;
    SDL_Rect level_name_text_rect = { next_level_x, 185, text_surface->w, text_surface->h };   

    SDL_Rect selected_button_rect = {555, 350, 30, 30};
    
    SDL_Rect back_to_menu_button_bg = { 60, 390, 300, 90 };
    SDL_Rect back_to_menu_button_text_rect = { 80, 400, 260, 60 };
    SDL_Rect back_to_menu_button_border = { 58, 388, 304, 94 };
    
    SDL_Rect continue_button_bg = { 420, 390, 300, 90 };
    SDL_Rect continue_button_text_rect = { 460, 400, 220, 60 };
    SDL_Rect continue_button_border = { 418, 388, 304, 94 };

    SDL_Rect tip_text_rect = {0,0,0,0};
    SDL_Rect tip_text_rect_border= {0,0,0,0};
    SDL_Rect tip_text_rect_background= {0,0,0,0};
    SDL_Rect tip_heading_rect= {0,0,0,0};
    
    SDL_Texture* tip_texture = nullptr;
    SDL_Texture* tip_heading_texture = nullptr;

    // check if data contains a tip
    if (game.data.contains("tip")) {
        TTF_SetFontSize(game.Operator_font, 25);
        
        // make space for the tip text
        selected_button_rect.y += 60;
        back_to_menu_button_bg.y += 60;
        back_to_menu_button_text_rect.y += 60;
        back_to_menu_button_border.y += 60;
        continue_button_bg.y +=60;
        continue_button_text_rect.y += 60;
        continue_button_border.y += 60;
        next_up_text_rect.y -= 60;
        level_name_text_rect.y -= 75;

        string tip_text = game.data["tip"];
        
        if (tip_text == "<random>") {
            tip_text = game.get_random_tip();
        }
        
        if (game.data.contains("font_size")) {
            TTF_SetFontSize(game.Operator_font, game.data["font_size"]); 
        }
        else {
        
            if (tip_text.size() < 110) {
                TTF_SetFontSize(game.Operator_font, 30); 
            }
            else if (tip_text.size() > 294) {
                TTF_SetFontSize(game.Operator_font, 20); 
                tip_text = tip_text.substr(0, 294);  // if text is too big to fit in
            }
            else if (tip_text.size() > 175) {
                TTF_SetFontSize(game.Operator_font, 20); 
            }
        }
        text_surface = TTF_RenderText_Solid_Wrapped(game.Operator_font, tip_text.c_str(), { 0, 0,0 }, 600);
        tip_text_rect = { (800 - text_surface->w)/2, 200, text_surface->w, text_surface->h };
        tip_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

        
        TTF_SetFontSize(game.Operator_font, 20);
        
        text_surface = TTF_RenderText_Solid(game.Operator_font, "tip: ", { 0, 0, 128 });
        tip_heading_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

        tip_text_rect_background = { tip_text_rect.x - 6, tip_text_rect.y , tip_text_rect.w + 12, tip_text_rect.h  };
        tip_text_rect_border = { tip_text_rect_background.x - 3, tip_text_rect_background.y - 3, tip_text_rect_background.w + 6, tip_text_rect_background.h + 6 };

        tip_heading_rect = { tip_text_rect_border.x-30, 160, 70, 30 };
        
    }

    text_surface = TTF_RenderText_Solid(game.Operator_font, "back to menu", { 0, 0, 0});
    SDL_Texture* back_to_menu_button_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    text_surface = TTF_RenderText_Solid(game.Operator_font, "continue", { 0, 0, 0});
    SDL_Texture* continue_button_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    text_surface = TTF_RenderText_Solid(game.Operator_font, "Next up: ", { 0, 0, 128 });
    SDL_Texture* next_up_text_texture = SDL_CreateTextureFromSurface(game.renderer, text_surface);

    SDL_Rect difficulty_rect = { level_name_text_rect.x - 40, level_name_text_rect.y + level_name_text_rect.h / 4 + 4 , level_name_text_rect.h/2, level_name_text_rect.h/2 };
    SDL_Rect difficulty_rect_border = {difficulty_rect.x-2,difficulty_rect.y-2,difficulty_rect.w+4, difficulty_rect.h+4 };
    
    bool selected_button = 1;
    SDL_Event event;
    
    SDL_FreeSurface(text_surface);
    while (running) {  // finish screen main loop

        while (SDL_PollEvent(&event)) {  // handle event
            
            if (event.type == SDL_QUIT) // check if window close button was pressed
                game.clean_exit();
            
            else if (event.type == SDL_TEXTINPUT) {  // if an event occured
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        running = 0;
                        break;
                    case SDLK_d:
                        selected_button = 1;
                        break;
                    case SDLK_a:
                        selected_button = 0;            
                        break;
                }
            }
        }
        
        // move selecting rectangle to selected button
        if (!selected_button && selected_button_rect.x > 195) {
            selected_button_rect.x -= 20;
        }
        else if (selected_button && selected_button_rect.x < 555) {
            selected_button_rect.x += 20;
        }
        
        // render background 
        SDL_SetRenderDrawColor(game.renderer, 94, 240, 31, 255);
        SDL_RenderClear(game.renderer);

        // render borders
        SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &back_to_menu_button_border);
            SDL_RenderFillRect(game.renderer, &continue_button_border);
        SDL_RenderFillRect(game.renderer, &tip_text_rect_border);

        SDL_SetRenderDrawColor(game.renderer, 94, 240, 31, 255);
        SDL_RenderFillRect(game.renderer, &tip_text_rect_background);
        
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &difficulty_rect_border);

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(game.renderer, &level_name_text_rect);
        

        SDL_SetRenderDrawColor(game.renderer, level_difficulty_color.r, level_difficulty_color.g, level_difficulty_color.b, 255);
        SDL_RenderFillRect(game.renderer, &difficulty_rect);

        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        if (selected_button) {  // continue button is selected

            SDL_RenderFillRect(game.renderer, &continue_button_bg);
            
            SDL_SetRenderDrawColor(game.renderer, 191, 0, 255, 255);
            SDL_RenderFillRect(game.renderer, &back_to_menu_button_bg);
        }
        else {
            
            SDL_RenderFillRect(game.renderer, &back_to_menu_button_bg);
            
            SDL_SetRenderDrawColor(game.renderer, 191, 0, 255, 255);
            SDL_RenderFillRect(game.renderer, &continue_button_bg);   
        }
        
        // render selected option rect
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &selected_button_rect);

        // render text 
        SDL_RenderCopy(game.renderer, next_up_text_texture, NULL, &next_up_text_rect);
        SDL_RenderCopy(game.renderer, level_name_text_texture, NULL, &level_name_text_rect);
        SDL_RenderCopy(game.renderer, back_to_menu_button_texture, NULL, &back_to_menu_button_text_rect);
        SDL_RenderCopy(game.renderer, continue_button_texture, NULL, &continue_button_text_rect);
        SDL_RenderCopy(game.renderer, tip_texture, NULL, &tip_text_rect);
        SDL_RenderCopy(game.renderer, tip_heading_texture, NULL, &tip_heading_rect);
        
        SDL_RenderPresent(game.renderer);
        SDL_Delay(10);
    }
    // handle selected button
    if (!selected_button) { // back_to_menu button selected
        game.save_level_to_settings_file(game.available_levels_vect[game.current_level_idx]);
        game.save_death_counter_to_settings_file(game.death_counter);
        menu();
    }
}
