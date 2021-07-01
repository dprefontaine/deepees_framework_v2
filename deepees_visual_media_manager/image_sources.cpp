#include "image_sources.h"

void image_source_controller::set_renderer(SDL_Renderer* new_renderer){
    renderer = new_renderer;
}

int image_source_controller::visual_media_new(visual_media n_media){
    visual_media_bin.push_back(n_media);
    visual_media_bin.back().set_renderer(renderer);

    return visual_media_bin.size()-1;
}

bool image_source_controller::visual_media_load_as_image(int index, const char image_path[]){
    bool load_flag = false;

    if (is_within_index(visual_media_bin.size(),index)){
        std::cout << "loading: " << (load_flag = visual_media_bin[index].load_as_image(image_path)) << std::endl;

        //SET COLOR AND ALPHA MODS
        if (load_flag){
            visual_media_bin[index].set_color_mod();
            visual_media_bin[index].set_alpha_mod();
        }
    }

    return load_flag;
}

bool image_source_controller::visual_media_load_as_text(int index, TTF_Font* font, const char content[], bool is_wrapping, Uint32 wrap_count){
    bool load_flag = false;

    if (is_within_index(visual_media_bin.size(),index)){
        load_flag = visual_media_bin[index].load_as_text(font, content, is_wrapping, wrap_count);

        //SET COLOR AND ALPHA MODS
        if (load_flag){
            visual_media_bin[index].set_color_mod();
            visual_media_bin[index].set_alpha_mod();
        }
    }

    return load_flag;
}

bool image_source_controller::load_as_blank(int index, SDL_TextureAccess texture_access){
    bool load_flag = false;

    if (is_within_index(visual_media_bin.size(),index)){
        load_flag = visual_media_bin[index].load_as_blank(texture_access);

        if (load_flag){
            visual_media_bin[index].set_color_mod();
            visual_media_bin[index].set_alpha_mod();
        }
    }

    return load_flag;
}

bool image_source_controller::visual_media_wipe(int index){
    if (is_within_index(visual_media_bin.size(),index)){
        visual_media_bin[index].wipe();
        return true;
    }

    return false;
}

void image_source_controller::visual_media_wipe_all(){
    for (unsigned int i = 0; i < visual_media_bin.size(); i++)
        visual_media_bin[i].wipe();
}

bool image_source_controller::visual_media_set_renderer_target(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].set_as_render_target();

    return false;
}

void image_source_controller::visual_media_free_renderer_target(){
    SDL_SetRenderTarget(renderer,NULL);
}

void image_source_controller::visual_media_set_x_size(int index, int x_size){
    try {
        return visual_media_bin.at(index).set_x_size(x_size);
    } catch (std::out_of_range e){
    }
}

int image_source_controller::visual_media_get_x_size(int index){
    try {
        return visual_media_bin.at(index).get_x_size();
    } catch (std::out_of_range e){
        return -1;
    }
}

void image_source_controller::visual_media_set_y_size(int index, int y_size){
    try {
        return visual_media_bin.at(index).set_y_size(y_size);
    } catch (std::out_of_range e){
    }
}

int image_source_controller::visual_media_get_y_size(int index){
    try {
        return visual_media_bin.at(index).get_y_size();
    } catch (std::out_of_range e){
        return -1;
    }
}

int image_source_controller::visual_media_find_by_ID(int element_ID){
    int i = 0;

    while (i < int(visual_media_bin.size()))
        if (visual_media_bin[i].get_element_ID() == element_ID)
            return i;
        else
            i++;

    return -1;
}

//GENERAL BINS

visual_media& image_source_controller::visual_media_get(int index){
    return visual_media_bin.at(index);
}

int image_source_controller::visual_media_bin_length(){
    return visual_media_bin.size();
}

///VISUAL MEDIA (SHARED)

SDL_Color image_source_controller::visual_media_get_color(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_color();
    else
        return {0x00,0x00,0x00,0x00};
}

bool image_source_controller::visual_media_set_color(int index, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_color(red, green, blue, alpha);
    else
        return false;

    return visual_media_mod_color(index);
}

bool image_source_controller::visual_media_mod_color(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return (visual_media_bin[index].set_color_mod());
    else
        return false;
}

Uint8 image_source_controller::visual_media_get_red(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_red();
    else
        return 0;
}

bool image_source_controller::visual_media_set_red(int index, Uint8 red){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_red(red);
    else
        return false;

    return visual_media_mod_color(index);
}

Uint8 image_source_controller::visual_media_get_green(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_green();
    else
        return 0;
}

bool image_source_controller::visual_media_set_green(int index, Uint8 green){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_green(green);
    else
        return false;

    return visual_media_mod_color(index);
}

Uint8 image_source_controller::visual_media_get_blue(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_blue();
    else
        return 0;
}

bool image_source_controller::visual_media_set_blue(int index, Uint8 blue){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_blue(blue);
    else
        return false;

    return visual_media_mod_color(index);
}

bool image_source_controller::visual_media_set_blend_mode(int index, SDL_BlendMode blend_mode){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_blend_mode(blend_mode);
    else
        return false;

    return true;
}

Uint8 image_source_controller::visual_media_get_alpha(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return visual_media_bin[index].get_alpha();
    else
        return 0;
}

bool image_source_controller::visual_media_set_alpha(int index, Uint8 alpha){
    if (is_within_index(visual_media_bin.size(),index))
        visual_media_bin[index].set_alpha(alpha);
    else
        return false;

    visual_media_mod_alpha(index);

    return true;
}

bool image_source_controller::visual_media_mod_alpha(int index){
    if (is_within_index(visual_media_bin.size(),index))
        return (visual_media_bin[index].set_alpha_mod());
    else
        return false;
}

bool image_source_controller::visual_media_get_locked(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_locked();

    return false;
}

bool image_source_controller::visual_media_set_locked(int index, bool lock){
    if (is_within_index(visual_media_bin.size(), index)){
        visual_media_bin[index].set_locked(lock);
        return true;
    }

    return false;
}

void* image_source_controller::visual_media_get_pixels(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pixels();

    return nullptr;
}

Uint32 image_source_controller::visual_media_get_pixel(int index, unsigned int x, unsigned int y){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pixel(x, y);

    return 0;
}

void image_source_controller::visual_media_set_pixels(int index, void* new_pixels){
    if (is_within_index(visual_media_bin.size(), index))
        visual_media_bin[index].set_pixels(new_pixels);
}

void image_source_controller::visual_media_set_pixel(int index, Uint32 pixel, unsigned int x, unsigned int y){
    if (is_within_index(visual_media_bin.size(), index))
        visual_media_bin[index].set_pixel(pixel, x, y);
}

int image_source_controller::visual_media_get_pitch(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pitch();

    return 0;
}

int image_source_controller::visual_media_get_pixel_length(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_pixel_length();

    return 0;
}

SDL_Rect image_source_controller::visual_media_get_lock_rect(int index){
    if (is_within_index(visual_media_bin.size(), index))
        return visual_media_bin[index].get_lock_rect();

    return {0,0,0,0};
}

void image_source_controller::visual_media_set_lock_rect(int index, SDL_Rect new_lock){
    if (is_within_index(visual_media_bin.size(), index))
        visual_media_bin[index].set_lock_rect(new_lock);
}

