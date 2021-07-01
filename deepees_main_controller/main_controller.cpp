#include "main_controller.h"

main_controller::main_controller(int screen_width, int screen_height, std::string screen_name, bool printing_on){
    counted_frames = 0;

    this->screen_width = screen_width;
    this->screen_height = screen_height;
    this->screen_name = screen_name;

    //TIMER COUNT AND FLAGS
    previous_ticks = 0;
    timer_play = true;
    pause_play = false;
    playing = true;
    paused = false;

    //INITIALIZING WINDOW AND SDL ElEMENTS
    window = new m_window();
    window_surface = NULL;

    renderer = NULL;
    renderer_setting = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    main_timer = timer();
    cap_timer = timer();
    main_timer.play();

    quit = false;

    //INITIALIZING POINTING MAPS
    images = Image_Point_Map();
    image_sources = Image_Source_Point_Map();
    music = std::map<std::string, mixer_point*>();
    sounds = std::map<std::string, mixer_point*>();

    //INITIALIZING EVENT STUFF
    room_event_list = std::queue<int>();
    key_down_events = std::vector<int>();

    initialize_queue = std::queue<loop_procedure>();
    closing_queue = std::queue<loop_procedure>();
    pre_render_procedures = std::vector<loop_procedure>();
    post_render_procedures = std::vector<loop_procedure>();


    key_down_procedures = std::vector<key_procedure>();

    key_up_procedures = std::vector<key_procedure>();

    key_state_procedures = std::vector<key_state_procedure>();

    mouse_procedures = std::vector<room_procedure>();



    printing = printing_on;

    rooms = NULL;

    is_closed = true;
}

main_controller::~main_controller(){
    if (!is_closed)
        close();

    main_timer.~timer();
    cap_timer.~timer();

    //SAFER MEANS OF EXITING THE GAME (ASSUMING OTHER FRAMES WILL HAVE A LOT OF ELEMENTS
    //
    images.clear();
    music.clear();
    sounds.clear();
    font_lib.clear();

    std::cout << "Clearing and deleting rooms successful!" << std::endl;

    std::cout << "Clearing and deleting procedures..." << std::endl;

    std::queue<int>().swap(room_event_list);
    std::vector<int>().swap(key_down_events);

    std::queue<key_push>().swap(key_down_queue);
    std::queue<key_push>().swap(key_up_queue);

    std::vector<key_procedure>().swap(key_down_procedures);
    std::vector<key_procedure>().swap(key_up_procedures);
    std::vector<key_state_procedure>().swap(key_state_procedures);

    std::vector<room_state_procedure>().swap(mouse_state_procedures);
    std::vector<room_procedure>().swap(mouse_procedures);

    std::queue<loop_procedure>().swap(initialize_queue);

    std::queue<loop_procedure>().swap(closing_queue);

    std::vector<loop_procedure>().swap(pre_render_procedures);

    std::vector<loop_procedure>().swap(post_render_procedures);

    std::cout << "Clearing and deleting procedures successful!" << std::endl;
    std::cout << "Finalizing closing..." << std::endl;

    close();
}

//CODE OF INITIATION AND CLOSING PROCEDURES
//

bool main_controller::initialize(){
    //INITIATION FLAG
    bool init_flag = true;

    //START BY INITIALIZING BASE SDL
    //
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0){
        if(printing) std::cout << "Failed to initialize: " << SDL_GetError() << std::endl;
        init_flag = false;
    }

    if (init_flag)
        //INITIALIZING WINDOW
        //
        if (!(window->init(screen_width, screen_height, screen_name.c_str()))){
            if(printing) std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            init_flag = false;
        }

    ///INITIALIZING WINDOW AND RENDERER
    if (init_flag){
        //INITIALIZE RENDERER AND OTHER ELEMENTS
        //
        renderer = window->create_renderer(renderer_setting);
        window_surface = window->get_surface();

        if (renderer == NULL){
            if(printing) std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
            init_flag = false;
        }

        //std::cout << renderer << std::endl;
    }

    ///INITIALIZING IMAGE LIBRARY
    if (init_flag){
        SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
        SDL_RenderClear(renderer);
        //INITIALIZING IMAGE LIBRARY
        //
        int png_flag = IMG_INIT_PNG;

        if (!(IMG_Init(png_flag) & png_flag)){
            if(printing) std::cout << "Failed to load png images: " << IMG_GetError() << std::endl;
            init_flag = false;
        }

        //HIDING THE CURSOR FOR CUSTOM CURSORS
        if ((SDL_ShowCursor(cursor_setting)) < 0){
            if(printing) std::cout << "Failed to hide mouse: " << SDL_GetError() << std::endl;
            init_flag = false;
        }
    }

    if (init_flag){
        //INITIALIZING TRUETYPEFONTS
        //
        if (TTF_Init() == -1){
            if(printing) std::cout << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
            init_flag = false;
        } else {
            font_lib = std::map<std::string, TTF_Font*>();
        }
    }

    if (init_flag){
        //INITIALIZING MIXER AND AUDIO
        //
        if (Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,2048) < 0){
            if(printing) std::cout << "Failed to initiate sound mixer!" << Mix_GetError() << std::endl;
            init_flag = false;
        } else
            mixer = new mixer_bin();
    }

    if(printing) std::cout << "Initializing SDL elements successful!" << std::endl;

    ///INITIALIZING IMAGE SOURCE OBJECT
    if (init_flag){
        sources = new image_source_controller();

        if (sources == NULL){
            init_flag = false;
        } else {
            sources->set_renderer(renderer);
        }
    }

    ///INITIALIZING ROOM CONTROLLER
    if (init_flag){
        rooms = new room_controller(window, renderer, sources);
        preloop_initiation();
        is_closed = false;
    }

    return init_flag;
}

void main_controller::close(){
    quit = true;

    std::cout << "Clearing and deleting rooms..." << std::endl;
    delete rooms;
    rooms = NULL;

    //Clear mixer
    delete mixer;
    mixer = NULL;

    //Clear window
    //
    if(printing) std::cout << "Clearing window........";
    delete window;
    window = NULL;
    window_surface = NULL;

    if(printing) std::cout << "successfully closed!" << std::endl;

    //Clearing fonts
    //
    if(printing) std::cout << "Clearing fonts........";
    for (std::map<std::string, TTF_Font*>::const_iterator i = font_lib.begin(); i != font_lib.end(); ++i)
        TTF_CloseFont(i->second);

    if(printing) std::cout << "successfully closed!" << std::endl;

    //Clear renderer
    //
    if(printing) std::cout << "Clearing renderer........";
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    if(printing) std::cout << "successfully closed!" << std::endl;
    ////ENDING SDL ACTIVITIES
    //
    if(printing) std::cout << "Ending SDL activities........" << std::endl;
    //
    if(printing) std::cout << "Quitting SDL resources........" << std::endl;
    Mix_Quit();
    if(printing) std::cout << "Mixer successfully closed!" << std::endl;
    TTF_Quit();
    if(printing) std::cout << "TTF successfully closed!" << std::endl;
    IMG_Quit();
    if(printing) std::cout << "IMG successfully closed!" << std::endl;

    if(printing) std::cout << "Quitting SDL........";
    SDL_Quit();
    if(printing) std::cout << " successfully closed!" << std::endl;

    is_closed = true;
}

void main_controller::set_cursor(int new_cursor_setting){
    cursor_setting = new_cursor_setting;

    if (rooms != NULL)
        SDL_ShowCursor(cursor_setting);
}

void main_controller::set_render_setting(int new_render_setting){
    renderer_setting = new_render_setting;

    if (rooms != NULL){
        //CLEARING RENDERER FIRST
        window->set_renderer(NULL);
        SDL_DestroyRenderer(renderer);

        renderer = window->create_renderer(renderer_setting);
        rooms->set_renderer(renderer);
    }
}

