#include "media_bin.h"

inline bool compare_layer(const rendering_bundle& i, const rendering_bundle& j){
    return i.rendering_layer < j.rendering_layer;
}

//CONSTRUCTOR
media_bin::media_bin(){
    std::vector<visual_media> visual_media_bin = std::vector<visual_media>();
    std::vector<rendering_bundle> media_render_bin = std::vector<rendering_bundle>();
}

//DESTRUCTOR
media_bin::~media_bin(){
    //UNPOINTING FROM IMAGE SOURCES SINCE FREEING MEMORY WILL TAKE PLACE IN MAIN CONTROLLER
    images = NULL; //(WIPE ALL IN MAIN CONTROLLER)

    //FREEING MEMORY
    //
    //TEMP VECTORS
    std::vector<rendering_bundle>().swap(media_render_bin);
}

void media_bin::set_renderer(SDL_Renderer* new_renderer){
    renderer = new_renderer;

    images->set_renderer(new_renderer);
}

//RENDERING

bool media_bin::render_bin(int x_pos, int y_pos, int x_size, int y_size){
    bool render_flag = true;

    for (unsigned int i = 0; i < media_render_bin.size(); i++){
        visual_media* image_returned;

        try {
            //std::cout << media_render_bin[i].visual_media_pointer << " ";
            image_returned = &images->visual_media_bin.at(media_render_bin[i].visual_media_pointer);
        } catch (std::out_of_range e){
            image_returned = NULL;
        }

        if (media_render_bin[i].is_active && image_returned != NULL){
            //std::cout << "rendering..." << std::endl;
            render_flag = media_render_bin[i].render_bundle(image_returned, x_pos, y_pos, x_size, y_size);
        } else if (image_returned == NULL){
            //NOTIFY ERROR AT SOME POINT
            std::cout << "failed to render image!" << std::endl;
            render_flag = false;
        }
    }

    //std::cout << std::endl;

    return render_flag;
}

//GENERAL BINS
rendering_bundle& media_bin::visual_media_bundle_get(int index){
    return media_render_bin.at(index);
}

int media_bin::visual_media_bin_length(){
    return images->size();
}

//IMAGE MANAGERS
//(ASSUMING NOT IN THE IMAGE SOURCES STRUCT)

bool media_bin::visual_media_get_active(int index){
    if (is_within_index(media_render_bin.size(),index))
        return media_render_bin[index].is_active;

    return false;
}

bool media_bin::visual_media_set_active(int index, bool active){
    //Check if index exists
    //
    if (is_within_index(media_render_bin.size(),index)){
        media_render_bin[index].is_active = active;
        return true;
    }

    return false;
}


int media_bin::visual_media_spot_new(rendering_bundle new_bundle){
    media_render_bin.push_back(new_bundle);

    return media_render_bin.size()-1;
}

int media_bin::visual_media_spot_find(int element_ID){
    int return_index = -1;

    for (unsigned int i = 0; i < media_render_bin.size(); i++)
        if (media_render_bin.at(i).element_ID == element_ID)
            return_index = i;

    return return_index;
}

SDL_Rect media_bin::visual_media_spot_get(int index){
    if (is_within_index(media_render_bin.size(), index))
        return media_render_bin.at(index).render_spot;

    return {-1,-1,-1,-1};
}

void media_bin::visual_media_spot_set(int index, SDL_Rect new_rect){
    if (is_within_index(media_render_bin.size(), index))
        media_render_bin.at(index).render_spot = new_rect;
}

bool media_bin::visual_media_spot_remove(int index){
    if (is_within_index(media_render_bin.size(),index)){
        media_render_bin.erase(media_render_bin.begin()+index);

        return true;
    }

    return false;
}

bool media_bin::visual_media_spot_clear(){
    media_render_bin.clear();
    return true;
}

int media_bin::visual_media_point_add(int index, int new_x, int new_y){
    if (is_within_index(media_render_bin.size(),index)){
        media_render_bin[index].render_points.push_back(point(new_x,new_y));
        return media_render_bin[index].render_points.size()-1;
    }

    return -1;
}

int media_bin::visual_media_point_find(int index, int x_obs, int y_obs){
    if (is_within_index(media_render_bin.size(),index))
        return media_render_bin[index].search_point(x_obs, y_obs);

    return -1;
}

point* media_bin::visual_media_point_get(int index, int point_index){
    point* return_point = NULL;

    try {
        return_point = &media_render_bin.at(index).render_points.at(point_index);
    } catch (std::out_of_range e){
        return_point = NULL;
    }

    return return_point;
}

bool media_bin::visual_media_point_remove(int index, int point_index){
    if (is_within_index(media_render_bin.size(),index)){
        media_render_bin[index].render_points.erase(media_render_bin[index].render_points.begin() + point_index);
        return true;
    }

    return false;
}

bool media_bin::visual_media_point_clear(int index){
    if (is_within_index(media_render_bin.size(),index)){
        media_render_bin[index].render_points.clear();
        return true;
    }

    return false;
}

///VISUAL MEDIA (SHARED)

double media_bin::visual_media_get_rotation(int index){
    if (is_within_index(media_render_bin.size(),index))
        return media_render_bin[index].rotation;
    else
        return 0;

    return 0;
}

bool media_bin::visual_media_set_rotation(int index, double new_rotation){
    if (is_within_index(media_render_bin.size(),index))
        media_render_bin[index].rotation = new_rotation;
    else
        return false;

    return true;
}

SDL_RendererFlip media_bin::visual_media_get_flip(int index){
    if (is_within_index(media_render_bin.size(),index))
        return media_render_bin[index].flip_state;
    else
        return SDL_FLIP_NONE;
}

bool media_bin::visual_media_set_flip(int index, SDL_RendererFlip new_flip){
    if (is_within_index(media_render_bin.size(),index))
        media_render_bin[index].flip_state = new_flip;
    else
        return false;

    return true;
}

int media_bin::visual_media_get_rendering_layer(int index){
    if (is_within_index(media_render_bin.size(),index))
        return media_render_bin[index].rendering_layer;
    else
        return -1;
}

bool media_bin::visual_media_set_rendering_layer(int index, int new_layer){
    if (is_within_index(media_render_bin.size(),index))
        media_render_bin[index].rendering_layer = new_layer;
    else
        return false;

    return true;
}

void media_bin::visual_media_sort(){
    std::sort(media_render_bin.begin(),media_render_bin.end(),compare_layer);
}

