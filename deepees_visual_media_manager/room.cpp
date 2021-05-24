#include "room.h"

//CONSTRUCTOR
//
room::room(m_window* window_pointer, SDL_Renderer* renderer_pointer){
    is_active = false;

    interfaces = std::vector<panel_interface>();
    room_start_procedures = std::vector<loop_procedure>();
    room_pre_render_procedures = std::vector<loop_procedure>();
    room_post_render_procedures = std::vector<loop_procedure>();
    room_end_procedures = std::vector<loop_procedure>();

    room_key_procedures = std::vector<key_procedure>();

    room_mouse_procedures = std::vector<room_procedure>();

    window = window_pointer;
    renderer = renderer_pointer;
};

//DESTRUCTOR
//
room::~room(){
    deactivate_room();

    //std::cout << "Setting room inactive..." << std::endl;

    //FREEING MEMORY
    std::vector<panel_interface>().swap(interfaces);

    std::vector<loop_procedure>().swap(room_start_procedures);

    std::vector<loop_procedure>().swap(room_pre_render_procedures);

    std::vector<loop_procedure>().swap(room_post_render_procedures);

    std::vector<loop_procedure>().swap(room_end_procedures);

    std::vector<key_procedure>().swap(room_key_procedures);

    std::vector<room_procedure>().swap(room_mouse_procedures);

    window = NULL;
};


int room::add_interface(int vp_x, int vp_y, int vp_w, int vp_h, int full_x_panel, int full_y_panel){
    interfaces.push_back(panel_interface(vp_x,vp_y,vp_w,vp_h, full_x_panel, full_y_panel, window, renderer));
    //REMOVED TOTAL INTERFACE VARIABLE HERE

    return interfaces.size()-1;
}

//LOADING WHOLE ROOM AS SNAPSHOT
//
bool room::visual_media_load_as_snapshot(int index, int x_pos, int y_pos, int x_size, int y_size){
    bool load_flag = false;

    if (is_within_index(visual_media_bin.size(),index)){
        load_flag = visual_media_bin[index].load_as_blank(SDL_TEXTUREACCESS_TARGET);

        if (load_flag)
            load_flag = visual_media_set_renderer_target(index);

        render(x_pos, y_pos, x_size, y_size);
        visual_media_free_renderer_target();
    } else
        std::cout << "This index is not available!" << std::endl;

    return load_flag;
}

void room::set_renderer(SDL_Renderer* new_renderer){
    media_bin::set_renderer(new_renderer);

    for (unsigned int i = 0; i < interfaces.size(); i++)
        interfaces.at(i).set_renderer(new_renderer);
}

//ACTIVATING A ROOM
//
void room::activate_room(){
    is_active = true;

    //ACTIVATING INTERFACES
    //
    for (unsigned int i = 0; i < interfaces.size(); i++){
        interfaces[i].set_active();
        interfaces[i].set_listening(true);
    }
}

//DEACTIVATING A ROOM
//
void room::deactivate_room(){
    is_active = false;

    for (unsigned int i = 0; i < interfaces.size(); i++){
        interfaces[i].set_inactive();
        interfaces[i].set_listening(false);
    }
}

//
std::queue<int> room::event_listen(SDL_Event& e){
    //IN CASE OF ANY WINDOW FEATURES CHANGING
    //
    window->handle_event(e.type,e.window.event,e.window.data1,e.window.data2);
    //GETTING MOUSE POSITION
    //
    int x1,y1;
    SDL_GetMouseState(&x1,&y1);
    //ARRAY OF INTERFACE ACTIVITIES
    //
    std::queue<int> activities = std::queue<int>();

    for (unsigned int i = 0; i < interfaces.size(); i++){
        activities.push(interfaces[i].touch_event(e.type,x1,y1));
        //std::cout << activities[i] << " ";
    }
    //std::cout << " " << std::endl;

    return activities;
};

//RENDERING ROOM AFTER UPDATE
//
void room::render(int x_pos, int y_pos, int x_size, int y_size){
    //INITIAL PROCEDURES (IF ANY)
    //
    SDL_RenderClear(renderer);
    //std::cout << interfaces.size() << std::endl;
    SDL_Rect test_fill;
    ///RENDERING INTERFACES FIRST
    //
    for (int i = interfaces.size()-1; i >= 0; i--){
        test_fill = interfaces[i].get_viewport();

        // THIS IS FOR SEEING IF VIEWPORTS ARE BEING PLACED PROPERLY

        SDL_SetRenderDrawColor(renderer,10+i*10,10+i*10,10+i*10,255);

        SDL_RenderSetViewport(renderer,&test_fill);
        SDL_RenderFillRect(renderer,&test_fill);
        interfaces[i].render();

        media_bin::render_bin(x_pos, y_pos, x_size, y_size);
    }
}

//INTERFACES ACCESSOR AND MODIFIERS
//
panel_interface* room::interface_get(int index){
    try {
        return &interfaces.at(index);
    } catch (std::out_of_range e){
        return NULL;
    }
}

bool room::interface_check_index(int index){
    return (is_within_index(interfaces.size(),index));
}

void room::interface_misc_camera_update(int index, int x_move, int y_move){
    if (is_within_index(interfaces.size(),index))
        interfaces.at(index).camera_update(x_move,y_move);
}

bool room::interface_misc_get_active(int index){
    if (is_within_index(interfaces.size(),index))
        return interfaces.at(index).get_active();
    else
        return false;
}

void room::interface_misc_set_active(int index){
    if (is_within_index(interfaces.size(),index))
        interfaces.at(index).set_active();
}

void room::interface_misc_set_inactive(int index){
    if (is_within_index(interfaces.size(),index))
        interfaces.at(index).set_inactive();
}

