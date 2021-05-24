#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include <queue>
#include "panel_interface.h"

#include <loop_procedure.h>
#include <key_procedure.h>

//FOR LOADING ROOMS
//

class room : public media_bin{
public:
    //CONSTRUCTOR
    room(m_window* window_pointer, SDL_Renderer* renderer_pointer);
    //DESTRUCTOR
    ~room();

    void set_renderer(SDL_Renderer* renderer);

    ///FOR LOADING INTERFACES AND REMOVING INTERFACES
    //
    int add_interface(int vp_x = 0, int vp_y = 0, int vp_w = 1, int vp_h = 1, int full_x_panel = 1, int full_y_panel = 1);
    void clear_room();
    //
    bool visual_media_load_as_snapshot(int index, int x_pos, int y_pos, int x_size, int y_size);
    //
    ///FOR ACTIVATING OR DEACTIVATING A ROOM
    //
    void activate_room();
    void deactivate_room();
    //
    ///EVENT STUFF
    //
    std::queue<int> event_listen(SDL_Event& e);
    //
    ///RENDERING
    //
    void render(int x_pos, int y_pos, int x_size, int y_size);
    //

    ///INTERFACES ACCESSOR AND MODIFIERS
    //
    panel_interface* interface_get(int index = 0);
    bool interface_check_index(int index = 0);
    //
    ///MISC
    //
    void interface_misc_camera_update(int index = 0, int x_move = 0, int y_move = 0);
    bool interface_misc_get_active(int index = 0);
    void interface_misc_set_active(int index = 0);
    void interface_misc_set_inactive(int index = 0);
    bool interface_misc_get_listening(int index = 0);
    void interface_misc_set_listening(int index = 0, bool listening = false);

    ///PROCEDURE MANAGER
    //
    bool read_start_procedures(bool playing, bool paused);
    void add_start_procedure(loop_procedure new_proc);
    void remove_start_procedure(int element_ID);

    bool read_end_procedures(bool playing, bool paused);
    void add_end_procedure(loop_procedure new_proc);
    void remove_end_procedure(int element_ID);

    bool read_pre_render_procedures(int ticks, bool playing, bool paused);
    void add_pre_render_procedure(loop_procedure new_proc);
    void remove_pre_render_procedure(int element_ID);

    bool read_post_render_procedures(int ticks, bool playing, bool paused);
    void add_post_render_procedure(loop_procedure new_proc);
    void remove_post_render_procedure(int element_ID);

    bool read_key_procedures(int input, bool playing, bool paused);
    void add_key_procedure(key_procedure new_proc);
    void remove_key_procedure(int element_ID);

    bool read_mouse_procedures(int mouse_activity, bool playing, bool paused);
    void add_mouse_procedure(room_procedure new_proc);
    void remove_mouse_procedure(int element_ID);

    bool read_interface_procedures(int interface, int mouse_activity, bool playing, bool paused);
    bool add_interface_procedure(int interface, room_procedure new_proc);
    bool remove_interface_procedure(int interface, int element_ID);

private:
    ///POINTERS TO WINDOW AND RENDERER
    //
    m_window* window;

    //BASIC ROOM DETAILS
    //
    bool is_active;

    std::vector<panel_interface> interfaces;

    ///ROOM PROCEDURES
    //
    std::vector<loop_procedure> room_start_procedures;
    std::vector<loop_procedure> room_pre_render_procedures;
    std::vector<loop_procedure> room_post_render_procedures;
    std::vector<loop_procedure> room_end_procedures;

    std::vector<key_procedure> room_key_procedures;

    std::vector<room_procedure> room_mouse_procedures;
};

#endif // ROOM_H_INCLUDED
