#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <SDL.h>

class m_window{
public:
    //CONSTRUCTOR
    //
    m_window();
    //DESTRUCTOR
    //
    ~m_window();
    //INITIATION
    //
    bool init(int screen_width, int screen_height, const char screen_name[]);
    //SETTING RENDERER
    //
    void set_renderer(SDL_Renderer* new_render_point);
    //CREATE RENDERER
    //
    SDL_Renderer* create_renderer(int settings = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //
    //
    void handle_event(Uint32 e_type, Uint8 e_event, Sint32 data_1, Sint32 data_2);
    //DELOCATOR
    //
    void free();
    //FULL SCREEN MANAGER
    //
    void fullscreen_switch();
    //SURFACEACCESSOR
    //
    SDL_Surface* get_surface();
    //VIEWPORT ACCESSOR
    //
    SDL_Rect get_viewport();
    //SIZE ACCESSOR
    //
    int get_x_size();
    int get_y_size();
    //LISTENER ACCESSORS
    //
    bool is_mouse_focus();
    bool is_keyboard_focus();
    //DETAIL ACCESSOR
    //
    bool is_minimized();

private:
    //Renderer
    //
    SDL_Renderer* renderer;

    //Window
    //
    SDL_Window* main_window;
    //Surface
    //
    SDL_Surface* window_surface;
    //Viewport
    //
    SDL_Rect window_viewport;
    //Dimensions
    //
    int x_size,y_size;
    //Listeners
    //
    bool mouse_focus;
    bool keyboard_focus;
    //Window details
    //
    bool fullscreen;
    bool minimized;
};

#endif // WINDOW_H_INCLUDED
