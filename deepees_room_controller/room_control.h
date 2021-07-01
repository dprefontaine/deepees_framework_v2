#ifndef ROOM_CONTROL_H_INCLUDED
#define ROOM_CONTROL_H_INCLUDED

#include <room.h>

///CODE FOR CONTROLLING ROOMS
//
class room_controller {
public:
    //CONSTRUCTOR
    room_controller(m_window* window_pointer, SDL_Renderer* renderer_pointer, image_source_controller* sources_pointer);
    //DESTRUCTOR
    ~room_controller();

    //RENDERER SET
    void set_renderer(SDL_Renderer* renderer);

    ///ROOM CONTROL FUNCTIONS
    //
    int create_room();
    int create_panel(int room_number, int x, int y, int w, int h, int full_w, int full_h);
    room* room_get(int index = 0);
    panel_interface* interface_get(int room_index = 0, int interface_index = 0);
    bool room_checker();
    bool room_request_deactivation();
    bool room_request_activation();
    void room_render(int x_pos, int y_pos, int x_size, int y_size);
    std::queue<int> room_listen(SDL_Event& e);

    ///ROOM POINTER ACCESSOR AND MUTATORS
    //
    int get_total_rooms();
    int get_current_room();
    int get_previous_room();

    int get_room_display();
    void set_room_display(int new_display = 0);

    //ROOM PROCEDURE MANAGER
    //
    bool read_room_start_procedures(bool playing, bool paused);
    bool add_room_start_procedure(int index, loop_procedure new_proc);
    bool remove_room_start_procedure(int index, int element_ID);

    bool read_room_end_procedures(bool playing, bool paused);
    bool add_room_end_procedure(int index, loop_procedure new_proc);
    bool remove_room_end_procedure(int index, int element_ID);

    bool read_room_pre_render_procedures(unsigned int ticks, bool playing, bool paused);
    bool add_room_pre_render_procedure(int index, loop_procedure new_proc);
    bool remove_room_pre_render_procedure(int index, int element_ID);

    bool read_room_post_render_procedures(unsigned int ticks, bool playing, bool paused);
    bool add_room_post_render_procedure(int index, loop_procedure new_proc);
    bool remove_room_post_render_procedure(int index, int element_ID);

    //KEY PROCEDURES

    ///FOUND I NEVER DO ANYTHING WITH THESE, MAYBE WILL BE RE-ADDED LATER, BUT OTHERWISE DON'T WANT TO OVERENGINEER
    /*
    bool read_room_key_state_procedures(const Uint8* key_states, bool playing, bool paused);
    bool add_room_key_state_procedure(int index, key_state_procedure new_proc);
    bool remove_room_key_state_procedure(int index, int element_ID);

    bool read_room_key_down_procedures(key_push key_obs, bool playing, bool paused);
    bool add_room_key_down_procedure(int index, key_procedure new_proc);
    bool remove_room_key_down_procedure(int index, int element_ID);

    bool read_room_key_up_procedures(key_push key_obs, bool playing, bool paused);
    bool add_room_key_up_procedure(int index, key_procedure new_proc);
    bool remove_room_key_up_procedure(int index, int element_ID);
    */

    bool read_room_mouse_procedures(int mouse_activity, unsigned int mouse_button, bool playing, bool paused);
    bool add_room_mouse_procedure(int index, room_procedure new_proc);
    bool remove_room_mouse_procedure(int index, int element_ID);

    bool read_interface_procedures(int interface, int mouse_activity, unsigned int mouse_button, bool playing, bool paused);
    bool add_interface_procedure(int index, int interface, room_procedure new_proc);
    bool remove_interface_procedure(int index, int interface, int element_ID);

private:
    bool room_exists();
    bool previous_room_exists();

    ///POINTER TO RENDERER AND WINDOW
    //
    m_window* window = NULL;
    SDL_Renderer* renderer = NULL;
    image_source_controller* sources = NULL;

    ///ROOM STATS
    //
    int total_rooms;                 //TOTAL ARRAY SIZE
    int current_room;                //VECTOR INDEX TO CURRENT
    int previous_room;               //VECTOR INDEX TO PREVIOUS
    int room_display;                //VECTOR INDEX TO DISPLAY
    int max_rooms = 10;

    ///ROOM CONTROL VARIABLES
    //
    std::vector<room> rooms;

    room empty_room = room(NULL, NULL, NULL);
    panel_interface empty_interface = panel_interface(-1,-1,-1,-1, -1, -1, NULL, NULL, NULL);
};


#endif // ROOM_CONTROL_H_INCLUDED