int main_controller::loop_run(){
    ///****GAME LOOP (UNTIL GAME QUITS)
    //
    if (!quit){
        ///PRE EVENT LOOP STUFF
        //
        avg_FPS = counted_frames / (main_timer.get_ticks()/1000.f);

        if (avg_FPS > 2000.0)
            avg_FPS = 0.0;

        //STARTING CAP TIMER
        //
        cap_timer.play();

        //TIMER CONTROLLER (IN CASE PLAYING OR PAUSED)
        //
        if (pause_play)
            main_timer.pause();
        else if (timer_play)
            main_timer.play();
        else
            main_timer.stop();

        playing = main_timer.get_playing();
        paused = main_timer.get_paused();


        while (SDL_PollEvent(&e) != 0){

            ///OLD EVENT LOOP HERE
            //
            /*

            //QUIT EVENT
            //
            if (e.type == SDL_QUIT)
                quit = true;

            ///KEY EVENTS TO PUT INTO QUEUE
            //
            const Uint8* current_key_state = SDL_GetKeyboardState(NULL);

            //KEY DOWN READER
            if (!key_bounce && e.type == SDL_KEYDOWN){
                //MOVE KEY STATES TO KEY EVENT CHECKER TO PROCESS
                //
                //std::cout << "key down" << std::endl;

                for (int k = 0; k < int(key_procedures.size()); k++){    //
                    //std::cout << (int)current_key_state[(key_procedures.at(k).get_input_ID())] << " ";

                    ///FINDING PREVIOUS KEY DOWN IN CASE OF REDUNDANCY
                    if (current_key_state[abs(key_procedures.at(k).get_input_ID())] && (std::find(key_down_events.begin(), key_down_events.end(), abs(key_procedures.at(k).get_input_ID())) == key_down_events.end())){
                        //PUTTING THE KEY IN THE RESPONSE QUEUE
                        //
                        //std::cout << "Key pressed value: " << k << std::endl;
                        room_event_list.push(abs(key_procedures.at(k).get_input_ID()));
                        key_down_events.push_back(abs(key_procedures.at(k).get_input_ID()));     //TRACKING KEY DOWNS AND REDUNDANCY
                    }

                    key_bounce = true;      //Might remove this later
                }
            }

            //KEY UP READER
            if (e.type == SDL_KEYUP){
                //MOVE KEY STATES TO KEY EVENT CHECKER TO PROCESS       (KEY UP VALUES HAVE A NEGATIVE MASK TO BE INDICATED AS KEY UP STATES)
                //
                ///NOTE: KEY UP KEYS HAVE THE SAME VALUE AS UNPRESSED KEYS, SO HAVE TO KEEP TRACK OF WHAT WAS PRESSED BEFORE INSTEAD
                //std::cout << "key up" << std::endl;
                std::vector<int> stored_keys = std::vector<int>();

                ///GO THROUGH PREVIOUSLY PUSHED VALUES
                while (!(key_down_events.empty())){
                    //std::cout << (int)current_key_state[(key_procedures.at(k).get_input_ID())] << " ";

                    //if (key_procedures.at(k).get_input_ID() < 0)
                    ///CHECK IF KEY IN QUESTION NOT PUSHED
                    //    if (!current_key_state[key_down_events.back()]){
                    ///SAVE IF STILL PUSHED

                    if (!current_key_state[key_down_events.back()])
                        room_event_list.push(-key_down_events.back());
                    else
                        stored_keys.push_back(key_down_events.back());
                        key_down_events.pop_back();

                        //}

                     //   }
                    //key_bounce = true;


                }

                if (!stored_keys.empty())
                    key_down_events.swap(stored_keys);
            }

            event_key_loop();


            ///MOVE MOUSE STATES TO MOUSE EVENT CHECKER TO PROCESS (ROOM EVENT QUEUE SHOULD BE CLEARED FROM READING)
            //
            room_event_list = rooms->room_listen(e);
            event_mouse_loop();

            //CLEARING ANY PREVIOUS INPUT
            //
            while (!(room_event_list.empty()))
                room_event_list.pop();
                */


            ///NEW EVENT LOOP

            //CHECKING KEY STATES
            const Uint8* current_key_state = SDL_GetKeyboardState(NULL);

            //CHECKING KEY PRESSES
            switch (e.type){
                case SDL_QUIT:
                    quit = true;
                break;

                case SDL_KEYDOWN:
                    add_to_key_down_queue(e.key.keysym.scancode, e.key.repeat != 0);
                break;

                case SDL_KEYUP:
                    add_to_key_up_queue(e.key.keysym.scancode, e.key.repeat != 0);
                break;
            }

            //UPDATING ALL KEY PROCEDURES
            key_state_procedures_update(current_key_state);
            key_down_procedures_update();
            key_up_procedures_update();

            //CHECKING MOUSE
            mouse_state_update();

            room_event_list = rooms->room_listen(e);
            event_mouse_update();
        }

        ///RENDERING CURRENT ROOM AND MOUSE BEHAVIORS
        //
        pre_render_update();
        rooms->room_render(0, 0, screen_width, screen_height);
        post_render_update();
        SDL_RenderPresent(renderer);

        //CHECKING AND MANAGING FPS
        //
        if (!(main_timer.get_paused()) && (main_timer.get_playing()))
            ++counted_frames;

        if (MAX_TICKS < int(cap_timer.get_ticks())){
            //std::cout << "WAITING FOR COUNTER TO CATCH UP BY:" << (cap_timer.get_ticks() - MAX_TICKS) << std::endl;
            SDL_Delay(cap_timer.get_ticks() - MAX_TICKS);
        }

        //CLEARING KEY BOUNCE
        //
        if (key_bounce)
        key_bounce = false;

        previous_ticks =  main_timer.get_ticks();
    }

    //std::cout << "Is quitting: " << quit << std::endl;

    ///-1 TICKS SIGNIFIES QUITTING THE GAME
    if (quit){
        previous_ticks = -1;
        postloop_update();
    }

    return previous_ticks;
}

int main_controller::new_room(){
    if (!is_closed)
        //RETURN THE ROOM NUMBER AFTER CREATION
        return rooms->create_room();

    return -1;
}

int main_controller::new_panel(int room_to, int x, int y, int w, int h, int full_w, int full_h){
    if (!is_closed && rooms->room_get(room_to) != NULL)
        //RETURN THE PANEL INDEX CREATED
        return rooms->create_panel(room_to, x, y, w, h, full_w, full_h);

    return -1;
}

int main_controller::new_panel(int room_to, int x, int y, int w, int h){
    return rooms->create_panel(room_to, x, y, w, h, w, h);
}

int main_controller::set_room(int new_room){
    int returning_room = -1;

    if (!is_closed){
        //RETURN THE NEW SET ROOM
        rooms->set_room_display(new_room);
        returning_room = rooms->get_room_display();

        //ROOM CHECKER AND LOADER (IN CASE ROOM CHANGED)
        //
        if (rooms->room_checker()){
            loop_room_end();
            loop_room_start();
        } else {
            returning_room = -1;
        }
    }

    return returning_room;
}

point main_controller::get_camera_position(int room_to, int panel_to){
    point return_camera_position = point(-1,-1);
    panel_interface* panel_obs = rooms->interface_get(room_to, panel_to);

    if (!is_closed){
        return_camera_position = panel_obs->camera_get_position();
    }

    panel_obs = NULL;

    return return_camera_position;
}

SDL_Rect main_controller::get_camera_rectangle(int room_to, int panel_to){
    SDL_Rect return_camera_spot = {-1,-1,-1,-1};
    panel_interface* panel_obs = rooms->interface_get(room_to, panel_to);

    if (!is_closed && panel_obs != NULL){
        return_camera_spot = panel_obs->camera_get_rect();
    }

    panel_obs = NULL;

    return return_camera_spot;
}

void main_controller::set_camera_position(int room_to, int panel_to, int x, int y){
    panel_interface* panel_obs = rooms->interface_get(room_to, panel_to);

    if (!is_closed && panel_obs != NULL){
        //SET THE CAMERA
        panel_obs->camera_update(x, y);
    }

    panel_obs = NULL;
}

int main_controller::point_to_room(int new_room){
    if (new_room == -1){
        room_index = -1;
        room_point = NULL;
    } else {
        room_index = new_room;

        if ((room_point = rooms->room_get(new_room)) == NULL){
            room_index = -1;
        }
    }

    return room_index;
}

int main_controller::point_to_panel(int new_panel){
    if (new_panel == -1){
        panel_index = -1;
        panel_point = NULL;
    } else {
        panel_index = new_panel;

        if ((panel_point = rooms->interface_get(room_index, panel_index)) == NULL){
            panel_index = -1;
        }
    }

    return panel_index;
}

//IMAGE SOURCE INITIALIZER
image_source_point* main_controller::get_image_source_point(std::string source){
    try {
        return image_sources.at(source);
    } catch (std::out_of_range e) {
        return NULL;
    }
}

std::string main_controller::new_image_source(std::string source_name, std::string image_path, int room_index, int panel_index){
    room* temp_room_point = NULL;
    panel_interface* temp_panel_point = NULL;

    bool source_initiated = false;

    //CHECK IF KEY NAME NOT REGISTERED AND GAME INITIATED
    if (get_image_source_point(source_name) == NULL && !is_closed){
        if ((temp_panel_point = rooms->interface_get(room_index, panel_index)) != NULL){
            if (printing) std::cout << "loading image source of " << source_name << " in panel " << panel_index << " of room " << room_index << std::endl;

            image_sources.insert(std::pair<std::string, image_source_point*>(source_name, new image_source_point(
                                           sources->visual_media_new(visual_media(0xFF, 0xFF, 0xFF, 0xFF, total_images)))));

            total_images++;

        } else if ((temp_room_point = rooms->room_get(room_index)) != NULL) {
            if (printing) std::cout << "loading image source of " << source_name << " in room " << room_index << std::endl;

            image_sources.insert(std::pair<std::string, image_source_point*>(source_name, new image_source_point(
                                           sources->visual_media_new(visual_media(0xFF, 0xFF, 0xFF, 0xFF, total_images)))));

            total_images++;
        }

        temp_room_point = NULL;
        temp_panel_point = NULL;
    }

    if (source_initiated)
        return source_name;
    else
        return "";
}

std::string main_controller::new_image_source(std::string source_name, std::string image_path, int room_index){
    return new_image_source(source_name, image_path, room_index, -1);
}

//QUICK IMAGE INITIALIZER
std::string main_controller::new_image(std::string image_name, std::string image_path, SDL_Rect clip_rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    int visual_media_point = -1;
    int render_spot_point = -1;

    bool image_initiated = false;

    //CHECK IF GAME HAS BEEN INITIATED AND KEY NAME IS NOT REGISTERED YET
    if (get_image_point(image_name) == NULL && !is_closed){
        if (panel_point != NULL){
            if (printing) std::cout << "loading image of " << image_name << " in panel " << panel_index << " of room " << room_index << std::endl;

            //CREATING A NEW IMAGE POINT AT A PANEL
            images.insert(std::make_pair(image_name, new image_point(
                                         (visual_media_point = sources->visual_media_new(visual_media(red, green, blue, alpha, total_images))), //CREATING IMAGE
                                         room_index, panel_index,
                                         render_spot_point = panel_point->visual_media_spot_new(rendering_bundle(visual_media_point, {clip_rect.x, clip_rect.y, clip_rect.w, clip_rect.h} , 0.0, SDL_FLIP_NONE, total_images, total_images)), //CREATING CLIP
                                         total_images)));

            //CREATING A NEW IMAGE SOURCE (IN CASE NO PANEL SPOTS)
            image_sources.insert(std::make_pair(image_name, new image_source_point(
                                        visual_media_point)));

            if (!sources->visual_media_load_as_image(visual_media_point, image_path.c_str())){
                if (printing) std::cout << "failed to load image!" << std::endl;
                image_initiated = false;
            }

            //SETTING RENDER SPOT SIZE TO IMAGE'S SIZE IF NEED-BE
            if (clip_rect.x < 0 || clip_rect.y < 0 || clip_rect.w < 1 || clip_rect.h < 1)
                panel_point->visual_media_spot_set(render_spot_point, {0,0,sources->visual_media_get_x_size(visual_media_point),sources->visual_media_get_y_size(visual_media_point)});

            //sort_and_repoint_images(room_index, panel_index, room_point, panel_point);

            //INCREASING ELEMENT ID INDEX
            total_images++;

            image_initiated = true;
        } else if (room_point != NULL){
            if (printing) std::cout << "loading image of " << image_name << " in room " << room_index << std::endl;

            //CREATING A NEW IMAGE POINT AT A ROOM
            images.insert(std::make_pair(image_name, new image_point(
                                         (visual_media_point = sources->visual_media_new(visual_media(red, green, blue, alpha, total_images))), //CREATING IMAGE
                                         room_index, -1,
                                         render_spot_point = room_point->visual_media_spot_new(rendering_bundle(visual_media_point, {clip_rect.x, clip_rect.y, clip_rect.w, clip_rect.h} , 0.0, SDL_FLIP_NONE, total_images, total_images)), //CREATING CLIP
                                         total_images)));

            //CREATING A NEW IMAGE SOURCE (IN CASE NO PANEL SPOTS)
            image_sources.insert(std::make_pair(image_name, new image_source_point(
                                        visual_media_point)));

            sources->visual_media_load_as_image(visual_media_point, image_path.c_str());

            //SETTING RENDER SPOT SIZE TO IMAGE'S SIZE IF NEED-BE
            if (clip_rect.x < 0 || clip_rect.y < 0 || clip_rect.w < 1 || clip_rect.h < 1)
                room_point->visual_media_spot_set(render_spot_point, {0,0,sources->visual_media_get_x_size(visual_media_point),sources->visual_media_get_y_size(visual_media_point)});

            //sort_and_repoint_images(room_index, panel_index, room_point, panel_point);

            //INCREASING ID INDEX
            total_images++;

            image_initiated = true;
        }
    }

    if (image_initiated){
        if (printing) std::cout << "image loaded at source " << visual_media_point << std::endl;

        return image_name;
    }else
        return "";
}

