#include "window.h"

m_window::m_window(){
    main_window = NULL;
    window_surface = NULL;
    x_size = y_size = 0;
    fullscreen = minimized = keyboard_focus = mouse_focus = false;
}

m_window::~m_window(){
    free();
}

bool m_window::init(int screen_width, int screen_height, const char screen_name[]){
    bool window_init_flag = true;

    main_window = SDL_CreateWindow(screen_name,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screen_width,screen_height,SDL_WINDOW_SHOWN);

    if (main_window == NULL){
        window_init_flag = false;
    } else {
        window_viewport.x = window_viewport.y = 0;
        window_viewport.w = x_size = screen_width;
        window_viewport.h = y_size = screen_height;
        mouse_focus = true;
        keyboard_focus = true;

        window_surface = SDL_GetWindowSurface(main_window);
    }

    return window_init_flag;
}


void m_window::set_renderer(SDL_Renderer* new_render_point){
    renderer = new_render_point;
}


SDL_Renderer* m_window::create_renderer(int settings){
    return renderer = SDL_CreateRenderer(main_window,-1,settings);
}

void m_window::free(){
    SDL_FreeSurface(window_surface);
    SDL_DestroyWindow(main_window);

    window_surface = NULL;
    window_viewport.h = window_viewport.w = window_viewport.y = window_viewport.x = 0;
    main_window = NULL;
}

void m_window::handle_event(Uint32 e_type, Uint8 e_event, Sint32 data_1, Sint32 data_2){
    //Event occur
    //
    if (e_type == SDL_WINDOWEVENT){
        switch (e_event){
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                x_size = data_1;
                y_size = data_2;
            break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent(renderer);
            break;
            case SDL_WINDOWEVENT_ENTER:
                mouse_focus = true;
            break;
            case SDL_WINDOWEVENT_LEAVE:
                mouse_focus = false;
            break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                keyboard_focus = true;
            break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                keyboard_focus = false;
            break;
            case SDL_WINDOWEVENT_MINIMIZED:
                minimized = true;
            break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                minimized = false;
            break;
            case SDL_WINDOWEVENT_RESTORED:
                minimized = false;
            break;
        }
    }
}

void m_window::fullscreen_switch(){
    //MANAGING FULLSCREEN
    //
    if (fullscreen){
        SDL_SetWindowFullscreen(main_window,SDL_FALSE);
        fullscreen = false;
    } else {
        SDL_SetWindowFullscreen(main_window,SDL_TRUE);
        fullscreen = true;
        minimized = false;
    }
}

SDL_Surface* m_window::get_surface(){
    return window_surface;
}

SDL_Rect m_window::get_viewport(){
    return window_viewport;
}

int m_window::get_x_size(){
    return x_size;
}

int m_window::get_y_size(){
    return y_size;
}

bool m_window::is_mouse_focus(){
    return mouse_focus;
}

bool m_window::is_keyboard_focus(){
    return keyboard_focus;
}

bool m_window::is_minimized(){
    return minimized;
}
