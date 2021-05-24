#include "panel_interface.h"

//CONSTRUCTOR
panel_interface::panel_interface(int vp_x,int vp_y,int vp_w,int vp_h, int x_bound, int y_bound, m_window* window_pointer, SDL_Renderer* renderer_pointer){
    viewport.x = vp_x;
    viewport.y = vp_y;
    viewport.w = vp_w;
    viewport.h = vp_h;

    actual_viewport.x = 0;
    actual_viewport.y = 0;
    actual_viewport.w = vp_x + vp_w;
    actual_viewport.h = vp_y + vp_h;

    full_panel_size = point(x_bound, y_bound);
    panel_camera = camera(-vp_x,-vp_y,vp_w+vp_x,vp_h+vp_y);

    is_active = false;
    is_listened = false;

    panel_procedures = std::vector<room_procedure>();

    window = window_pointer;
    renderer = renderer_pointer;
}

//DESTRUCTOR

panel_interface::~panel_interface(){
    is_active = false;
    is_listened = false;

    //FREEING MEMORY
    std::vector<button_bundle>().swap(panel_buttons);

    std::vector<room_procedure>().swap(panel_procedures);

    window = NULL;
}

//VIEWPORT MANAGER

SDL_Rect panel_interface::get_viewport(){
    return actual_viewport;
}

void panel_interface::render(){
    media_bin::render_bin(panel_camera.get_x_pos(),panel_camera.get_y_pos(),panel_camera.get_x_size(),panel_camera.get_y_size());
}

//TOUCH EVENT MANAGER

int panel_interface::touch_event(const Uint32 e_type, const int x1, const int y1){
    //INITIALIZING THIS FIRST IN CASE OF VOID CASES
    //
    //J REPRESENTS A PARTICULAR NODE EVENT RETURNED TO THE MAIN PROGRAM
    //
    int current_button = -1;

    //TO SHOW THE MOUSE IS OVER A VIEWPORT
    //
    if (x1 > viewport.x && x1 < viewport.x + viewport.w && y1 > viewport.y && y1 < viewport.y + viewport.h){
        current_button = 0;
    }

    if (is_active && is_listened &&
        current_button == 0 &&
        (e_type == SDL_MOUSEMOTION || e_type == SDL_MOUSEBUTTONDOWN || e_type == SDL_MOUSEBUTTONUP)){
        //GET MOUSE POSITION
        //
        int total_buttons = panel_buttons.size();
        bool over_button = false;
        //CYCLE THROUGH BUTTON STACK AND EVALUATE ALL BUTTON RECTANGLES
        //
        //ALSO CONSIDER THE OFFSET OF THE VIEWPORT (OR THE STARTING CORNER COORDINATES OF THE VIEWPORT)
        //
        //USING WHILE SO THAT NOT ALL VIEWPORTS HAVE TO BE CYCLED IF REACHED
        //
        //THE LOOP SHOULD BREAK ONCE A BUTTON IS REACHED
        //
        while (!(over_button) && current_button < total_buttons){
            if (panel_buttons.at(current_button).is_over(x1,y1,viewport.x,viewport.y)){
                over_button = true;
            } else {
                current_button++;
            }
        }
        //
        //IF REACHED THE END OF THE LIST
        if (current_button == total_buttons)
            current_button = 0;
        else if (panel_buttons.at(current_button).m_button.get_active()){
            //MAKE SURE THE BUTTON SELECTED IS ACTIVE

            //IF CONDITION FOR ANY OF THE BUTTONS IS FULFILLED, RETURN A VALUE RELATED TO ITS NODE
            //
            //NODE = (BUTTON STACK)*4 + some number
            //
            //WARNING: PROBABLY WILL HAVE TO MANUALLY PROGRAM RENDERING AND BASE EVENTS FOR EACH BUTTON CONDITION
            //
            switch (e_type){
            case SDL_MOUSEBUTTONUP:
                button_update(current_button,1);
                current_button = current_button*5 + 1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                button_update(current_button,2);
                current_button = current_button*5 + 2;
                break;
            case SDL_MOUSEMOTION:
                button_update(current_button,3);
                current_button = current_button*5 + 3;
                break;
            }
        } else {
            //CONSIDER IT AS INACTIVE
            button_update(current_button,4);
            current_button = current_button*5 + 4;
        }
    }

    return current_button;
}

int panel_interface::button_new(button_bundle new_rect, std::vector<SDL_Rect> button_rects){
    panel_buttons.push_back(new_rect);
    panel_buttons.at(panel_buttons.size()-1).rectangles.swap(button_rects);

    return (panel_buttons.size()-1);
}

bool panel_interface::button_update(int index, int new_state){
    bool update_flag = true;

    if (is_within_index(panel_buttons.size(),index))
        panel_buttons[index].m_button.set_state(new_state);

    return update_flag;
}

bool panel_interface::button_move(int index, int x_move, int y_move){
    if (is_within_index(panel_buttons.size(),index)) {
        panel_buttons[index].move_rectangles(x_move,y_move);
        return true;
    }
    else
        return false;
}

bool panel_interface::button_get_active(int index){
    if (is_within_index(panel_buttons.size(),index)) {
        return panel_buttons.at(index).m_button.get_active();
    }
    else
        return false;
}

bool panel_interface::button_set_active(int index, bool active){
    if (is_within_index(panel_buttons.size(),index)) {
        panel_buttons.at(index).m_button.set_active(active);
        return true;
    }
    else
        return false;
}