std::string main_controller::new_image(std::string image_name, std::string image_path, SDL_Rect clip_rect){
    return new_image(image_name, image_path, clip_rect, 0xFF, 0xFF, 0xFF, 0xFF);
}

std::string main_controller::new_image(std::string image_name, std::string image_path, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    return new_image(image_name, image_path, {0,0,-1,-1}, red, green, blue, alpha);
}

std::string main_controller::new_image(std::string image_name, std::string image_path){
    return new_image(image_name, image_path, {0,0,-1,-1});
}

//ABOUT THE SAME AS LOADING AN IMAGE, BUT NOW LOADING A BLANK IMAGE
std::string main_controller::new_blank_image(std::string image_name, SDL_TextureAccess access_type, int x_size, int y_size, SDL_Rect clip_rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    int visual_media_point = -1;

    bool image_initiated = false;

    //CHECK IF GAME HAS BEEN INITIATED AND KEY NAME IS NOT REGISTERED YET
    if (get_image_point(image_name) == NULL && !is_closed){
        if (panel_point != NULL){
            if (printing) std::cout << "loading blank image of " << image_name << " in panel " << panel_index << " of room " << room_index << std::endl;

            //CREATING A NEW IMAGE POINT AT A PANEL
            images.insert(std::make_pair(image_name, new image_point(
                                         (visual_media_point = sources->visual_media_new(visual_media(red, green, blue, alpha, total_images))), //CREATING IMAGE
                                         room_index, panel_index,
                                         panel_point->visual_media_spot_new(rendering_bundle(visual_media_point, {clip_rect.x, clip_rect.y, clip_rect.w, clip_rect.h} , 0.0, SDL_FLIP_NONE, total_images, total_images)), //CREATING CLIP
                                         total_images)));

            //CREATING A NEW IMAGE SOURCE (IN CASE NO PANEL SPOTS)
            image_sources.insert(std::make_pair(image_name, new image_source_point(
                                        visual_media_point)));

            sources->visual_media_set_x_size(visual_media_point, x_size);
            sources->visual_media_set_y_size(visual_media_point, y_size);
            sources->load_as_blank(visual_media_point, access_type);

            //sort_and_repoint_images(room_index, panel_index, room_point, panel_point);

            //INCREASING ELEMENT ID INDEX
            total_images++;

            image_initiated = true;
        } else if (room_point != NULL){
            if (printing) std::cout << "loading blank image of " << image_name << " in room " << room_index << std::endl;

            //CREATING A NEW IMAGE POINT AT A ROOM
            images.insert(std::make_pair(image_name, new image_point(
                                         (visual_media_point = sources->visual_media_new(visual_media(red, green, blue, alpha, total_images))), //CREATING IMAGE
                                         room_index, -1,
                                         room_point->visual_media_spot_new(rendering_bundle(visual_media_point, {clip_rect.x, clip_rect.y, clip_rect.w, clip_rect.h} , 0.0, SDL_FLIP_NONE, total_images, total_images)), //CREATING CLIP
                                         total_images)));

            //CREATING A NEW IMAGE SOURCE (IN CASE NO PANEL SPOTS)
            image_sources.insert(std::make_pair(image_name, new image_source_point(
                                        visual_media_point)));

            sources->visual_media_set_x_size(visual_media_point, x_size);
            sources->visual_media_set_y_size(visual_media_point, y_size);
            sources->load_as_blank(visual_media_point, access_type);

            //sort_and_repoint_images(room_index, panel_index, room_point, panel_point);

            //INCREASING ID INDEX
            total_images++;

            image_initiated = true;
        }
    }

    if (image_initiated)
        return image_name;
    else
        return "";
}

std::string main_controller::new_blank_image(std::string image_name, SDL_TextureAccess access_type, int x_size, int y_size, SDL_Rect clip_rect){
    return new_blank_image(image_name, access_type, x_size, y_size, {0, 0, x_size, y_size}, 0xFF, 0xFF, 0xFF, 0xFF);
}

std::string main_controller::new_blank_image(std::string image_name, SDL_TextureAccess access_type, int x_size, int y_size, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    return new_blank_image(image_name, access_type, x_size, y_size, {0, 0, x_size, y_size}, red, green, blue, alpha);
}

std::string main_controller::new_blank_image(std::string image_name, SDL_TextureAccess access_type, int x_size, int y_size){
    return new_blank_image(image_name, access_type, x_size, y_size, {0, 0, x_size, y_size});
}

//TEXT LOAD
TTF_Font* main_controller::get_font(std::string font_name){
    try {
        return font_lib.at(font_name);
    } catch (std::out_of_range e) {
        return NULL;
    }
}

std::string main_controller::new_text(std::string image_name, std::string font, std::string text_contents, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool is_wrapping, Uint32 wrap_count){
    int visual_media_point = -1;
    int spot_point = -1;
    TTF_Font* font_to_use = get_font(font);

    bool image_initiated = false;

    //CHECK IF GAME HAS BEEN INITIATED AND KEY NAME IS NOT REGISTERED YET
    if (get_image_point(image_name) == NULL && !is_closed && font_to_use){
        if (panel_point != NULL){
            if (printing) std::cout << "loading text of " << image_name << " in panel " << panel_index << " of room " << room_index << std::endl;

            //CREATING A NEW IMAGE POINT AT A PANEL
            images.insert(std::make_pair(image_name, new image_point(
                                         (visual_media_point = sources->visual_media_new(visual_media(red, green, blue, alpha, total_images))), //CREATING IMAGE
                                         room_index, panel_index,
                                         spot_point = panel_point->visual_media_spot_new(rendering_bundle(visual_media_point, {0,0,1,1} , 0.0, SDL_FLIP_NONE, total_images, total_images)), //CREATING CLIP
                                         total_images)));

            //CREATING A NEW IMAGE SOURCE (IN CASE NO PANEL SPOTS)
            image_sources.insert(std::make_pair(image_name, new image_source_point(
                                        visual_media_point)));

            sources->visual_media_load_as_text(visual_media_point, font_to_use, text_contents.c_str(), is_wrapping, wrap_count);

            //ADJUST VISUAL MEDIA SPOT SIZE
            panel_point->visual_media_spot_set(spot_point,{0,0,sources->visual_media_get_x_size(visual_media_point),sources->visual_media_get_y_size(visual_media_point)});

            //sort_and_repoint_images(room_index, panel_index, room_point, panel_point);

            //INCREASING ELEMENT ID INDEX
            total_images++;

            image_initiated = true;
        } else if (room_point != NULL){
            if (printing) std::cout << "loading text of " << image_name << " in room " << room_index << std::endl;

            //CREATING A NEW IMAGE POINT AT A ROOM
            images.insert(std::make_pair(image_name, new image_point(
                                         (visual_media_point = sources->visual_media_new(visual_media(red, green, blue, alpha, total_images))), //CREATING IMAGE
                                         room_index, -1,
                                         spot_point = room_point->visual_media_spot_new(rendering_bundle(visual_media_point, {0, 0, 1, 1} , 0.0, SDL_FLIP_NONE, total_images, total_images)), //CREATING CLIP
                                         total_images)));

            //CREATING A NEW IMAGE SOURCE (IN CASE NO PANEL SPOTS)
            image_sources.insert(std::make_pair(image_name, new image_source_point(
                                        visual_media_point)));

            sources->visual_media_load_as_text(visual_media_point, font_to_use, text_contents.c_str(), is_wrapping, wrap_count);

            //ADJUST VISUAL MEDIA SPOT SIZE
            room_point->visual_media_spot_set(spot_point,{0,0,sources->visual_media_get_x_size(visual_media_point),sources->visual_media_get_y_size(visual_media_point)});

            //sort_and_repoint_images(room_index, panel_index, room_point, panel_point);

            //INCREASING ELEMENT ID INDEX
            total_images++;

            image_initiated = true;
        }

        set_blend_mode(image_name, SDL_BLENDMODE_BLEND);
    }

    font_to_use = NULL;

    if (image_initiated)
        return image_name;
    else
        //RETURN BLANK IF NO FONT AVAILABLE
        return "";
}

std::string main_controller::new_text(std::string image_name, std::string font, std::string text_contents, bool is_wrapping, Uint32 wrap_count){
    return new_text(image_name, font, text_contents, 0xFF, 0xFF, 0xFF, 0xFF, is_wrapping, wrap_count);
}

//"CREATING" A NEW IMAGE BASED ON A PRE-LOADED ONE
image_point* main_controller::get_image_point(std::string image){
    try {
        return images.at(image);
    } catch (std::out_of_range e) {
        return NULL;
    }
}

