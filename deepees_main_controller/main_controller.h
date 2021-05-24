#ifndef MAIN_CONTROLLER_H_INCLUDED
#define MAIN_CONTROLLER_H_INCLUDED

#include <map>
#include <algorithm>
#include <math.h>

#include <alarm.h>
#include <animation_player.h>

#include <timer.h>

#include <room_control.h>
#include <mixer_bin.h>

enum RENDER_STYLE{
    OUTLINE = 0,
    SOLID
};

class main_controller {
public:
    //CONSTRUCTOR
    main_controller(int screen_width, int screen_height, std::string screen_name, bool printing_on);
    //DESTRUCTOR AND QUITTER
    ~main_controller();

    //SETTINGS
    //
    void set_cursor(int new_cursor_setting);
    void set_render_setting(int new_render_setting);

    //INITIALIZATION AND CLOSING
    //
    bool initialize();
    void close();

    //LOOP
    //
    int loop_run();

    //ROOM MANAGER
    //
    int new_room();
    int new_panel(int room_to, int x, int y, int w, int h, int full_x, int full_y);
    int new_panel(int room_to, int x, int y, int w, int h);
    int set_room(int new_room);

    point get_camera_position(int room_to, int panel_to);
    SDL_Rect get_camera_rectangle(int room_to, int panel_to);
    void set_camera_position(int room_to, int panel_to, int x, int y);        //MAY ADD MORE CAMERA MANAGEMENT FUNCTIONS LATER

    //IMAGE MANAGER
    //
    //FOR LOADING AN IMAGE BAREBONES FOR USE
    std::string new_image_source(std::string source_name, std::string image_path, int room_index, int panel_index);
    std::string new_image_source(std::string source_name, std::string image_path, int room_index);

