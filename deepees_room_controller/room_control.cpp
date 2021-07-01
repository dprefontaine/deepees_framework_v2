#include "room_control.h"

///CODE FOR CONTROLLING ROOMS
//
room_controller::room_controller(m_window* window_pointer, SDL_Renderer* renderer_pointer, image_source_controller* sources_pointer){
    rooms = std::vector<room>();
    current_room = -1;
    previous_room = 0;
    room_display = 0;
    total_rooms = 0;

    window = window_pointer;
    renderer = renderer_pointer;
    sources = sources_pointer;
}

room_controller::~room_controller(){
    //CLEARING POINTERS
    window = NULL;
    renderer = NULL;
    sources = NULL;

    //FREEING
    rooms.clear();
    //FREEING MEMORY OF ROOM ARRAY
    std::vector<room>().swap(rooms);
    empty_room.~room();
    empty_interface.~panel_interface();
}

void room_controller::set_renderer(SDL_Renderer* new_renderer){
    renderer = new_renderer;

    for (unsigned int i = 0; i < rooms.size(); i++)
        rooms.at(i).set_renderer(new_renderer);
}

/*
void room_controller::set_sources(image_sources* new_image_sources){
    sources = new_image_sources;

    for (unsigned int i = 0; i < rooms.size(); i++)
        rooms.at(i).set_sources(new_renderer);
}
*/

int room_controller::create_room(){
    rooms.push_back(room(window, renderer, sources));
    total_rooms++;
    return total_rooms-1;
}

int room_controller::create_panel(int room_number, int x, int y, int w, int h, int full_w, int full_h){
    if (is_within_index(rooms.size(), room_number))
        //RETURNING THE INTERFACE INDEX
        return rooms.at(room_number).add_interface(x, y, w, h, full_w, full_h);
    else
        //RETURNING A FALSE VALUE
        return -1;
}

room* room_controller::room_get(int room_index){
    room* returning_room = NULL;

    if (is_within_index(total_rooms, room_index))
        returning_room = &rooms.at(room_index);

    return returning_room;
}

panel_interface* room_controller::interface_get(int room_index, int interface_index){
    try {
        return rooms.at(room_index).interface_get(interface_index);
    } catch (std::out_of_range e) {
        return NULL;
    }
}

bool room_controller::room_exists(){
    //CHECKING IF CURRENT ROOM IS IN BOUNDS OF THE ROOMS ARRAY
    //
    return is_within_index(total_rooms,current_room);
}

bool room_controller::previous_room_exists(){
    //CHECKING IF CURRENT ROOM IS IN BOUNDS OF THE ROOMS ARRAY
    //
    if (is_within_index(total_rooms,previous_room)){
        //std::cout << "Room was found" << std::endl;
        return true;
    } else
        return false;
}

bool room_controller::room_checker(){
    if (current_room != room_display){
        std::cout << "Switching rooms: display " << room_display << " ; current " << current_room << " ; total " << total_rooms << std::endl;
        //DEACTIVATE PREVIOUS ROOM ONCE NOTED

        //KEEP TRACK OF PREVIOUS ROOM
        //
        previous_room = current_room;

        //SET THE NEW CURRENT ROOM
        //
        if (room_display < 0){
            //std::cout << "Setting to min room" << std::endl;
            current_room = 0;
        } else if (room_display >= total_rooms){
            //std::cout << "Setting to max room" << std::endl;
            current_room = room_display = total_rooms - 1;
        } else {
            //std::cout << "Setting to display room" << std::endl;
            current_room = room_display;
        }

        return true;
    } else {
        return false;
    }
}

bool room_controller::room_request_deactivation(){
    try {
        rooms.at(current_room).deactivate_room();
        return true;
    } catch (std::out_of_range e){
        set_room_display();
        return false;
    }
}

bool room_controller::room_request_activation(){
    try {
        rooms.at(current_room).activate_room();
        return true;
    } catch (std::out_of_range e){
        set_room_display();
        return false;
    }
}

void room_controller::room_render(int x_pos, int y_pos, int x_size, int y_size){
    try {
        rooms.at(current_room).render(x_pos, y_pos, x_size, y_size);
    } catch (std::out_of_range e){
        set_room_display();
    }
}