std::string main_controller::copy_image(std::string image_name, std::string other_image, SDL_Rect clip_rect){
    image_point* image_to_copy = get_image_point(other_image);
    bool initiation_flag = false;

    if (image_to_copy != NULL && !is_closed && get_image_point(image_name) == NULL){
        if (printing) std::cout << "linking visual media of " << other_image << " to " << image_name << " to source " << image_to_copy->visual_media_point << std::endl;
        room* copy_room_point = rooms->room_get(image_to_copy->render_spot_room_point);
        panel_interface* copy_panel_point = rooms->interface_get(image_to_copy->render_spot_room_point, image_to_copy->render_spot_panel_point);

        if (copy_panel_point != NULL){
            //COPY TO VISUAL MEDIA IN INTERFACE
            if (clip_rect.x < 0 || clip_rect.y < 0 || clip_rect.w < 1 || clip_rect.h < 1){
                SDL_Rect copy_rect = copy_panel_point->visual_media_spot_get(image_to_copy->render_spot_index_point);

                //SET CLIP TO IMAGE TO COPYS CLIP
                images.insert(std::make_pair(image_name, new image_point(
                                             image_to_copy->visual_media_point,
                                             image_to_copy->render_spot_room_point, image_to_copy->render_spot_panel_point,
                                             copy_panel_point->visual_media_spot_new(rendering_bundle(image_to_copy->visual_media_point, copy_rect, 0.0, SDL_FLIP_NONE, total_images, total_images)),
                                             total_images)));
            }else
                images.insert(std::make_pair(image_name, new image_point(
                                             image_to_copy->visual_media_point,
                                             image_to_copy->render_spot_room_point, image_to_copy->render_spot_panel_point,
                                             copy_panel_point->visual_media_spot_new(rendering_bundle(image_to_copy->visual_media_point, clip_rect, 0.0, SDL_FLIP_NONE, total_images, total_images)),
                                             total_images)));

            //sort_and_repoint_images(image_to_copy->visual_media_room_point, image_to_copy->visual_media_index_point, room_point, panel_point);

            total_images++;

            initiation_flag = true;

        } else if (copy_room_point != NULL){
            //COPY TO VISUAL MEDIA IN ROOM
            if (clip_rect.x < 0 || clip_rect.y < 0 || clip_rect.w < 1 || clip_rect.h < 1){
                SDL_Rect copy_rect = copy_room_point->visual_media_spot_get(image_to_copy->render_spot_index_point);

                //SET CLIP TO IMAGE TO COPYS CLIP
                images.insert(std::make_pair(image_name, new image_point(
                                             image_to_copy->visual_media_point,
                                             image_to_copy->render_spot_room_point, image_to_copy->render_spot_panel_point,
                                             copy_room_point->visual_media_spot_new(rendering_bundle(image_to_copy->visual_media_point, copy_rect, 0.0, SDL_FLIP_NONE, 0)),
                                             total_images)));
            }else
                images.insert(std::make_pair(image_name, new image_point(
                                             image_to_copy->visual_media_point,
                                             image_to_copy->render_spot_room_point, image_to_copy->render_spot_panel_point,
                                             copy_room_point->visual_media_spot_new(rendering_bundle(image_to_copy->visual_media_point, clip_rect, 0.0, SDL_FLIP_NONE, 0)),
                                             total_images)));

            //sort_and_repoint_images(image_to_copy->visual_media_room_point, -1, room_point, panel_point);

            total_images++;

            initiation_flag = true;
        }
    }

    image_to_copy = NULL;

    if (initiation_flag)
        return image_name;
     else
        return "";
}

std::string main_controller::copy_image(std::string image_name, std::string other_image){
    return copy_image(image_name, other_image, {-1, -1, -1, -1});
}

std::string main_controller::create_image(std::string image_name, std::string image_source, SDL_Rect clip_rect){
    //CREATION FLAG
    bool create_flag = false;

    //FIND IF THE IMAGE EXISTS
    Image_Point_Map::iterator image_index = images.find(image_name);
    Image_Source_Point_Map::iterator image_source_index = image_sources.find(image_source);

    if (image_index == images.end() && image_source_index != image_sources.end()){
        if (panel_point != NULL){
            //CREATING IMAGE IN PANEL
            images.insert(std::pair<std::string, image_point*>(image_name, new image_point(image_source_index->second->visual_media_point,
                                                                room_index,
                                                                panel_index,
                                                                panel_point->visual_media_spot_new(rendering_bundle(image_source_index->second->visual_media_point, clip_rect, 0.0, SDL_FLIP_NONE, total_images, total_images)),
                                                                total_images)));

            //std::cout << total_images << " " << panel_obs->visual_media_spot_find(total_images) << std::endl;

            //sort_and_repoint_images();

            total_images++;

            create_flag = true;

        } else if (room_point != NULL){
            //CREATING IMAGE IN ROOM
            images.insert(std::pair<std::string, image_point*>(image_name, new image_point(image_source_index->second->visual_media_point,
                                                                room_index, -1,
                                                                room_point->visual_media_spot_new(rendering_bundle(image_source_index->second->visual_media_point, clip_rect, 0.0, SDL_FLIP_NONE, total_images, total_images)),
                                                                total_images)));

            //sort_and_repoint_images(image_source_index->second->visual_media_room_point, -1, room_obs, NULL);

            total_images++;

            create_flag = true;
        }
    }

    if (create_flag)
        return image_name;
    else
        return "";
}

void main_controller::remove_image_copy(std::string image_name){
    //FIND IF THAT IMAGE EXISTS
    Image_Point_Map::iterator image_index = images.find(image_name);

    ///NOTE: THIS ONLY REMOVES THE RENDER SPOT, NOT THE IMAGE SOURCE ITSELF
    if (image_index != images.end()){
        //CLEAR THE SPOT FOR THAT IMAGE FIRST
        room* temp_room_obs = NULL;
        panel_interface* temp_panel_obs = NULL;

        if ((temp_panel_obs = rooms->interface_get(images.at(image_name)->render_spot_room_point, images.at(image_name)->render_spot_panel_point))){
            if (printing){
                std::cout << "Removing image " << image_name << " " << images.at(image_name)->visual_media_point << std::endl;
                std::cout << "               " << image_name << " " << images.at(image_name)->render_spot_room_point << " " << images.at(image_name)->render_spot_panel_point << " " << images.at(image_name)->render_spot_index_point << std::endl;
            }

            temp_panel_obs->visual_media_spot_remove(images.at(image_name)->render_spot_index_point);

            //sort_and_repoint_images(images.at(image_name)->visual_media_room_point, images.at(image_name)->visual_media_panel_point,
            //                    room_obs, panel_obs);

        } else if ((temp_room_obs = rooms->room_get(images.at(image_name)->render_spot_room_point))){
            if (printing){
                std::cout << "Removing image " << image_name << " " << images.at(image_name)->visual_media_point << std::endl;
                std::cout << "               " << image_name << " " << images.at(image_name)->render_spot_room_point << " " << images.at(image_name)->render_spot_index_point << std::endl;
            }

            temp_room_obs->visual_media_spot_remove(images.at(image_name)->render_spot_index_point);

            //sort_and_repoint_images(images.at(image_name)->visual_media_room_point, -1,
            //                    room_obs, NULL);

            if (image_index->second->image_animations != NULL){
                image_index->second->image_animations->set_playing(false);
                delete image_index->second->image_animations;
            }
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;

        images.erase(image_index);
    }
}

std::string main_controller::reload_as_image(std::string image_name, std::string new_image_path, SDL_Rect clip_rect){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    bool reload_flag = true;

    //see if image exists first
    if (image_obs != NULL){
        //check if the image points to a room or a panel and reload as an image (wiping already done in texture loading)

        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            sources->visual_media_load_as_image(image_obs->visual_media_point, new_image_path.c_str());
            temp_panel_obs->visual_media_spot_set(image_obs->visual_media_point, clip_rect);

        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL){
            sources->visual_media_load_as_image(image_obs->visual_media_point, new_image_path.c_str());
            temp_room_obs->visual_media_spot_set(image_obs->visual_media_point, clip_rect);
        }

        temp_panel_obs = NULL;
        temp_room_obs = NULL;
        image_obs = NULL;
    } else {
        reload_flag = false;
    }

    if (reload_flag)
        return image_name;
    else
        return "";
}

std::string main_controller::reload_as_text(std::string image_name, std::string font, std::string new_text, bool is_wrapping, Uint32 wrap_count){
    image_point* image_obs = get_image_point(image_name);
    TTF_Font* font_found = get_font(font);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    bool reload_flag = true;

    //see if image exists first
    if (image_obs != NULL && font_found != NULL){
        //check if the image points to a room or a panel and reload as text
        SDL_Rect text_rect = {0,0,0,0};

        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            sources->visual_media_load_as_text(image_obs->visual_media_point, font_found, new_text.c_str(), is_wrapping, wrap_count);

            text_rect.w = sources->visual_media_get_x_size(image_obs->visual_media_point);
            text_rect.h = sources->visual_media_get_y_size(image_obs->visual_media_point);

            temp_panel_obs->visual_media_spot_set(image_obs->render_spot_index_point, text_rect);

        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL){
            sources->visual_media_load_as_text(image_obs->visual_media_point, font_found, new_text.c_str(), is_wrapping, wrap_count);

            text_rect.w = sources->visual_media_get_x_size(image_obs->visual_media_point);
            text_rect.h = sources->visual_media_get_y_size(image_obs->visual_media_point);

            temp_room_obs->visual_media_spot_set(image_obs->render_spot_index_point, text_rect);
        }

        set_blend_mode(image_name, SDL_BLENDMODE_BLEND);

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    } else {
        reload_flag = false;
    }

    font_found = NULL;
    image_obs = NULL;

    if (reload_flag)
        return image_name;
    else
        return "";
}

std::string main_controller::reload_as_blank(std::string image_name, SDL_TextureAccess access_type, int x_size, int y_size){
    image_point* image_obs = get_image_point(image_name);
    bool reload_flag = true;

    //see if image exists first
    if (image_obs != NULL){
        //check if the image points to a room or a panel and reload as a blank image
        sources->visual_media_set_x_size(image_obs->visual_media_point, x_size);
        sources->visual_media_set_y_size(image_obs->visual_media_point, y_size);
        sources->load_as_blank(image_obs->visual_media_point, access_type);

    } else {
        reload_flag = false;
    }

    if (reload_flag)
        return image_name;
    else
        return "";
}

bool main_controller::get_active(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    bool active = false;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            active = temp_panel_obs->visual_media_get_active(image_obs->render_spot_index_point);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            active = temp_room_obs->visual_media_get_active(image_obs->render_spot_index_point);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return active;
}

