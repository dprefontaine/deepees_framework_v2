#ifndef MEDIA_BIN_H_INCLUDED
#define MEDIA_BIN_H_INCLUDED

///THIS IS TO SAVE ON MEDIA CODE FOR BOTH ROOMS AND INTERFACE BLOCKS AND MAKE IT EASIER TO MODIFY MEDIA IF NEEDED

#include "image_sources.h"

inline bool compare_layer(const rendering_bundle& i, const rendering_bundle& j);


class media_bin {
public:
    ///CONSTRUCTOR
    media_bin();

    ///DESTRUCTOR
    ~media_bin();

    //RENDERING AND RENDERER
    //
    bool render_bin(int x_pos = 0, int y_pos = 0, int x_size = 1, int y_size = 1);
    void set_renderer(SDL_Renderer* new_renderer);
    //
    //FOR GETTING MEDIA OUTSIDE OF MEDIA BIN
    //
    visual_media& visual_media_get(int index = 0);
    rendering_bundle& visual_media_bundle_get(int index = 0);

    int visual_media_bin_length();
    int audio_media_bin_length();
    //
    ///VISUAL MEDIA MANAGER
    //
    bool visual_media_get_active(int index = 0);
    bool visual_media_set_active(int index = 0, bool active = false);

    int visual_media_find_by_ID(int element_ID = 0);

    int visual_media_get_x_size(int index = 0);
    bool visual_media_set_x_size(int index = 0, int x_size = 1);
    int visual_media_get_y_size(int index = 0);
    bool visual_media_set_y_size(int index = 0, int y_size = 1);

    SDL_Color visual_media_get_color(int index = 0);
    bool visual_media_set_color(int index = 0, Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 0);
    bool visual_media_mod_color(int index = 0);
    Uint8 visual_media_get_red(int index = 0);
    bool visual_media_set_red(int index = 0, Uint8 red = 0);
    Uint8 visual_media_get_green(int index = 0);
    bool visual_media_set_green(int index = 0, Uint8 green = 0);
    Uint8 visual_media_get_blue(int index = 0);
    bool visual_media_set_blue(int index = 0, Uint8 blue = 0);

    bool visual_media_set_blend_mode(int index = 0, SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE);

    bool visual_media_mod_alpha(int index = 0);

    double visual_media_get_rotation(int index = 0);
    bool visual_media_set_rotation(int index = 0, double new_rotation = 0.0);

    SDL_RendererFlip visual_media_get_flip(int index = 0);
    bool visual_media_set_flip(int index = 0, SDL_RendererFlip new_flip = SDL_FLIP_NONE);

    int visual_media_get_rendering_layer(int index = 0);
    bool visual_media_set_rendering_layer(int index = 0, int new_layer = 0);
    void visual_media_sort();
    //
    ///RENDERING BUNDLE MANAGER
    //
    int visual_media_spot_new(rendering_bundle new_spot = rendering_bundle(0,{0,0,0,0},0.0,SDL_FLIP_NONE,0));
    int visual_media_spot_find(int element_ID = 0);
    SDL_Rect visual_media_spot_get(int index = 0);
    void visual_media_spot_set(int index = 0, SDL_Rect new_rect = {0,0,0,0});
    bool visual_media_spot_remove(int index = 0);
    bool visual_media_spot_clear();

    int visual_media_point_add(int index = 0, int new_x = 0, int new_y = 0);
    int visual_media_point_find(int index = 0, int x_obs = 0, int y_obs = 0);
    point* visual_media_point_get(int index = 0, int point_index = 0);
    bool visual_media_point_remove(int index = 0 , int point_index = 0);
    bool visual_media_point_clear(int index = 0);

protected:

    SDL_Renderer* renderer = NULL;

    image_source_controller* images = NULL;

    //std::vector<visual_media> visual_media_bin;
    std::vector<rendering_bundle> media_render_bin;
};

#endif // MEDIA_BIN_H_INCLUDED
