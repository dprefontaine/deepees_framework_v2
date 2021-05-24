#include "media_bin.h"

inline bool compare_layer(const rendering_bundle& i, const rendering_bundle& j){
    return i.rendering_layer < j.rendering_layer;
}

//CONSTRUCTOR
media_bin::media_bin(){
    std::vector<visual_media> visual_media_bin = std::vector<visual_media>();
    std::vector<rendering_bundle> media_render_bin = std::vector<rendering_bundle>();
    red_filter = green_filter = blue_filter = alpha_filter = filter_hue = filter_saturation = filter_luminosity = 0;
}

//DESTRUCTOR
media_bin::~media_bin(){
    //WIPING VISUAL MEDIA (SETTING RENDERER TO NULL SHOULD BE PASSED BY ALL)
    visual_media_wipe_all();

    //FREEING MEMORY
    //
    //TEMP VECTORS
    std::vector<visual_media>().swap(visual_media_bin);
    std::vector<rendering_bundle>().swap(media_render_bin);
}

void media_bin::set_renderer(SDL_Renderer* new_renderer){
    renderer = new_renderer;

    for (unsigned int i = 0; i < visual_media_bin.size(); i++)
        visual_media_bin.at(i).set_renderer(new_renderer);
}

//RENDERING

bool media_bin::render_bin(int x_pos, int y_pos, int x_size, int y_size){
    bool render_flag = true;

    for (int i = 0; i < int(media_render_bin.size()); i++){
        if (media_render_bin[i].is_active)
            render_flag = media_render_bin[i].render_bundle(&visual_media_get(media_render_bin[i].visual_media_pointer), x_pos, y_pos, x_size, y_size);
    }

    return render_flag;
}

//GENERAL BINS

visual_media& media_bin::visual_media_get(int index){
    return visual_media_bin.at(index);
}

rendering_bundle& media_bin::visual_media_bundle_get(int index){
    return media_render_bin.at(index);
}

int media_bin::visual_media_bin_length(){
    return visual_media_bin.size();
}

//IMAGE MANAGERS

int media_bin::visual_media_new(visual_media n_media){
    n_media.set_renderer(renderer);
    visual_media_bin.push_back(n_media);

    return visual_media_bin.size()-1;
}

bool media_bin::visual_media_load_as_image(int index, const char image_path[]){
    bool load_flag = false;

    if (is_within_index(visual_media_bin.size(),index)){
        load_flag = visual_media_bin[index].load_as_image(image_path);

        //SET COLOR AND ALPHA MODS
        if (load_flag){
            visual_media_mod_color(index);
            visual_media_mod_alpha(index);
        }
    }

    return load_flag;
}

bool media_bin::visual_media_load_as_text(int index, TTF_Font* font, const char content[], bool is_wrapping, Uint32 wrap_count){
    bool load_flag = false;

    if (is_within_index(visual_media_bin.size(),index)){
        load_flag = visual_media_bin[index].load_as_text(font, content, is_wrapping, wrap_count);

        //SET COLOR AND ALPHA MODS
        if (load_flag){
            visual_media_mod_color(index);
            visual_media_mod_alpha(index);
        }
    }

    return load_flag;
}

bool media_bin::visual_media_load_as_blank(int index, SDL_TextureAccess texture_access){
    bool load_flag = false;

    if (is_within_index(visual_media_bin.size(),index)){
        load_flag = visual_media_bin[index].load_as_blank(texture_access);

        if (load_flag){
            visual_media_mod_color(index);
            visual_media_mod_alpha(index);
        }
    }

    return load_flag;
}

bool media_bin::visual_media_wipe(int index){
    if (is_within_index(visual_media_bin.size(),index)){
        visual_media_bin[index].wipe();
        return true;
    }

    return false;
}

void media_bin::visual_media_wipe_all(){
    for (unsigned int i = 0; i < visual_media_bin.size(); i++)
        visual_media_bin[i].wipe();
}

bool media_bin::visual_media_set_renderer_target(int index){
    if (is_within_index(media_render_bin.size(),index)){
        return visual_media_bin[index].set_as_render_target();
    }

    return false;
}

void media_bin::visual_media_free_renderer_target(){
    SDL_SetRenderTarget(renderer,NULL);
}

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

