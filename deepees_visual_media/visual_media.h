#ifndef VISUAL_MEDIA_H_INCLUDED
#define VISUAL_MEDIA_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>

///BASE FOR TEXT, IMAGES, AND SHAPES TO INHERIT FROM (SAVES ON A BIT OF CODE)
//

class visual_media{
public:
    //CONSTRUCTOR
    visual_media(Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 0, int index = 0);
    //DESTRUCTOR
    ~visual_media();

    //SETTING RENDERER
    void set_renderer(SDL_Renderer* new_renderer);
    SDL_Renderer* get_renderer();
    bool renderer_present();
    //
    //TEXTURE LOADERS AND CLEARER
    //
    //IMAGE TEXTURE LOAD AND UNLOADER
    bool load_as_image(const char image_path[]);
    bool load_as_text(TTF_Font* given_font, const char content[], bool is_wrapping, Uint32 wrap_length);
    bool load_as_blank(SDL_TextureAccess access_type = SDL_TEXTUREACCESS_STATIC);
    bool set_as_render_target();

    void wipe();
    //
    //RENDERING
    //
    bool render(int x = 0, int y = 0, SDL_Rect render_clip = {}, double rotation = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);
    //
    //GENERAL VARIABLES
    //
    //SIZE ACCESSORS AND MUTATORS
    //
    //x
    int get_x_size();
    void set_x_size(int new_x_size = 1);
    //y
    int get_y_size();
    void set_y_size(int new_y_size = 1);

    //ACTIVE ACCESSOR AND MUTATOR
    //
    //bool get_active();
    //void set_active(bool new_active = false);

    //COLOR ACCESSOR AND MUTATOR
    //
    SDL_Color get_color();
    void set_color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    bool set_color_mod();
    //red
    Uint8 get_red();
    void set_red(Uint8 red);
    //green
    Uint8 get_green();
    void set_green(Uint8 green);
    //blue
    Uint8 get_blue();
    void set_blue(Uint8 blue);
    //alpha
    Uint8 get_alpha();
    void set_alpha(Uint8 alpha);
    bool set_alpha_mod(Uint8 bonus_a = 0x00);
    //hsl
    Uint8 get_hue();
    void set_hue(int new_hue);
    //
    Uint8 get_saturation();
    void set_saturation(int new_sat);
    //
    Uint8 get_luminosity();
    void set_luminosity(int new_lum);

    bool set_blend_mode(SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE);

    //ELEMENT ID ACCESSOR AND MUTATOR
    //
    int get_element_ID();
    void set_element_ID(int new_ID = 0);
    int compare_element_ID(int other_element_ID = 0);

    //PIXEL MANIPULATION
    //
    //LOCK
    //
    bool get_locked();
    bool set_locked(bool lock = false);
    //
    //PIXELS
    //
    Uint32 get_pixel(unsigned int x = 0, unsigned int y = 0);
    void set_pixel(Uint32 new_pixel, unsigned int x, unsigned int y);
    //
    void* get_pixels();
    void set_pixels(void* new_pixels);
    //
    //PITCH, PIXEL LENGTH, MOD AREA
    //
    int get_pitch();
    int get_pixel_length();
    //
    SDL_Rect get_lock_rect();
    void set_lock_rect(SDL_Rect new_lock);

private:
    void hsl_to_rgb();
    void rgb_to_hsl();

    //RENDERER IN REFERENCE
    SDL_Renderer* renderer;

    //TEXTURE
    SDL_Texture* texture;

    //FOR MODIFICATION
    void* pixels;
    SDL_Rect lock_rect;
    int pitch;
    int pixel_length;
    bool is_locked;

    //SIZE OF MEDIA
    int x_size, y_size;

    //COLOR OF MEDIA
    SDL_Color color_and_transparency = {0xFF,0xFF,0xFF,0xFF};

    //HSL
    Uint8 hue = 239;
    Uint8 saturation = 240;
    Uint8 luminosity = 240;

    //ELEMENT ID
    int element_ID;
};

#endif // VISUAL_MEDIA_H_INCLUDED