bool room::interface_misc_get_listening(int index){
    if (is_within_index(interfaces.size(),index))
        return interfaces.at(index).get_listening();
    else
        return false;
}

void room::interface_misc_set_listening(int index, bool listening){
    if (is_within_index(interfaces.size(),index))
        interfaces.at(index).set_listening(listening);
}

bool room::read_start_procedures(bool playing, bool paused){
    int i = 0;
    bool start_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < int(room_start_procedures.size()) && start_flag){
        start_flag = room_start_procedures.at(i).procedure_do(0, true, true);
        i++;
    }

    return start_flag;
}

void room::add_start_procedure(loop_procedure new_proc){
    room_start_procedures.push_back(new_proc);
}

void room::remove_start_procedure(int element_ID){
    for (int i = 0; i < int(room_start_procedures.size()); i++)
        if (room_start_procedures.at(i).comp_ID(element_ID)){
            room_start_procedures.erase(room_start_procedures.begin() + i);
            i--;
        }
}

bool room::read_end_procedures(bool playing, bool paused){
    int i = 0;
    bool end_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < int(room_end_procedures.size()) && end_flag){
        end_flag = room_end_procedures.at(i).procedure_do(0, true, true);
        i++;
    }

    return end_flag;
}

void room::add_end_procedure(loop_procedure new_proc){
    room_end_procedures.push_back(new_proc);
}

void room::remove_end_procedure(int element_ID){
    for (int i = 0; i < int(room_end_procedures.size()); i++)
        if (room_end_procedures.at(i).comp_ID(element_ID)){
            room_end_procedures.erase(room_end_procedures.begin() + i);
            i--;
        }
}

bool room::read_pre_render_procedures(int ticks, bool playing, bool paused){
    int i = 0;
    bool pre_render_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < int(room_pre_render_procedures.size()) && pre_render_flag){
        pre_render_flag = room_pre_render_procedures.at(i).procedure_do(ticks, playing, paused);
        i++;
    }

    return pre_render_flag;
}

void room::add_pre_render_procedure(loop_procedure new_proc){
    room_pre_render_procedures.push_back(new_proc);
}

void room::remove_pre_render_procedure(int element_ID){
    for (int i = 0; i < int(room_pre_render_procedures.size()); i++)
        if (room_pre_render_procedures.at(i).comp_ID(element_ID)){
            room_pre_render_procedures.erase(room_pre_render_procedures.begin() + i);
            i--;
        }
}

bool room::read_post_render_procedures(int ticks, bool playing, bool paused){
    int i = 0;
    bool post_render_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < int(room_post_render_procedures.size()) && post_render_flag){
        post_render_flag = room_post_render_procedures.at(i).procedure_do(ticks, playing, paused);
        i++;
    }

    return post_render_flag;
}

void room::add_post_render_procedure(loop_procedure new_proc){
    room_post_render_procedures.push_back(new_proc);
}

void room::remove_post_render_procedure(int element_ID){
    for (int i = 0; i < int(room_post_render_procedures.size()); i++)
        if (room_post_render_procedures.at(i).comp_ID(element_ID)){
            room_post_render_procedures.erase(room_post_render_procedures.begin() + i);
            i--;
        }
}

bool room::read_key_procedures(int input, bool playing, bool paused){
    int i = 0;
    bool key_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < int(room_key_procedures.size()) && key_flag){
        if (room_key_procedures.at(i).comp_input_ID(input))
            key_flag = room_key_procedures.at(i).procedure_do(playing, paused);

        i++;
    }

    return key_flag;
}

void room::add_key_procedure(key_procedure new_proc){
    room_key_procedures.push_back(new_proc);
}

void room::remove_key_procedure(int element_ID){
    for (int i = 0; i < int(room_key_procedures.size()); i++)
        if (room_key_procedures.at(i).comp_ID(element_ID)){
            room_key_procedures.erase(room_key_procedures.begin() + i);
            i--;
        }
}

//THIS SHOULD BE O(N*M)
bool room::read_mouse_procedures(int mouse_activity, bool playing, bool paused){
    int i = 0;
    bool mouse_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    while (i < int(room_mouse_procedures.size()) && mouse_flag){
        if (room_mouse_procedures[i].comp_input_ID(mouse_activity))
            mouse_flag = room_mouse_procedures.at(i).procedure_do(playing, paused);

        i++;
    }

    return mouse_flag;
}

void room::add_mouse_procedure(room_procedure new_proc){
    room_mouse_procedures.push_back(new_proc);
}

void room::remove_mouse_procedure(int element_ID){
    for (int i = 0; i < int(room_mouse_procedures.size()); i++)
        if (room_mouse_procedures.at(i).comp_ID(element_ID)){
            room_mouse_procedures.erase(room_mouse_procedures.begin() + i);
            i--;
        }
}

bool room::read_interface_procedures(int interface, int mouse_activity, bool playing, bool paused){
    bool interface_flag = true;

    //GOING THROUGH PROCEDURES UNTIL ERROR OR ALL ARE READ
    if (is_within_index(interfaces.size(),interface))
        interface_flag = interfaces.at(interface).read_panel_procedures(mouse_activity, playing, paused);

    return interface_flag;
}

bool room::add_interface_procedure(int interface, room_procedure new_proc){
    if (is_within_index(interfaces.size(),interface)){
        interfaces.at(interface).add_panel_procedure(new_proc);
        return true;
    } else
        return false;
}

bool room::remove_interface_procedure(int interface, int element_ID){
    if (is_within_index(interfaces.size(),interface)){
        interfaces.at(interface).remove_panel_procedure(element_ID);
        return true;
    } else
        return false;
}
