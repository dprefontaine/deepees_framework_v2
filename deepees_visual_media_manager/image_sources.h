#ifndef IMAGE_SOURCES_H_INCLUDED
#define IMAGE_SOURCES_H_INCLUDED

#pragma once

#include "media_set.h"
#include "camera.h"
#include <algorithm>


class image_source_controller {
public:
    image_source_controller(){}

    virtual ~image_source_controller(){}

    void set_renderer(SDL_Renderer* new_renderer = NULL);

    int visual_media_new(visual_media n_media);
    bool visual_media_load_as_image(int index, const char image_path[]);
    bool visual_media_load_as_text(int index, TTF_Font* font, const char content[], bool is_wrapping, Uint32 wrap_count);
    bool load_as_blank(int index, SDL_TextureAccess texture_access);

    bool visual_media_wipe(int index);
    void visual_media_wipe_all();

    bool visual_media_set_renderer_target(int index);
    void visual_media_free_renderer_target();

    void visual_media_set_x_size(int index, int x_size);
    int visual_media_get_x_size(int index);

    void visual_media_set_y_size(int index, int y_size);
    int visual_media_get_y_size(int index);

    int visual_media_find_by_ID(int element_ID);

    inline int size(){return (int)visual_media_bin.size();}

    visual_media& visual_media_get(int index);

    int visual_media_bin_length();

    ///VISUAL MEDIA (SHARED)

    SDL_Color visual_media_get_color(int index);
    bool visual_media_set_color(int index, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    bool visual_media_mod_color(int index);

    Uint8 visual_media_get_red(int index);
    bool visual_media_set_red(int index, Uint8 red);

    Uint8 visual_media_get_green(int index);
    bool visual_media_set_green(int index, Uint8 green);

    Uint8 visual_media_get_blue(int index);
    bool visual_media_set_blue(int index, Uint8 blue);

    bool visual_media_set_blend_mode(int index, SDL_BlendMode blend_mode);

    Uint8 visual_media_get_alpha(int index);
    bool visual_media_set_alpha(int index, Uint8 alpha);
    bool visual_media_mod_alpha(int index);

    bool visual_media_get_locked(int index);
    bool visual_media_set_locked(int index, bool lock);

    void* visual_media_get_pixels(int index);
    Uint32 visual_media_get_pixel(int index, unsigned int x, unsigned int y);
    void visual_media_set_pixels(int index, void* new_pixels);

    void visual_media_set_pixel(int index, Uint32 pixel, unsigned int x, unsigned int y);
    int visual_media_get_pitch(int index);

    int visual_media_get_pixel_length(int index);
    SDL_Rect visual_media_get_lock_rect(int index);
    void visual_media_set_lock_rect(int index, SDL_Rect new_lock);

    std::vector<visual_media> visual_media_bin = std::vector<visual_media>();
    SDL_Renderer* renderer = NULL;
};

#endif // IMAGE_SOURCES_H_INCLUDED
