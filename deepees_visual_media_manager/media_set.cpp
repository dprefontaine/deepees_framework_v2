#include "media_set.h"

image_point::image_point(int visual_media_room_point, int visual_media_panel_point, int visual_media_index_point, int render_spot_room_point, int render_spot_panel_point, int render_spot_index_point, int element_ID){
    this->visual_media_room_point = visual_media_room_point;
    this->visual_media_panel_point = visual_media_panel_point;
    this->visual_media_index_point = visual_media_index_point;
    this->render_spot_room_point = render_spot_room_point;
    this->render_spot_panel_point = render_spot_panel_point;
    this->render_spot_index_point = render_spot_index_point;
    this->element_ID = element_ID;
}

image_point::~image_point(){
    if (image_animations != NULL){
        delete image_animations;
        image_animations = NULL;
    }
}

image_source_point::image_source_point(int visual_media_room_point, int visual_media_panel_point, int visual_media_index_point){
    this->visual_media_room_point = visual_media_room_point;
    this->visual_media_panel_point = visual_media_panel_point;
    this->visual_media_index_point = visual_media_index_point;
}

button_bundle::button_bundle(int element_ID){
    m_button = button(element_ID);
    rectangles = std::vector<SDL_Rect>();
    x_over = y_over = 0;
}

button_bundle::~button_bundle(){
    std::vector<SDL_Rect>().swap(rectangles);

    m_button.~button();
    rectangles.~vector();
}

int button_bundle::search_rectangle(SDL_Rect obs_rect){
    unsigned int i = 0;

    while (i < rectangles.size()){
        if (rectangles.at(i).x == obs_rect.x &&
            rectangles.at(i).y == obs_rect.y &&
            rectangles.at(i).w == obs_rect.w &&
            rectangles.at(i).h == obs_rect.h)
                return i;
        else
            i++;
    }

    return -i;
}

void button_bundle::move_rectangles(int x_move, int y_move){
     //std::cout << "Moving by: x" << x_move << " y" << y_move << std::endl;

     for (int i = 0; i < int(rectangles.size()); i++){
        rectangles[i].x = rectangles[i].x + x_move;
        rectangles[i].y = rectangles[i].y + y_move;
    }
}

bool button_bundle::is_over(int mouse_x, int mouse_y, int vp_x, int vp_y){
    //GO THROUGH ALL THE RECTANGLES UNTIL CONFIRMED OVER
    bool over_flag = false;
    unsigned int current_rectangle = 0;

    //CHECK THE COORDINATES
    while (!over_flag && current_rectangle < rectangles.size()){
        if ((mouse_x-vp_x > rectangles.at(current_rectangle).x && mouse_x-vp_x < (rectangles.at(current_rectangle).x + rectangles.at(current_rectangle).w)) &&
            (mouse_y-vp_y > rectangles.at(current_rectangle).y && mouse_y-vp_y < (rectangles.at(current_rectangle).y + rectangles.at(current_rectangle).h))){
                over_flag = true;

                //UPDATING MOUSE POSITION RELATIVE TO THE BUTTON'S ORIGIN
                x_over = mouse_x-vp_x*2 - rectangles.at(0).x;
                y_over = mouse_y-vp_y*2 - rectangles.at(0).y;
        } else
            current_rectangle++;
    }

    return over_flag;
}

int button_bundle::get_x_over(){
    return x_over;
}

int button_bundle::get_y_over(){
    return y_over;
}

rendering_bundle::rendering_bundle(int new_pointer, SDL_Rect spot, double new_rotation, SDL_RendererFlip flip, int layer, int ID){
    visual_media_pointer = new_pointer;
    render_spot = spot;
    render_points = std::vector<point>();
    is_active = false;
    rotation = new_rotation;
    flip_state = flip;
    rendering_layer = layer;
    element_ID = ID;
}

rendering_bundle::~rendering_bundle(){
    std::vector<point> temp_point = std::vector<point>();
    render_points.swap(temp_point);
    render_points.~vector();
}

bool rendering_bundle::render_bundle(visual_media* media_to_render, int x_pos, int y_pos, int x_size, int y_size){
    bool bundle_flag = true;
    int i, render_x, render_y;

    if (is_active && media_to_render != NULL)
        for (i = 0; i < int(render_points.size()); i++){
            render_x = render_points[i].get_x()-x_pos-render_spot.w/2;
            render_y = render_points[i].get_y()-y_pos-render_spot.h/2;

            //CHECKING IF RENDERING SPOT IS WITHIN BOUNDS
            //
            if (render_x + render_spot.w > 0 && render_x < x_size && render_y + render_spot.h  > 0 && render_y < y_size)
                bundle_flag = media_to_render->render(render_x,render_y,render_spot,rotation,flip_state);
        }

    return bundle_flag;
};

bool rendering_bundle::search_spot(SDL_Rect given_spot){
    if (given_spot.x == render_spot.x && given_spot.y == render_spot.y &&
        given_spot.w == render_spot.w && given_spot.h == render_spot.h)
            return true;

    return false;
};

int rendering_bundle::search_point(int x, int y){
    for (int i = 0; i < int(render_points.size()); i++)
        if (render_points[i].get_x() == x &&
            render_points[i].get_y() == y)
            return i;

    return -1;
}