std::queue<int> room_controller::room_listen(SDL_Event& e){
    try {
        return rooms.at(current_room).event_listen(e);
    } catch (std::out_of_range e){
        set_room_display();
        return std::queue<int>();
    }
}

int room_controller::get_total_rooms(){
    return total_rooms;
}

int room_controller::get_current_room(){
    return current_room;
}

int room_controller::get_previous_room(){
    return previous_room;
}

int room_controller::get_room_display(){
    return room_display;
}

void room_controller::set_room_display(int new_display){
    room_display = new_display;
}

bool room_controller::read_room_start_procedures(bool playing, bool paused){
    try {
        return rooms.at(current_room).read_start_procedures(playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_start_procedure(int index, const loop_procedure new_proc){
    try {
        rooms.at(index).add_start_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_start_procedure(int index, int element_ID){
    try {
        rooms.at(index).remove_start_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::read_room_end_procedures(bool playing, bool paused){
    try {
        rooms.at(current_room).read_end_procedures(playing, paused);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_end_procedure(int index, loop_procedure new_proc){
    try {
        rooms.at(index).add_end_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_end_procedure(int index, int element_ID){
    try {
        rooms.at(index).remove_end_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::read_room_pre_render_procedures(unsigned int ticks, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_pre_render_procedures(ticks, playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_pre_render_procedure(int index, loop_procedure new_proc){
    try {
        rooms.at(index).add_pre_render_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_pre_render_procedure(int index, int element_ID){
    try {
        rooms.at(index).remove_pre_render_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::read_room_post_render_procedures(unsigned int ticks, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_post_render_procedures(ticks, playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_post_render_procedure(int index, loop_procedure new_proc){
    try {
        rooms.at(index).add_post_render_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_post_render_procedure(int index, int element_ID){
    try {
        rooms.at(index).remove_post_render_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}



/*
bool room_controller::read_room_key_state_procedures(const Uint8* key_states, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_key_state_procedures(key_states, playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_key_state_procedure(int index, key_state_procedure new_proc){
    try {
        rooms.at(current_room).add_key_state_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_key_state_procedure(int index, int element_ID){
    try {
        rooms.at(current_room).remove_key_state_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::read_room_key_down_procedures(key_push key_obs, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_key_down_procedures(key_obs, playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_key_down_procedure(int index, key_procedure new_proc){
    try {
        rooms.at(current_room).add_key_down_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_key_down_procedure(int index, int element_ID){
    try {
        rooms.at(current_room).remove_key_down_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::read_room_key_up_procedures(key_push key_obs, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_key_up_procedures(key_obs, playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_key_up_procedure(int index, key_procedure new_proc){
    try {
        rooms.at(current_room).add_key_up_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_key_up_procedure(int index, int element_ID){
    try {
        rooms.at(current_room).remove_key_up_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::read_room_key_procedures(int input, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_key_procedures(input, playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_key_procedure(int index, key_procedure new_proc){
    try {
        rooms.at(current_room).add_key_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_key_procedure(int index, int element_ID){
    try {
        rooms.at(current_room).remove_key_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}
*/

bool room_controller::read_room_mouse_procedures(int mouse_activity, unsigned int mouse_button, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_mouse_procedures(mouse_activity, mouse_button, playing, paused);
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::add_room_mouse_procedure(int index, room_procedure new_proc){
    try {
        rooms.at(current_room).add_mouse_procedure(new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_room_mouse_procedure(int index, int element_ID){
    try {
        rooms.at(current_room).remove_mouse_procedure(element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::read_interface_procedures(int interface, int mouse_activity, unsigned int mouse_button, bool playing, bool paused){
    try {
        return rooms.at(current_room).read_interface_procedures(interface, mouse_activity, mouse_button, playing, paused);
    } catch (std::out_of_range e){
        return true;
    }
}

bool room_controller::add_interface_procedure(int index, int interface, room_procedure new_proc){
    try {
        rooms.at(index).add_interface_procedure(interface, new_proc);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}

bool room_controller::remove_interface_procedure(int index, int interface, int element_ID){
    try {
        rooms.at(index).remove_interface_procedure(interface, element_ID);
        return true;
    } catch (std::out_of_range e){
        return false;
    }
}