bool main_controller::set_active(std::string image_name, bool active){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    bool return_active = false;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            return_active = temp_panel_obs->visual_media_set_active(image_obs->render_spot_index_point, active);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            return_active = temp_room_obs->visual_media_set_active(image_obs->render_spot_index_point, active);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return return_active;
}

int main_controller::get_image_x_size(std::string image_source_name){
    image_source_point* source_obs = get_image_source_point(image_source_name);
    int return_x = -1;

    if (source_obs != NULL && !is_closed){
        return_x = sources->visual_media_get_x_size(source_obs->visual_media_point);
    }

    return return_x;
}

int main_controller::get_image_y_size(std::string image_source_name){
    image_source_point* source_obs = get_image_source_point(image_source_name);
    int return_y = -1;

    if (source_obs != NULL && !is_closed){
        return_y = sources->visual_media_get_y_size(source_obs->visual_media_point);
    }

    return return_y;
}

SDL_Rect main_controller::get_spot(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    SDL_Rect return_spot = {-1,-1,-1,-1};

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            return_spot = temp_panel_obs->visual_media_spot_get(image_obs->render_spot_index_point);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            return_spot = temp_room_obs->visual_media_spot_get(image_obs->render_spot_index_point);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return return_spot;
}

void main_controller::set_spot(std::string image_name, SDL_Rect new_spot){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            temp_panel_obs->visual_media_spot_set(image_obs->render_spot_index_point, new_spot);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            temp_room_obs->visual_media_spot_set(image_obs->render_spot_index_point, new_spot);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;
}

int main_controller::add_point(std::string image_name, int x, int y){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    int point_index = -1;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            point_index = temp_panel_obs->visual_media_point_add(image_obs->render_spot_index_point, x, y);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            point_index = temp_room_obs->visual_media_point_add(image_obs->render_spot_index_point, x, y);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return point_index;
}

point* main_controller::get_point(std::string image_name, int point_index){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    point* return_point = NULL;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            return_point = temp_panel_obs->visual_media_point_get(image_obs->render_spot_index_point, point_index);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            return_point = temp_room_obs->visual_media_point_get(image_obs->render_spot_index_point);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return return_point;
}

void main_controller::set_point(std::string image_name, int point_index, int new_x, int new_y){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            temp_panel_obs->visual_media_point_get(image_obs->render_spot_index_point)->set_x(new_x);
            temp_panel_obs->visual_media_point_get(image_obs->render_spot_index_point)->set_y(new_y);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            temp_room_obs->visual_media_point_get(image_obs->render_spot_index_point)->set_x(new_x);
            temp_room_obs->visual_media_point_get(image_obs->render_spot_index_point)->set_y(new_y);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;
}

void main_controller::clear_points(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            temp_panel_obs->visual_media_point_clear(image_obs->render_spot_index_point);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            temp_room_obs->visual_media_point_clear(image_obs->render_spot_index_point);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;
}

void main_controller::sort_and_repoint_images(){
    ///***THIS IS SUPPOSING THAT THE ROOM OR PANEL POINT WAS ALREADY FOUND AND UNCHANGED
    if (panel_point != NULL){
        //SORT AND REPOINT THAT PANEL
        panel_point->visual_media_sort();

        //REPOINT
        //CHECK IF INDEX OF A MAP IS UNDER THIS PANEL
        for (Image_Point_Map::iterator i = images.begin(); i != images.end(); ++i)
            if (i->second->render_spot_room_point == room_index && i->second->render_spot_panel_point == panel_index){
                //FIND AND SET INDEX OF THE SORTED SPOT TO THE IMAGE
                i->second->render_spot_index_point = panel_point->visual_media_spot_find(i->second->element_ID);
            }
    } else if (room_point != NULL){
        //SORT AND REPOINT THAT ROOM
        room_point->visual_media_sort();

        //REPOINT
        //CHECK IF INDEX OF A MAP IS UNDER THIS ROOM
        for (Image_Point_Map::iterator i = images.begin(); i != images.end(); ++i)
            if (i->second->render_spot_room_point == room_index)
                //FIND AND SET INDEX OF THE SORTED SPOT TO THE IMAGE
                i->second->render_spot_index_point = room_point->visual_media_spot_find(i->second->element_ID);
    }
}

int main_controller::get_layer(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    int return_layer = -1;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            return_layer = temp_panel_obs->visual_media_get_rendering_layer(image_obs->render_spot_index_point);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            return_layer = temp_room_obs->visual_media_get_rendering_layer(image_obs->render_spot_index_point);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return return_layer;
}

void main_controller::set_layer(std::string image_name, int new_layer){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //SET LAYER, SORT, REPOINT FROM PANEL FOUND
            temp_panel_obs->visual_media_set_rendering_layer(image_obs->render_spot_index_point, new_layer);
            //sort_and_repoint_images(image_obs->render_spot_room_point, image_obs->render_spot_panel_point, NULL, panel_obs);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //SET LAYER, SORT, REPOINT FROM PANEL FOUND
            temp_room_obs->visual_media_set_rendering_layer(image_obs->render_spot_index_point, new_layer);
            //sort_and_repoint_images(image_obs->render_spot_room_point, image_obs->render_spot_panel_point, room_obs, NULL);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;
}

int main_controller::get_flip(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    int return_flip = SDL_FLIP_NONE;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            return_flip = temp_panel_obs->visual_media_get_flip(image_obs->render_spot_index_point);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            return_flip = temp_room_obs->visual_media_get_flip(image_obs->render_spot_index_point);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return return_flip;
}

void main_controller::set_flip(std::string image_name, int new_flip){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            temp_panel_obs->visual_media_set_flip(image_obs->render_spot_index_point, SDL_RendererFlip(new_flip));
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            temp_room_obs->visual_media_set_flip(image_obs->render_spot_index_point, SDL_RendererFlip(new_flip));
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;
}

double main_controller::get_rotation(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;
    double return_rotation = 0.0;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            return_rotation = temp_panel_obs->visual_media_get_rotation(image_obs->render_spot_index_point);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            return_rotation = temp_room_obs->visual_media_get_rotation(image_obs->render_spot_index_point);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;

    return return_rotation;
}

void main_controller::set_rotation(std::string image_name, double new_rotation){
    image_point* image_obs = get_image_point(image_name);
    room* temp_room_obs = NULL;
    panel_interface* temp_panel_obs = NULL;

    if (image_obs != NULL && !is_closed){
        if ((temp_panel_obs = rooms->interface_get(image_obs->render_spot_room_point, image_obs->render_spot_panel_point)) != NULL){
            //RETURN FROM PANEL FOUND
            temp_panel_obs->visual_media_set_rotation(image_obs->render_spot_index_point, new_rotation);
        } else if ((temp_room_obs = rooms->room_get(image_obs->render_spot_room_point)) != NULL) {
            //RETURN FROM ROOM FOUND
            temp_room_obs->visual_media_set_rotation(image_obs->render_spot_index_point, new_rotation);
        }

        temp_room_obs = NULL;
        temp_panel_obs = NULL;
    }

    image_obs = NULL;
}

SDL_Color main_controller::get_color(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    SDL_Color return_color = {0x00,0x00,0x00,0x00};

    if (image_obs != NULL && !is_closed){
        return_color = sources->visual_media_get_color(image_obs->visual_media_point);
    }

    image_obs = NULL;

    return return_color;
}

void main_controller::set_color(std::string image_name, SDL_Color new_color){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_color(image_obs->visual_media_point, new_color.r, new_color.g, new_color.b, new_color.a);
    }

    image_obs = NULL;
}

Uint8 main_controller::get_red(std::string image_name){
    image_point* image_obs = get_image_point(image_name);

    Uint8 return_red = 0x00;

    if (image_obs != NULL && !is_closed){
        return_red = sources->visual_media_get_red(image_obs->visual_media_point);
    }

    image_obs = NULL;

    return return_red;
}

void main_controller::set_red(std::string image_name, Uint8 new_red){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_red(image_obs->visual_media_point, new_red);
    }

    image_obs = NULL;
}

Uint8 main_controller::get_green(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    Uint8 return_green = 0x00;

    if (image_obs != NULL && !is_closed){
        return_green = sources->visual_media_get_green(image_obs->visual_media_point);
    }

    image_obs = NULL;

    return return_green;
}

void main_controller::set_green(std::string image_name, Uint8 new_green){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_green(image_obs->visual_media_point, new_green);
    }

    image_obs = NULL;
}

Uint8 main_controller::get_blue(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    Uint8 return_blue = 0x00;

    if (image_obs != NULL && !is_closed){
        return_blue = sources->visual_media_get_blue(image_obs->visual_media_point);
    }

    image_obs = NULL;

    return return_blue;
}

void main_controller::set_blue(std::string image_name, Uint8 new_blue){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_blue(image_obs->visual_media_point, new_blue);
    }

    image_obs = NULL;
}

Uint8 main_controller::get_alpha(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    Uint8 return_alpha = 0x00;

    if (image_obs != NULL && !is_closed){
        return_alpha = sources->visual_media_get_alpha(image_obs->visual_media_point);
    }

    image_obs = NULL;

    return return_alpha;
}

void main_controller::set_alpha(std::string image_name, Uint8 new_alpha){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_alpha(image_obs->visual_media_point, new_alpha);
    }

    image_obs = NULL;
}

void main_controller::set_blend_mode(std::string image_name, SDL_BlendMode blend_mode){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_blend_mode(image_obs->visual_media_point, blend_mode);
    }

    image_obs = NULL;
}

bool main_controller::lock_image(std::string image_name, bool lock){
    image_point* image_obs = get_image_point(image_name);
    bool return_lock = false;

    if (image_obs != NULL && !is_closed){
        return_lock = sources->visual_media_set_locked(image_obs->visual_media_point, lock);
    }

    image_obs = NULL;

    return return_lock;
}

SDL_Rect main_controller::get_lock_rect(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    SDL_Rect return_rect = {-1,-1,-1,-1};

    if (image_obs != NULL && !is_closed){
        return_rect = sources->visual_media_get_lock_rect(image_obs->visual_media_point);
    }

    image_obs = NULL;

    return return_rect;
}

void main_controller::set_lock_rect(std::string image_name, SDL_Rect new_lock_rect){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_lock_rect(image_obs->visual_media_point, new_lock_rect);
    }

    image_obs = NULL;
}

