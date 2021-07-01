#ifndef GAME_INTERFACE_H_INCLUDED
#define GAME_INTERFACE_H_INCLUDED

#include "media_bin.h"
#include <window.h>
#include <procedures.h>

class panel_interface : public media_bin{
public:
    ///CONSTRUCTOR
    //
    //extended (bound not set to viewport)
    panel_interface(int vp_x,int vp_y,int vp_w,int vp_h, int x_bound, int y_bound, m_window* window_pointer, SDL_Renderer* renderer_pointer, image_source_controller* source_pointer);
    //
    ///DESTRUCTOR
    ~panel_interface();

    ///VIEWPORT MANAGER
    //
    SDL_Rect get_viewport();
    //
    ///RENDERING
    //
    void render();
    //
    ///EVENTS
    //
    int touch_event(const Uint32 e_type, const int x1, const int y1);
    bool* hitbox_check(int element_ID_1 = 0, int element_ID_2 = 0);
    //
    ///BUTTON MANAGER
    //
    int button_new(button_bundle new_button, std::vector<SDL_Rect> button_rects);
    bool button_update(int index = 0, int new_state = 0);
    bool button_move(int index = 0, int x_move = 0, int y_move = 0);
    bool button_get_active(int index = 0);
    bool button_set_active(int index = 0, bool active = false);
    int button_find_by_ID(int element_ID = 0);
    button button_get(int index = 0);
    void button_transform(int index, std::vector<SDL_Rect> new_rects);
    bool button_clear(int index = 0);
    int button_get_over_x(int index = 0);
    int button_get_over_y(int index = 0);

    //
    ///CAMERA MANAGER (offset to other objects more implied)
    //
    void camera_update(int x_move = 0, int y_move = 0);
    point camera_get_position();
    SDL_Rect camera_get_rect();
    //
    ///ACTIVE AND LISTENING MANAGER
    //
    bool get_active();
    void set_active();
    void set_inactive();
    //
    bool get_listening();
    void set_listening(bool listening = false);
    //
    ///PROCEDURE STUFF
    //
    bool read_panel_procedures(int mouse_activity, unsigned int mouse_button, bool playing, bool paused);
    void add_panel_procedure(room_procedure new_proc);
    void remove_panel_procedure(int element_ID);

private:
    //POINTERS TO WINDOW AND RENDERER
    m_window* window;

    //GAME BUTTONS (FOR TOUCH EVENT) AND OTHER EVENT STUFF
    //
    std::vector<button_bundle> panel_buttons = {};
    //
    //MATH BINS AND STUFF
    //
    camera panel_camera;
    point full_panel_size;
    //
    //IS ACTIVE, LISTENED, OR NOT
    //
    bool is_active;
    bool is_listened;
    //VIEWING WINDOW OF INTERFACE
    //
    SDL_Rect viewport;
    SDL_Rect actual_viewport;
    //INTERFACE ACTIVITY PROCEDURES
    //
    std::vector<room_procedure> panel_procedures;
};

#endif // GAME_INTERFACE_H_INCLUDED