int media_bin::visual_media_find_by_ID(int element_ID){
    int i = 0;

    while (i < int(visual_media_bin.size()))
        if (visual_media_bin[i].get_element_ID() == element_ID)
            return i;
        else
            i++;

    return -1;
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

int media_bin::visual_media_get_x_size(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_x_size();
    else
        return -1;
}

bool media_bin::visual_media_set_x_size(int index, int x_size){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_x_size(x_size);
    else
        return false;

    return true;
}

int media_bin::visual_media_get_y_size(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_y_size();
    else
        return -1;
}

bool media_bin::visual_media_set_y_size(int index, int y_size){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_y_size(y_size);
    else
        return false;

    return true;
}

SDL_Color media_bin::visual_media_get_color(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_color();
    else
        return {0x00,0x00,0x00,0x00};
}

bool media_bin::visual_media_set_color(int index, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_color(red, green, blue, alpha);
    else
        return false;

    return visual_media_mod_color(index);
}

bool media_bin::visual_media_mod_color(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return (visual_media_bin[index].set_color_mod());
    else
        return false;
}

Uint8 media_bin::visual_media_get_red(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_red();
    else
        return 0;
}

bool media_bin::visual_media_set_red(int index, Uint8 red){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_red(red);
    else
        return false;

    return visual_media_mod_color(index);
}

Uint8 media_bin::visual_media_get_green(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_green();
    else
        return 0;
}

bool media_bin::visual_media_set_green(int index, Uint8 green){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_green(green);
    else
        return false;

    return visual_media_mod_color(index);
}

Uint8 media_bin::visual_media_get_blue(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_blue();
    else
        return 0;
}

bool media_bin::visual_media_set_blue(int index, Uint8 blue){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_blue(blue);
    else
        return false;

    return visual_media_mod_color(index);
}

Uint8 media_bin::visual_media_get_hue(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_hue();
    else
        return 0;
}

bool media_bin::visual_media_set_hue(int index, int hue){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_hue(hue);
    else
        return false;

    visual_media_mod_color(index);

    return true;
}

Uint8 media_bin::visual_media_get_saturation(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_saturation();
    else
        return 0;
}

bool media_bin::visual_media_set_saturation(int index, int saturation){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_saturation(saturation);
    else
        return false;

    visual_media_mod_color(index);

    return true;
}

Uint8 media_bin::visual_media_get_luminosity(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_luminosity();
    else
        return 0;
}

bool media_bin::visual_media_set_luminosity(int index, int luminosity){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_luminosity(luminosity);
    else
        return false;

    visual_media_mod_color(index);

    return true;
}

bool media_bin::visual_media_set_blend_mode(int index, SDL_BlendMode blend_mode){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_blend_mode(blend_mode);
    else
        return false;

    return true;
}

Uint8 media_bin::visual_media_get_alpha(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_alpha();
    else
        return 0;
}

bool media_bin::visual_media_set_alpha(int index, Uint8 alpha){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_alpha(alpha);
    else
        return false;

    visual_media_mod_alpha(index);

    return true;
}

bool media_bin::visual_media_mod_alpha(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return (visual_media_bin[index].set_alpha_mod(alpha_filter));
    else
        return false;
}

double media_bin::visual_media_get_rotation(int index){
    if (is_within_index(media_render_bin.size(),index))
        return media_render_bin[index].rotation;
    else
        return -1.0;
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

int media_bin::visual_media_get_element_ID(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_element_ID();
    else
        return -1;
}

bool media_bin::visual_media_set_element_ID(int index, int new_ID){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_element_ID(new_ID);
    else
        return false;

    return true;
}

bool media_bin::visual_media_get_locked(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_locked();

    return false;
}

bool media_bin::visual_media_set_locked(int index, bool lock){
    if (is_within_index(visual_media_bin.size(), index)){
        visual_media_bin[index].set_locked(lock);
        return true;
    }

    return false;
}

void* media_bin::visual_media_get_pixels(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pixels();

    return nullptr;
}

Uint32 media_bin::visual_media_get_pixel(int index, unsigned int x, unsigned int y){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pixel(x, y);

    return 0;
}

void media_bin::visual_media_set_pixels(int index, void* new_pixels){
    if (is_within_index(visual_media_bin.size(), index))
        visual_media_bin[index].set_pixels(new_pixels);
}

void media_bin::visual_media_set_pixel(int index, Uint32 pixel, unsigned int x, unsigned int y){
    if (is_within_index(visual_media_bin.size(), index))
        visual_media_bin[index].set_pixel(pixel, x, y);
}

int media_bin::visual_media_get_pitch(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pitch();

    return 0;
}

int media_bin::visual_media_get_pixel_length(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pixel_length();

    return 0;
}

SDL_Rect media_bin::visual_media_get_lock_rect(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_lock_rect();

    return {0,0,0,0};
}

void media_bin::visual_media_set_lock_rect(int index, SDL_Rect new_lock){
    if (is_within_index(visual_media_bin.size(), index))
        visual_media_bin[index].set_lock_rect(new_lock);
}