void* main_controller::get_pixels(std::string image_name){
    image_point* image_obs = get_image_point(image_name);
    void* return_pixels = NULL;

    if (image_obs != NULL && !is_closed){
        return_pixels = sources->visual_media_get_pixels(image_obs->visual_media_point);
    }

    image_obs = NULL;

    return return_pixels;
}

Uint32 main_controller::get_pixel(std::string image_name, int x, int y){
    image_point* image_obs = get_image_point(image_name);
    Uint32 return_pixel = 0x00000000;

    if (image_obs != NULL && !is_closed){
        return_pixel = sources->visual_media_get_pixel(image_obs->visual_media_point, x, y);
    }

    image_obs = NULL;

    return return_pixel;
}

void main_controller::set_pixel(std::string image_name, Uint32 pixel, int x, int y){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_pixel(image_obs->visual_media_point, pixel, x, y);
    }

    image_obs = NULL;
}

void main_controller::set_pixels(std::string image_name, Uint32* pixels){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_pixels(image_obs->visual_media_point, pixels);
    }

    image_obs = NULL;
}

void main_controller::set_as_render_target(std::string image_name, int room_to_copy, int panel_to_copy){
    image_point* image_obs = get_image_point(image_name);
    room* room_to_copy_obs = NULL;
    panel_interface* panel_to_copy_obs = NULL;

    if (image_obs != NULL && !is_closed){
        sources->visual_media_set_renderer_target(image_obs->visual_media_point);

        //GET THE ROOM OR PANEL TO COPY
        if ((panel_to_copy_obs = rooms->interface_get(room_to_copy, panel_to_copy)) != NULL)
            panel_to_copy_obs->render();
        else if ((room_to_copy_obs = rooms->room_get(room_to_copy)) != NULL)
            room_to_copy_obs->render(0,0, screen_width, screen_height);

        //FREE TARGET
        SDL_SetRenderTarget(renderer, NULL);
    }

    image_obs = NULL;
}

image_point* main_controller::get_button(std::string button_name){
    try {
        return buttons.at(button_name);
    } catch (std::out_of_range e){
        return NULL;
    }
}

void main_controller::add_animation_set_to_image(std::string image_name){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        image_obs->image_animations = new animation_set();
    }
}

void main_controller::add_animation_state_to_image(std::string image_name, std::string new_animation_state, bool continuous, bool playback){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        if (image_obs->image_animations != NULL){
            image_obs->image_animations->add_animation_state(new_animation_state, continuous, playback);
        }
    }
}

void main_controller::add_animation_frame_to_image(std::string image_name, std::string animation_state, unsigned int ticks, SDL_Rect animation_rect){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        if (image_obs->image_animations != NULL){
            image_obs->image_animations->add_frame_to_animation(animation_state, ticks, animation_rect);
        }
    }
}

void main_controller::set_animation_state(std::string image_name, std::string animation_state){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        if (image_obs->image_animations != NULL){
            image_obs->image_animations->set_animation_state(animation_state);
        }
    }
}

int main_controller::get_animation_frame_index(std::string image_name, std::string animation_state){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        //SEE IF ANIMATION EXISTS
        if (image_obs->image_animations != NULL){
            return image_obs->image_animations->get_animation_frame_index(animation_state);
        }
    }

    return -1;
}

void main_controller::set_animation_active(std::string image_name, bool animation_play){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        //SEE IF ANIMATION EXISTS
        if (image_obs->image_animations != NULL){
            image_obs->image_animations->set_playing(animation_play);
        }
    }
}

bool main_controller::get_animation_active(std::string image_name){
    image_point* image_obs = get_image_point(image_name);

    if (image_obs != NULL && !is_closed){
        //SEE IF ANIMATION EXISTS
        if (image_obs->image_animations != NULL){
            return image_obs->image_animations->get_playing();
        }
    }

    return false;
}

std::string main_controller::new_button(std::string button_name, std::vector<SDL_Rect> new_rectangles){
    bool button_initiated = false;

    if (!is_closed && panel_point != NULL && get_button(button_name) == NULL){
        buttons.insert(std::make_pair(button_name, new image_point(-1, room_index, panel_index ,panel_point->button_new(button_bundle(total_buttons), new_rectangles), total_buttons)));
        total_buttons++;
        button_initiated = true;
    }

    if (button_initiated)
        return button_name;
    else
        return "";
}

bool main_controller::get_button_active(std::string button_name){
    image_point* button_obs = get_button(button_name);
    panel_interface* temp_panel_obs = NULL;
    bool return_active = false;

    if (!is_closed && button_obs != NULL){
        if ((temp_panel_obs = rooms->interface_get(button_obs->render_spot_room_point,button_obs->render_spot_panel_point)) != NULL)
            return_active = temp_panel_obs->button_get_active(button_obs->render_spot_index_point);

        temp_panel_obs = NULL;
    }

    button_obs = NULL;

    return return_active;
}

bool main_controller::set_button_active(std::string button_name, bool active){
    image_point* button_obs = get_button(button_name);
    panel_interface* temp_panel_obs = NULL;
    bool return_active = false;

    if (!is_closed && button_obs != NULL){
        if ((temp_panel_obs = rooms->interface_get(button_obs->render_spot_room_point,button_obs->render_spot_panel_point)) != NULL)
            return_active = temp_panel_obs->button_set_active(button_obs->render_spot_index_point, active);

        temp_panel_obs = NULL;
    }

    button_obs = NULL;

    return return_active;
}

void main_controller::transform_button(std::string button_name, std::vector<SDL_Rect> new_rectangles){
    image_point* button_obs = get_button(button_name);
    panel_interface* temp_panel_obs = NULL;

    if (!is_closed && button_obs != NULL){
        if ((temp_panel_obs = rooms->interface_get(button_obs->render_spot_room_point,button_obs->render_spot_panel_point)) != NULL)
            temp_panel_obs->button_transform(button_obs->render_spot_index_point, new_rectangles);

        temp_panel_obs = NULL;
    }

    button_obs = NULL;
}

int main_controller::get_button_over_x(std::string button_name){
    image_point* button_obs = get_button(button_name);
    panel_interface* temp_panel_obs = NULL;
    int returning_x = -1;

    if (!is_closed && button_obs != NULL){
        if ((temp_panel_obs = rooms->interface_get(button_obs->render_spot_room_point,button_obs->render_spot_panel_point)) != NULL)
            returning_x = temp_panel_obs->button_get_over_x(button_obs->render_spot_index_point);

        temp_panel_obs = NULL;
    }

    button_obs = NULL;

    return returning_x;
}

int main_controller::get_button_over_y(std::string button_name){
    image_point* button_obs = get_button(button_name);
    panel_interface* temp_panel_obs = NULL;
    int returning_y = -1;

    if (!is_closed && button_obs != NULL){
        if ((temp_panel_obs = rooms->interface_get(button_obs->render_spot_room_point,button_obs->render_spot_panel_point)) != NULL)
            returning_y = temp_panel_obs->button_get_over_y(button_obs->render_spot_index_point);

        temp_panel_obs = NULL;
    }

    button_obs = NULL;

    return returning_y;
}


///MIXER
std::string main_controller::new_sound(std::string sound_name, std::string sound_path, int channel, int loops, Uint8 volume){
    bool new_flag = false;

    //CHECK IF SOUND NAME IS TAKEN AND IF GAME IS INITIATED
    if (get_sound(sound_name) == NULL && !is_closed){
        sounds.insert(std::make_pair(sound_name, new mixer_point(mixer->sound_new(channel, loops, volume, total_sounds), total_sounds)));
        total_sounds++;

        std::cout << "a" << std::endl;
        std::cout << (get_sound(sound_name) == NULL) << std::endl;
        std::cout << "d" << std::endl;
        new_flag = mixer->sound_load(sounds.at(sound_name)->element_ID, sound_path.c_str());

        std::cout << "a" << std::endl;
    }

    if (new_flag)
        return sound_name;
    else
        return "";
}

std::string main_controller::new_music(std::string music_name, std::string music_path, int loops){
    bool new_flag = false;

    //CHECK IF MUSIC NAME IS TAKEN AND IF GAME IS INITIATED
    if (get_music(music_name) == NULL && !is_closed){
        music.insert(std::make_pair(music_name, new mixer_point(mixer->music_new(loops, total_music), total_music)));
        total_music++;

        mixer->music_load(music[music_name]->index, music_path.c_str());
        new_flag = true;
    }

    if (new_flag)
        return music_name;
    else
        return "";
}

mixer_point* main_controller::get_sound(std::string sound_name){
    mixer_point* return_sound = NULL;

    //SEE IF SOUND EXISTS IN MAP
    try {
        return_sound = sounds.at(sound_name);
    } catch (std::out_of_range e){
        return_sound = NULL;
    }

    return return_sound;
}

mixer_point* main_controller::get_music(std::string music_name){
    mixer_point* return_music = NULL;

    //SEE IF MUSIC EXISTS IN MAP
    try {
        return_music = music[music_name];
    } catch (std::out_of_range e){
        return_music = NULL;
    }

    return return_music;
}

bool main_controller::play_sound(std::string sound_name){
    mixer_point* sound_point = NULL;
    bool played = false;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        played = mixer->sound_play(sound_point->index);
    }

    sound_point = NULL;
    return played;
}

bool main_controller::fade_play_sound(std::string sound_name, unsigned int ms){
    mixer_point* sound_point = NULL;
    bool played = false;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        played = mixer->sound_fade_play(sound_point->index, ms);
    }

    sound_point = NULL;
    return played;
}

bool main_controller::get_sound_active(std::string sound_name){
    mixer_point* sound_point = NULL;
    bool active = false;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        active = mixer->sound_get_active(sound_point->index);
    }

    sound_point = NULL;
    return active;
}

bool main_controller::set_sound_active(std::string sound_name, bool active){
    mixer_point* sound_point = NULL;
    bool return_active = false;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        return_active = mixer->sound_set_active(sound_point->index, active);
    }

    sound_point = NULL;
    return return_active;
}

int main_controller::get_sound_channel(std::string sound_name){
    mixer_point* sound_point = NULL;
    int channel_to_return = -1;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        channel_to_return = mixer->sound_get_channel(sound_point->index);
    }

    sound_point = NULL;

    //RETURN POTENTIAL RESULT
    return channel_to_return;
}

void main_controller::set_sound_channel(std::string sound_name, int new_channel){
    mixer_point* sound_point = NULL;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        mixer->sound_set_channel(sound_point->index, new_channel);
    }

    sound_point = NULL;
}