int panel_interface::button_find_by_ID(int element_ID){
    int i = 0;

    while (i < int(panel_buttons.size()))
        if (panel_buttons[i].m_button.get_element_ID() == element_ID)
            return i;
        else
            i++;

    return -1;
}

button panel_interface::button_get(int index){
    if (is_within_index(panel_buttons.size(),index))
        return panel_buttons[index].m_button;
    else {
        button null_button = button();
        return null_button;
    }
}

void panel_interface::button_transform(int index, std::vector<SDL_Rect> new_rects){
    if (is_within_index(panel_buttons.size(), index))
        panel_buttons[index].rectangles.swap(new_rects);
}

int panel_interface::button_get_over_x(int index){
    if (is_within_index(panel_buttons.size(), index))
        return panel_buttons.at(index).get_x_over() + viewport.x;
    else
        return -1;
}

int panel_interface::button_get_over_y(int index){
    if (is_within_index(panel_buttons.size(), index))
        return panel_buttons.at(index).get_y_over() + viewport.y;
    else
        return -1;
}

void panel_interface::camera_update(int x_move, int y_move){
    //SETTING INITIAL NEW POSITIONS
    //
    int new_x_pos = panel_camera.get_x_pos() + x_move;
    int new_y_pos = panel_camera.get_y_pos() + y_move;

    //CHECKING X BOUNDS
    //
    if (new_x_pos+panel_camera.get_x_size() > full_panel_size.get_x()){
        new_x_pos = full_panel_size.get_x() - panel_camera.get_x_size();
    } else if (new_x_pos < -viewport.x) {
        new_x_pos = -viewport.x;
    }

    //CHECKING Y BOUNDS
    //
    if (new_y_pos+panel_camera.get_y_size() > full_panel_size.get_y()){
        new_y_pos = full_panel_size.get_y() - panel_camera.get_y_size();
    } else if (new_y_pos < -viewport.y) {
        new_y_pos = -viewport.y;
    }

    //std::cout << "New camera x position: " << new_x_pos << std::endl;
    //std::cout << "New camera y position: " << new_y_pos << std::endl;

    ///UPDATING THE CAMERA IS BASED ON IF THERE WAS ANY DIFFERENTIATION IN POSITION

    //SETTING BUTTONS (BASED ON DIFFERENCE FROM PREVIOUS TO NEW POSITIONS
    //
    //std::cout << "Buttons will be moved by: x" << -(new_x_pos-panel_camera.get_x_pos()) << " ,y" << -(new_y_pos-panel_camera.get_y_pos()) << std::endl;;

    if (-(new_x_pos-panel_camera.get_x_pos()) != 0){
        for (int i = 0; i < int(panel_buttons.size()); i++)
            panel_buttons[i].move_rectangles(-(new_x_pos-panel_camera.get_x_pos()), 0);
        panel_camera.set_x_pos(new_x_pos);
    }

    if (-(new_y_pos-panel_camera.get_y_pos()) != 0){
        for (int i = 0; i < int(panel_buttons.size()); i++)
            panel_buttons[i].move_rectangles(0, -(new_y_pos-panel_camera.get_y_pos()));
        panel_camera.set_y_pos(new_y_pos);
    }
}

point panel_interface::camera_get_position(){
    return point(panel_camera.get_x_pos()+panel_camera.get_x_size()/2,panel_camera.get_y_pos()+panel_camera.get_y_size()/2);
}

SDL_Rect panel_interface::camera_get_rect(){
    return {panel_camera.get_x_pos(),panel_camera.get_y_pos(),panel_camera.get_x_size(),panel_camera.get_y_size()};
}

//ACTIVE AND LISTENING MANAGERS

bool panel_interface::get_active(){
    return is_active;
}

void panel_interface::set_active(){
    is_active = true;
}

void panel_interface::set_inactive(){
    int i;

    //DEACTIVATING EVENT STUFF

    //DEACTIVATING BUTTONS
    //
    for (i = 0; i < int(panel_buttons.size()); i++)
        button_set_active(i,false);

    //DEACTIVATING MEDIA

    //DEACTIVATING VISUAL MEDIA
    //
    for (i = 0; i < media_bin::visual_media_bin_length(); i++)
        visual_media_set_active(i,false);

    std::cout << "All interface elements have been set inactive" << std::endl;

    is_active = false;
}

bool panel_interface::get_listening(){
    return is_listened;
}

void panel_interface::set_listening(bool listening){
    is_listened = listening;
}

bool panel_interface::read_panel_procedures(int mouse_activity, bool playing, bool paused){
    int i = 0;
    bool interface_flag = true;

    while (i < int(panel_procedures.size()) && interface_flag){
        if (panel_procedures.at(i).comp_input_ID(mouse_activity))
            interface_flag = panel_procedures.at(i).procedure_do(playing, paused);

        i++;
    }

    return interface_flag;
}

void panel_interface::add_panel_procedure(room_procedure new_proc){
    panel_procedures.push_back(new_proc);
}

void panel_interface::remove_panel_procedure(int element_ID){
    for (int i = 0; i < int(panel_procedures.size()); i++)
        if (panel_procedures.at(i).comp_ID(element_ID)){
            panel_procedures.erase(panel_procedures.begin() + i);
            i--;
        }
}