    //FOR QUICK LOADING AN IMAGE
    std::string new_image(std::string image_name, std::string image_path, int room_index, int panel_index, SDL_Rect clip_rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_image(std::string image_name, std::string image_path, int room_index, int panel_index, SDL_Rect clip_rect);
    std::string new_image(std::string image_name, std::string image_path, int room_index, int panel_index, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_image(std::string image_name, std::string image_path, int room_index, int panel_index);
    std::string new_image(std::string image_name, std::string image_path, int room_index, SDL_Rect clip_rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_image(std::string image_name, std::string image_path, int room_index, SDL_Rect clip_rect);
    std::string new_image(std::string image_name, std::string image_path, int room_index, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_image(std::string image_name, std::string image_path, int room_index);

    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int panel_index, int x_size, int y_size, SDL_Rect clip_rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int panel_index, int x_size, int y_size, SDL_Rect clip_rect);
    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int panel_index, int x_size, int y_size, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int panel_index, int x_size, int y_size);
    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int x_size, int y_size, SDL_Rect clip_rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int x_size, int y_size, SDL_Rect clip_rect);
    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int x_size, int y_size, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    std::string new_blank_image(std::string image_name, SDL_TextureAccess access_type, int room_index, int x_size, int y_size);

    std::string new_text(std::string image_name, std::string font, int room_index, int panel_index, std::string text_contents, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool is_wrapping, Uint32 wrap_count);
    std::string new_text(std::string image_name, std::string font, int room_index, int panel_index, std::string text_contents, bool is_wrapping, Uint32 wrap_count);
    std::string new_text(std::string image_name, std::string font, int room_index, std::string text_contents, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool is_wrapping, Uint32 wrap_count);
    std::string new_text(std::string image_name, std::string font, int room_index, std::string text_contents, bool is_wrapping, Uint32 wrap_count);

    std::string copy_image(std::string image_name, std::string other_image, SDL_Rect clip_rect);
    std::string copy_image(std::string image_name, std::string other_image);

    //dealing with image spots
    std::string create_image(std::string image_name, std::string image_source, SDL_Rect clip_rect);

    void remove_image_copy(std::string image_name);

    //changing image
    std::string reload_as_image(std::string image_name, std::string new_image_path, SDL_Rect clip_rect);
    std::string reload_as_text(std::string image_name, std::string font, std::string new_text, bool is_wrapping, Uint32 wrap_count);
    std::string reload_as_blank(std::string image_name, SDL_TextureAccess access_type, int x_size, int y_size);

    bool get_active(std::string image_name);
    bool set_active(std::string image_name, bool active);

    int get_image_x_size(std::string image_source_name);
    int get_image_y_size(std::string image_source_name);

    SDL_Rect get_spot(std::string image_name);
    void set_spot(std::string image_name, SDL_Rect new_spot);

    int add_point(std::string image_name, int x, int y);
    point* get_point(std::string image_name, int point_index);
    void set_point(std::string image_name, int point_index, int new_x, int new_y);
    void clear_points(std::string image_name);

    int get_layer(std::string image_name);
    void set_layer(std::string image_name, int new_layer);

    int get_flip(std::string image_name);
    void set_flip(std::string image_name, int new_flip);

    double get_rotation(std::string image_name);
    void set_rotation(std::string image_name, double new_rotation);

    SDL_Color get_color(std::string image_name);
    void set_color(std::string image_name, SDL_Color new_color);
    Uint8 get_red(std::string image_name);
    void set_red(std::string image_name, Uint8 new_red);
    Uint8 get_green(std::string image_name);
    void set_green(std::string image_name, Uint8 new_green);
    Uint8 get_blue(std::string image_name);
    void set_blue(std::string image_name, Uint8 new_blue);

    Uint8 get_alpha(std::string image_name);
    void set_alpha(std::string image_name, Uint8 new_alpha);

    void set_blend_mode(std::string image_name, SDL_BlendMode blend_mode);

    bool lock_image(std::string image_name, bool lock);
    SDL_Rect get_lock_rect(std::string image_name);
    void set_lock_rect(std::string image_name, SDL_Rect new_lock_rect);
    void* get_pixels(std::string image_name);
    Uint32 get_pixel(std::string image_name, int x, int y);
    void set_pixel(std::string image_name, Uint32 pixel, int x, int y);
    void set_pixels(std::string image_name, Uint32* pixels);

    void set_as_render_target(std::string image_name, int room_to_copy, int panel_to_copy);
    void set_as_render_target(std::string image_name, int room_to_copy);

    //ANIMATION MANAGER
    //
    void add_animation_set_to_image(std::string image_name);
    void add_animation_state_to_image(std::string image_name, std::string new_animation_state, bool continuous, bool playback);
    void add_animation_frame_to_image(std::string image_name, std::string animation_state, unsigned int ticks, SDL_Rect animation_rect);

    void set_animation_state(std::string image_name, std::string animation_state);

    int get_animation_frame_index(std::string image_name, std::string animation_state);

    void set_animation_active(std::string image_name, bool animation_play);
    bool get_animation_active(std::string image_name);

    //BUTTON MANAGER
    //
    std::string new_button(std::string button_name, int room_index, int panel_index, std::vector<SDL_Rect> new_rectangles);

    bool get_button_active(std::string button_name);
    bool set_button_active(std::string button_name, bool active);

    void transform_button(std::string button_name, std::vector<SDL_Rect> new_rectangles);

    int get_button_over_x(std::string button_name);
    int get_button_over_y(std::string button_name);

    //MIXER MANAGER
    //
    std::string new_sound(std::string sound_name, std::string sound_path, int channel, int loops, Uint8 volume);
    std::string new_music(std::string music_name, std::string music_path, int loops);

    bool play_sound(std::string sound_name);
    bool fade_play_sound(std::string sound_name, unsigned int ms);

    bool get_sound_active(std::string sound_name);
    bool set_sound_active(std::string sound_name, bool active);

    int get_sound_channel(std::string sound_name);
    void set_sound_channel(std::string sound_name, int new_channel);

    Uint8 get_sound_volume(std::string sound_name);
    void set_sound_volume(std::string sound_name, Uint8 new_volume);

    int get_sound_loops(std::string sound_name);
    void set_sound_loops(std::string sound_name, int new_loops);

    bool play_music(std::string music_name);
    bool fade_play_music(std::string music_name, unsigned int ms);

    bool get_music_active(std::string music_name);
    bool set_music_active(std::string music_name, bool active);

    Uint8 get_music_volume();
    Uint8 set_music_volume(Uint8 new_volume);

    int get_music_loops(std::string music_name);
    void set_music_loops(std::string music_name, int new_loops);

    bool get_music_paused();
    void pause_music();

    void halt_music();
    bool fade_halt_music(unsigned int ms);

    bool get_music_playing();
    void resume_music();

    //FONT LIBRARY MANAGER
    //
    std::string add_font(std::string font_name, std::string font_file_path, int font_size);
    std::string get_font_name(int index);

    //LOOP MANAGER
    //
    void add_start_procedure(loop_procedure new_proc);
    void remove_start_procedure(int element_ID);

    void add_end_procedure(loop_procedure new_proc);
    void remove_end_procedure(int element_ID);

    void add_pre_render_procedure(loop_procedure new_proc);
    void remove_pre_render_procedure(int element_ID);

    void add_post_render_procedure(loop_procedure new_proc);
    void remove_post_render_procedure(int element_ID);

    void add_key_procedure(key_procedure new_proc, int room_to);
    void add_key_procedure(key_procedure new_proc);
    void remove_key_procedure(int element_ID, int room_to);
    void remove_key_procedure(int element_ID);

    void add_mouse_procedure(room_procedure new_proc, int room_to, int panel_to);
    void add_mouse_procedure(room_procedure new_proc, int room_to);
    void add_mouse_procedure(room_procedure new_proc);
    void remove_mouse_procedure(int element_ID, int room_to, int panel_to);
    void remove_mouse_procedure(int element_ID, int room_to);
    void remove_mouse_procedure(int element_ID);

    //RENDER
    void draw_point(SDL_Color color, int x, int y);
    void draw_line(SDL_Color color, int x1, int y1, int x2, int y2);
    void draw_rectangle(SDL_Color color, SDL_Rect* rect, RENDER_STYLE render_style);

    //PLAYING AND PAUSING
    bool get_playing();
    void play();

    bool get_paused();
    void pause();

    void stop();

private:
    //WINDOW DATA
    m_window* window;
    int screen_width;
    int screen_height;
    std::string screen_name;

    //CURSOR
    int cursor_setting = SDL_DISABLE;

    //RENDERER
    SDL_Renderer* renderer;
    unsigned int renderer_setting = SDL_RENDERER_SOFTWARE;

    //FOR GETTING PRINTS
    bool printing;

    //FRAME CONSTANTS
    const int MAX_FPS = 60;
    const int MAX_TICKS = 1000/MAX_FPS;

    ///****MAIN MEDIA HERE****
    //
    ///ROOMS
    room_controller* rooms;

    ///POINTING MAPS FOR ROOMS
    typedef std::map<std::string, image_point*> Image_Point_Map;
    Image_Point_Map images;
    image_point* get_image_point(std::string image);
    int total_images = 0;

    ///FOR MANAGING IMAGE SOURCES TO SET IMAGE SPOTS TO
    typedef std::map<std::string, image_source_point*> Image_Source_Point_Map;
    Image_Source_Point_Map image_sources;
    image_source_point* get_image_source_point(std::string source);

    ///FOR SETTING IMAGE LAYERS
    void sort_and_repoint_images(int room_index, int panel_index, room* room_point, panel_interface* panel_point);

    ///BUTTONS
    std::map<std::string, image_point*> buttons; //IT'S CALLED IMAGE POINT, BUT FUNCTIONALLY, IT CAN BE USED FOR BUTTONS AS THEY POINT TO BUTTONS
    image_point* get_button(std::string button);
    int total_buttons = 0;

    ///FONTS
    typedef std::map<std::string, TTF_Font*> Font_Map;
    Font_Map font_lib;
    TTF_Font* get_font(std::string font_name);

    ///MIXER
    mixer_bin* mixer;
    std::map<std::string, mixer_point*> music;
    mixer_point* get_music(std::string music_name);
    int total_music = 0;
    std::map<std::string, mixer_point*> sounds;
    mixer_point* get_sound(std::string sound_name);
    int total_sounds = 0;

    //
    ///***********************

    ///ONLY THE GAME LOOP RUN ACTUALLY USES THESE
    //
    void preloop_initiation();
    void postloop_update();
    //
    void loop_room_start();
    void loop_room_end();
    //
    void pre_render_update();
    void animation_update();
    void post_render_update();
    //
    void event_key_loop();
    //
    void event_mouse_loop();

    //QUITTING APPLICATION
    bool quit;
    bool is_closed;

    //EVENT VARIABLES
    SDL_Event e;
    bool key_bounce;
    std::queue<int> room_event_list;

    std::vector<int> key_down_events;       //TREATING THIS LIKE A STACK AND IS FOR KEEPING TRACK OF REDUNDANT KEY EVENTS AND INDEPENDENT KEY UPS

    //FRAME COUNT
    float avg_FPS;
    int counted_frames;

    //TIMER COUNT AND FLAGS
    Uint32 previous_ticks;
    bool timer_play, pause_play;

    //LOOP CONTROL
    bool playing, paused;

    SDL_Surface* window_surface;// = NULL;
    timer main_timer;
    timer cap_timer;

    //LOOP VARIABLES
    std::queue<loop_procedure> initialize_queue;
    int max_init = 10;
    std::queue<loop_procedure> closing_queue;
    int max_close = 10;
    std::vector<loop_procedure> pre_render_procedures;
    int max_pre_render = 50;
    std::vector<loop_procedure> post_render_procedures;
    int max_post_render = 50;

    //CONTROL
    std::vector<key_procedure> key_procedures;
    int max_key_responses = 50;
    std::vector<room_procedure> mouse_procedures;
    int max_mouse_responses = 50;
};

#endif // MAIN_CONTROLLER_H_INCLUDED