Uint8 main_controller::get_sound_volume(std::string sound_name){
    mixer_point* sound_point = NULL;
    Uint8 return_volume = 0;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        return_volume = mixer->sound_get_channel(sound_point->index);
    }

    sound_point = NULL;

    return return_volume;
}

void main_controller::set_sound_volume(std::string sound_name, Uint8 new_volume){
    mixer_point* sound_point = NULL;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        mixer->sound_set_volume(sound_point->index, new_volume);
    }

    sound_point = NULL;
}

int main_controller::get_sound_loops(std::string sound_name){
    mixer_point* sound_point = NULL;
    int loop_return = -1;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        loop_return = mixer->sound_get_loops(sound_point->index);
    }

    sound_point = NULL;

    return loop_return;
}

void main_controller::set_sound_loops(std::string sound_name, int new_loops){
    mixer_point* sound_point = NULL;

    if ((sound_point = get_sound(sound_name)) != NULL && !is_closed){
        mixer->sound_set_loops(sound_point->index, new_loops);
    }

    sound_point = NULL;
}

bool main_controller::play_music(std::string music_name){
    mixer_point* music_point = NULL;
    int played = false;

    if ((music_point = get_music(music_name)) != NULL && !is_closed){
        played = mixer->music_play(music_point->index);
    }

    music_point = NULL;

    return played;
}

bool main_controller::fade_play_music(std::string music_name, unsigned int ms){
    mixer_point* music_point = NULL;
    int played = false;

    if ((music_point = get_music(music_name)) != NULL && !is_closed){
        played = mixer->music_fade_play(music_point->index, ms);
    }

    music_point = NULL;

    return played;
}

bool main_controller::get_music_active(std::string music_name){
    mixer_point* music_point = NULL;
    int active = false;

    if ((music_point = get_music(music_name)) != NULL && !is_closed){
        active = mixer->music_get_active(music_point->index);
    }

    music_point = NULL;

    return active;
}

bool main_controller::set_music_active(std::string music_name, bool active){
    mixer_point* music_point = NULL;
    int return_active = false;

    if ((music_point = get_music(music_name)) != NULL && !is_closed){
        return_active = mixer->music_set_active(music_point->index, active);
    }

    music_point = NULL;

    return return_active;
}

Uint8 main_controller::get_music_volume(){
    if (!is_closed){
        return mixer->music_get_volume();
    }

    return 0;
}

Uint8 main_controller::set_music_volume(Uint8 new_volume){
    if (!is_closed){
        return mixer->music_set_volume(new_volume);
    }

    return 0;
}

int main_controller::get_music_loops(std::string music_name){
    mixer_point* music_point = NULL;
    int return_loops = -1;

    if ((music_point = get_music(music_name)) != NULL && !is_closed){
        return_loops = mixer->music_get_loops(music_point->index);
    }

    music_point = NULL;

    return return_loops;
}

void main_controller::set_music_loops(std::string music_name, int new_loops){
    mixer_point* music_point = NULL;

    if ((music_point = get_music(music_name)) != NULL && !is_closed){
        mixer->music_set_loops(music_point->index, new_loops);
    }

    music_point = NULL;
}

bool main_controller::get_music_paused(){
    if (!is_closed){
        return mixer->is_paused();
    }

    return false;
}

void main_controller::pause_music(){
    if (!is_closed){
        mixer->pause_music();
    }
}

void main_controller::halt_music(){
    if (!is_closed){
        mixer->halt_music();
    }
}

bool main_controller::fade_halt_music(unsigned int ms){
    if (!is_closed){
        return mixer->fade_halt_music(ms);
    }

    return false;
}

bool main_controller::get_music_playing(){
    if (!is_closed){
        return mixer->is_playing();
    }

    return false;
}

void main_controller::resume_music(){
    if (!is_closed){
        mixer->resume_music();
    }
}

///FONTS
std::string main_controller::add_font(std::string font_name, std::string font_file_path, int font_size){
    if (get_font(font_name) == NULL){
        font_lib.insert(std::make_pair(font_name, TTF_OpenFont(font_file_path.c_str(), font_size)));

        if (!font_lib[font_name]){
            if (printing) std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
            return "";
        } else {
            return font_name;
        }
    } else
        return "";
}

std::string main_controller::get_font_name(int index){
    if (is_within_index(font_lib.size(), index)){
        Font_Map::iterator i = font_lib.begin();
        std::advance(i, index);
        return i->first;
    } else
        return "";
}


///LOOP PROCEDURES (OCCASIONAL CALL)

void main_controller::preloop_initiation(){
    bool preloop_flag = true;

    if(printing) std::cout << "Doing preloop initiation procedures" << std::endl;

    //WHILE THERE ARE THINGS TO QUEUE AND IS NOT EMPTY
    while (!(initialize_queue.empty()) && preloop_flag && !quit){
        //DOING PROCEDURE
        preloop_flag = initialize_queue.front().procedure_do(0, true, true);

        initialize_queue.pop();
    }

    if (!preloop_flag){
        if(printing) std::cout << "Error was found in initiation procedures" << std::endl;
        quit = true;
    }
}

//Goes like:
//preloop stuff
//room starts -> activate room -> activate additional procedures -> ... main loop & event loop stuff ... -> deactivation procedures -> deactivate room
//postloop stuff

void main_controller::loop_room_start(){
    //ACTIVATE NEW CURRENT ROOM
    //
    bool start_proc_flag = rooms->room_request_activation();
    rooms->read_room_start_procedures(playing, paused);

    if (!start_proc_flag)
        if(printing) std::cout << "Current room is invalid" << std::endl;
}

void main_controller::loop_room_end(){
    //DEACTIVATE THE OLD ROOM (ASSUMING CURRENT ROOM HAS ACTUALLY BEEN INITIATED AND REFERRING TO A REAL ROOM ADDRESS)
    //
    bool end_proc_flag = rooms->read_room_end_procedures(playing, paused);
    rooms->room_request_deactivation();

    if (!end_proc_flag)
        if(printing) std::cout << "Previous room is invalid" << std::endl;
}

void main_controller::postloop_update(){
    ///DO LAST PROCEDURES
    bool postloop_flag = true;

    while (!(closing_queue.empty()) && postloop_flag){
        //DOING PROCEDURE
        postloop_flag = closing_queue.front().procedure_do(0, true, true);

        closing_queue.pop();
    }

    if (!postloop_flag){
        if(printing) std::cout << "Error was found in closing procedures" << std::endl;
    }
}

/// MAIN LOOP (MAINLY CALLED)

void main_controller::pre_render_update(){
    int i = 0;
    bool pre_render_flag = rooms->read_room_pre_render_procedures(previous_ticks, playing, paused);

    while (i < int(pre_render_procedures.size()) && pre_render_flag){
        pre_render_flag = pre_render_procedures.at(i).procedure_do(previous_ticks, playing, paused);
        i++;
    }

    animation_update();

    if (!pre_render_flag){
        if(printing) std::cout << "Error was found in pre render update" << std::endl;
        quit = true;
    }
}

void main_controller::animation_update(){
    //CONSIDER WHICH ROOM IS BEING PLAYED
    static int animation_room;
    animation_room = rooms->get_current_room();

    for (Image_Point_Map::iterator image_spot_index = images.begin(); image_spot_index != images.end(); ++image_spot_index){
        if (image_spot_index->second->render_spot_room_point == animation_room && image_spot_index->second->image_animations != NULL){
            //UPDATE AND CHECK FOR FLAG
            if (image_spot_index->second->image_animations->update_animation(previous_ticks)){
                //CHANGING IMAGE SPOT
                set_spot(image_spot_index->first, image_spot_index->second->image_animations->get_frame());
            }
        }
    }
}

void main_controller::post_render_update(){
    int i = 0;
    bool post_render_flag = rooms->read_room_post_render_procedures(previous_ticks, playing, paused);

    while (i < int(post_render_procedures.size()) && post_render_flag){
        post_render_flag = post_render_procedures.at(i).procedure_do(previous_ticks, playing, paused);
        i++;
    }

    if (!post_render_flag){
        if(printing) std::cout << "Error was found in post render update" << std::endl;
        quit = true;
    }
}

///EVENT LOOPS (CALLED BY INPUT)
void main_controller::key_state_procedures_update(const Uint8* key_states){
    unsigned int i = 0;
    int input_to_check = 0;
    bool key_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < key_state_procedures.size() && key_flag){
        input_to_check = key_state_procedures.at(i).get_input_ID();
        key_flag = key_state_procedures.at(i).procedure_do(key_states[input_to_check], playing, paused);


        i++;
    }

    //key_flag = rooms->read_room_key_state_procedures(key_states);

    if (!key_flag){
        if(printing) std::cout << "Error occurred in the key state procedures" << std::endl;
        quit = true;
    }
}

void main_controller::add_to_key_down_queue(int key_code, bool repeat){
    key_down_queue.push(key_push(key_code, repeat));
}

void main_controller::key_down_procedures_update(){
    int i;
    bool key_flag = true;

    if (printing) std::cout << "Key Down queue: ";

    while (!(key_down_queue.empty()) && key_flag){
        if (printing) std::cout << key_down_queue.front().to_string() << "; ";
        ///SEE IF KEY PROCEDURES NEED TO BE DONE
        //DOING MAIN KEY RESPONSES
        if (key_flag){
            i = 0;

            while (i < int(key_down_procedures.size()) && key_flag){
                if (key_down_procedures.at(i).comp_input_ID(key_down_queue.front().scan_code))
                    key_flag = key_down_procedures.at(i).procedure_do(key_down_queue.front().was_repeat, playing, paused);
                i++;
            }
        }

        key_down_queue.pop();
    }

    if (printing) std::cout << "\n";

    if (!key_flag){
        if(printing) std::cout << "Error occurred in the key loop procedures" << std::endl;
        quit = true;
    }

    //std::cout << std::endl;
}

void main_controller::add_to_key_up_queue(int key_code, bool repeat){
    key_up_queue.push(key_push(key_code, repeat));
}

void main_controller::key_up_procedures_update(){
    int i;
    bool key_flag = true;

    if (printing) std::cout << "Key Up queue: ";

    while (!(key_up_queue.empty()) && key_flag){
        if (printing) std::cout << key_up_queue.front().to_string()  << "; ";
        ///SEE IF KEY PROCEDURES NEED TO BE DONE

        //DOING MAIN KEY RESPONSES
        if (key_flag){
            i = 0;

            while (i < int(key_up_procedures.size()) && key_flag){
                if (key_up_procedures.at(i).comp_input_ID(key_up_queue.front().scan_code))
                    key_flag = key_up_procedures.at(i).procedure_do(key_up_queue.front().was_repeat, playing, paused);
                i++;
            }
        }

        key_up_queue.pop();
    }

    if (printing) std::cout << "\n";

    if (!key_flag){
        if(printing) std::cout << "Error occured in the key loop procedures" << std::endl;
        quit = true;
    }

    //std::cout << std::endl;
}

void main_controller::mouse_state_update(){
    unsigned int i = 0;
    bool mouse_flag = true;

    //GET INFORMATION FROM MOUSE'S ACTIONS
    Uint32 mouse_state = e.motion.state;
    int mouse_click;

    if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)){
        mouse_click = 1;
    } else if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        mouse_click = 2;
    } else if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        mouse_click = 3;
    } else {
        mouse_click = 0;
    }

    int x_pos = e.motion.x;
    int y_pos = e.motion.y;

    int x_motion = e.motion.xrel;
    int y_motion = e.motion.yrel;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < mouse_state_procedures.size() && mouse_flag){
        mouse_flag = mouse_state_procedures.at(i).procedure_do(mouse_click, x_pos, y_pos, x_motion, y_motion, playing, paused);

        i++;
    }

    //key_flag = rooms->read_room_key_state_procedures(key_states);

    if (!mouse_flag){
        if(printing) std::cout << "Error occurred in the key state procedures" << std::endl;
        quit = true;
    }
}


void main_controller::event_mouse_update(){
    int i = 0, j;
    bool mouse_flag = true;
    unsigned int button;


    //DECIDE ON THE BUTTON STATE
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
        button = 1;
    } else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        button = 2;
    } else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)){
        button = 3;
    } else {
        button = 0;
    }

    if (printing) std::cout << "Mouse queue: ";

    while (!(room_event_list.empty()) && mouse_flag){
        if (printing) std::cout << room_event_list.front() << "; ";
        ///SEE IF MOUSE PROCEDURES NEED TO BE DONE
        //DOING RESPONSES FROM INTERFACE OF THIS QUEUE
        if (mouse_flag)
            mouse_flag = rooms->read_interface_procedures(i, room_event_list.front(), button, playing, paused);

        //DOING RESPONSES FROM CURRENT ROOM
        if (mouse_flag)
            mouse_flag = rooms->read_room_mouse_procedures(room_event_list.front(), button, playing, paused);

        //DOING RESPONSES FROM MAIN ITSELF
        if (mouse_flag){
            j = 0;

            while (i < int(mouse_procedures.size()) && mouse_flag){
                if (mouse_procedures.at(i).comp_input_ID(room_event_list.front()) && mouse_flag)
                    mouse_flag = mouse_procedures.at(i).procedure_do(button, playing, paused);
                j++;
            }
        }

        room_event_list.pop();
        i++;
    }

    if (printing) std::cout << "\n";

    if (!mouse_flag){
        if(printing) std::cout << "Error occurred in the mouse loop procedures" << std::endl;
        quit = true;
    }

    //std::cout << std::endl;
}

void main_controller::add_start_procedure(loop_procedure new_proc){
    if (room_index >= 0){
        rooms->add_room_start_procedure(room_index, new_proc);
    } else {
        initialize_queue.push(new_proc);
    }
}

///BEGINNING AND END PROCEDURES IN THE MAIN CONTROLLER ARE REMOVED ANYWAYS
void main_controller::remove_start_procedure(int element_ID){
    if (room_index >= 0){
        rooms->remove_room_start_procedure(room_index, element_ID);
    }
}

void main_controller::add_end_procedure(loop_procedure new_proc){
    if (room_index >= 0){
        rooms->add_room_end_procedure(room_index, new_proc);
    } else {
        closing_queue.push(new_proc);
    }
}

///BEGINNING AND END PROCEDURES IN THE MAIN CONTROLLER ARE REMOVED ANYWAYS
void main_controller::remove_end_procedure(int element_ID){
    if (room_index >= 0){
        rooms->remove_room_end_procedure(room_index, element_ID);
    }
}

void main_controller::add_pre_render_procedure(loop_procedure new_proc){
    if (room_index >= 0){
        rooms->add_room_pre_render_procedure(room_index, new_proc);
    } else {
        pre_render_procedures.push_back(new_proc);
    }
}

void main_controller::remove_pre_render_procedure(int element_ID){
    if (room_index >= 0){
        rooms->remove_room_pre_render_procedure(room_index, element_ID);
    } else {
        for (int i = 0; i < int(pre_render_procedures.size()); i++){
            if (pre_render_procedures.at(i).comp_ID(element_ID)){
                pre_render_procedures.erase(pre_render_procedures.begin() + i);
                i--;
            }
        }
    }
}

void main_controller::add_post_render_procedure(loop_procedure new_proc){
    if (room_index >= 0){
        rooms->add_room_post_render_procedure(room_index, new_proc);
    } else {
        post_render_procedures.push_back(new_proc);
    }
}

void main_controller::remove_post_render_procedure(int element_ID){
    if (room_index >= 0){
        rooms->remove_room_post_render_procedure(room_index, element_ID);
    } else {
        for (int i = 0; i < int(post_render_procedures.size()); i++){
            if (post_render_procedures.at(i).comp_ID(element_ID)){
                post_render_procedures.erase(post_render_procedures.begin() + i);
                i--;
            }
        }
    }
}

int main_controller::add_key_state_procedure(int (*new_proc)(bool), const bool play, const bool pause, const int input){
    total_key_state_procs++;
    key_state_procedures.push_back(key_state_procedure(*new_proc, play, pause, input, total_key_state_procs));

    return total_key_state_procs;
}

void main_controller::remove_key_state_procedure(int element_ID){
    for (int i = 0; i < int(key_state_procedures.size()); i++){
        if (key_down_procedures.at(i).comp_ID(element_ID)){
            key_down_procedures.erase(key_down_procedures.begin() + i);
            i--;
        }
    }
}

int main_controller::add_key_down_procedure(int (*new_proc)(bool), const bool play, const bool pause, const int input){
    total_key_down_procs++;
    key_down_procedures.push_back(key_procedure(*new_proc, play, pause, input, total_key_down_procs));

    return total_key_down_procs;
}

void main_controller::remove_key_down_procedure(int element_ID){
    for (int i = 0; i < int(key_down_procedures.size()); i++){
        if (key_down_procedures.at(i).comp_ID(element_ID)){
            key_down_procedures.erase(key_down_procedures.begin() + i);
            i--;
        }
    }
}

int main_controller::add_key_up_procedure(int (*new_proc)(bool), const bool play, const bool pause, const int input){
    total_key_up_procs++;
    key_up_procedures.push_back(key_procedure(new_proc, play, pause, input, total_key_up_procs));

    return total_key_up_procs;
}

void main_controller::remove_key_up_procedure(int element_ID){
    for (int i = 0; i < int(key_up_procedures.size()); i++){
        if (key_up_procedures.at(i).comp_ID(element_ID)){
            key_up_procedures.erase(key_up_procedures.begin() + i);
            i--;
        }
    }
}

/*
void main_controller::add_key_procedure(key_procedure new_proc){
    key_procedures.push_back(new_proc);
}

void main_controller::remove_key_procedure(int element_ID){
    for (int i = 0; i < int(key_procedures.size()); i++){
        if (key_procedures.at(i).comp_ID(element_ID)){
            key_procedures.erase(key_procedures.begin() + i);
            i--;
        }
    }
}
*/

int main_controller::add_mouse_state_procedure(int (*new_proc)(int,int,int,int,int), const bool play, const bool pause){
    total_room_state_procs++;
    mouse_state_procedures.push_back(room_state_procedure(*new_proc, play, pause, total_room_state_procs));

    return total_room_state_procs;
}

void main_controller::remove_mouse_state_procedure(int element_ID){
    for (int i = 0; i < int(mouse_state_procedures.size()); i++){
        if (mouse_state_procedures.at(i).comp_ID(element_ID)){
            mouse_state_procedures.erase(mouse_state_procedures.begin() + i);
            i--;
        }
    }
}

void main_controller::add_mouse_procedure(int (*new_proc)(unsigned int), int mouse_input, const bool play, const bool pause){
    if (room_index >= 0 && panel_index >= 0){
        rooms->add_interface_procedure(room_index, panel_index, room_procedure(new_proc, play, pause, mouse_input, total_mouse_procs));
        total_mouse_procs++;
    } else if (room_index >= 0) {
        rooms->add_room_mouse_procedure(room_index, room_procedure(new_proc, play, pause, mouse_input, total_mouse_procs));
        total_mouse_procs++;
    }
}

void main_controller::remove_mouse_procedure(int element_ID){
    if (room_index >= 0 && panel_index >= 0){
        rooms->remove_interface_procedure(room_index, panel_index, element_ID);
    } else if (room_index >= 0) {
        rooms->remove_room_mouse_procedure(room_index, element_ID);
    } else {
        for (int i = 0; i < int(mouse_procedures.size()); i++){
            if (mouse_procedures.at(i).comp_ID(element_ID)){
                mouse_procedures.erase(mouse_procedures.begin() + i);
                i--;
            }
        }
    }
}

void main_controller::draw_point(SDL_Color color, int x, int y){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_RenderDrawPoint(renderer, x, y);
}

void main_controller::draw_line(SDL_Color color, int x1, int y1, int x2, int y2){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void main_controller::draw_rectangle(SDL_Color color, SDL_Rect* rect, RENDER_STYLE render_style){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    switch (render_style){
        case SOLID:
            SDL_RenderFillRect(renderer, rect);
        break;
        case OUTLINE:
            SDL_RenderDrawRect(renderer, rect);
        break;
    }
}

bool main_controller::get_playing(){
    return playing;
}

void main_controller::play(){
    timer_play = playing = true;
    pause_play = paused = false;
}

bool main_controller::get_paused(){
    return paused;
}

void main_controller::pause(){
    timer_play = playing = false;
    pause_play = paused = true;
}

void main_controller::stop(){
    timer_play = playing = false;
    pause_play = paused = false;
}
