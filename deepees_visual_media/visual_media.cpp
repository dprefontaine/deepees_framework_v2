#include "visual_media.h"

visual_media::visual_media(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, int index){
    renderer = NULL;
    texture = NULL;
    pixels = NULL;
    lock_rect = {0,0,0,0};
    x_size = y_size = pitch = 0;
    is_locked = false;
    color_and_transparency.r = red;
    color_and_transparency.g = green;
    color_and_transparency.b = blue;
    color_and_transparency.a = alpha;
    element_ID = index;
}

visual_media::~visual_media(){

}

void visual_media::set_renderer(SDL_Renderer* new_renderer){
    renderer = new_renderer;
}

SDL_Renderer* visual_media::get_renderer(){
    return renderer;
}

bool visual_media::renderer_present(){
    return (renderer != NULL);
}

void visual_media::wipe(){
    if (texture != NULL){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }

    if (pixels != NULL){
        set_locked(false);
    }
}

bool visual_media::render(int x, int y, SDL_Rect render_clip, double rotation, SDL_RendererFlip flip){
    bool render_flag = true;

    SDL_Rect render_spot = {x,y,render_clip.w,render_clip.h};

    //printing rectangle dimensions
    //
    /*
    std::cout << "Current clip x: " << image::current_clip.x << ", Current clip y: " << image::current_clip.y << ", Current clip width: " << image::current_clip.w << ", Current clip height " << image::current_clip.h << std::endl;
    std::cout << "Render spot x: " << render_spot.x << ", Render spot y: " << render_spot.y << ", Render spot width: " << render_spot.w << ", Render spot height " << render_spot.h << std::endl;
    std::cout << image::texture << std::endl;
    */
    //
    if (renderer == NULL)
        render_flag = false;
    else
        if (SDL_RenderCopyEx(renderer,texture,&render_clip,&render_spot,rotation,NULL,flip) != 0){
            render_flag = false;
        }

    return render_flag;
}

bool visual_media::load_as_image(const char image_path[]){
    bool load_flag = true;

    if (renderer_present()){
        wipe();

        //Loading a base surface first
        //
        SDL_Surface* first_image = IMG_Load(image_path);
        SDL_Surface* second_image = NULL;

        if (first_image == NULL){
            load_flag = false;
            std::cout << "IMG Load failed to load image path" << std::endl;
        } else {
            //CONVERT IMAGE
            //
            second_image = SDL_ConvertSurfaceFormat(first_image, SDL_PIXELFORMAT_RGBA8888, 0);

            if (second_image == NULL){
                load_flag = false;
                std::cout << "Failed to convert image to format!" << std::endl;
            } else {
                //SETTING SIZE
                x_size = second_image->w;
                y_size = second_image->h;

                texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, x_size, y_size);

                if (texture == NULL){
                    load_flag = false;
                    std::cout << "Failed to create texture" << std::endl;
                } else {
                    //COPY SURFACE ONTO TEXTURE
                    set_locked(true);
                    set_pixels(second_image->pixels);            ///MAY GO BACK LATER TO EXPAND ON
                    set_locked(false);
                }
            }

            //CLEARING SURFACES BEFORE
            SDL_FreeSurface(first_image);
        }

    } else {
        std::cout << "Renderer is not present!" << std::endl;
        load_flag = false;
    }

    return load_flag;
}

bool visual_media::load_as_text(TTF_Font* given_font, const char content[], bool is_wrapping, Uint32 wrap_length){
    bool load_flag = true;

    if ((load_flag = renderer_present())){
        wipe();

        SDL_Surface* first_text = NULL;

        if (is_wrapping)
            first_text = TTF_RenderText_Blended_Wrapped(given_font,content,color_and_transparency, wrap_length);
        else
            first_text = TTF_RenderText_Blended(given_font,content,color_and_transparency);

        SDL_Surface* second_text = SDL_ConvertSurfaceFormat(first_text, SDL_PIXELFORMAT_RGBA8888, 0);

        if (first_text == NULL || renderer == NULL || given_font == NULL){
        } else {
            x_size = second_text->w;
            y_size = second_text->h;

            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, x_size, y_size);

            if (texture == NULL){
                load_flag = false;
            } else {
                //COPY SURFACE ONTO TEXTURE
                set_locked(true);
                set_pixels(second_text->pixels);
                set_locked(false);
            }

            SDL_FreeSurface(first_text);
            SDL_FreeSurface(second_text);
        }
    } else
        load_flag = false;

    return load_flag;
}

bool visual_media::load_as_blank(SDL_TextureAccess access_type){
    bool load_flag = true;

    if ((load_flag = renderer_present())){
        wipe();

        if (renderer != NULL)
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access_type, x_size, y_size);

        if (texture == NULL){
            load_flag = false;
        }
    } else
        load_flag = false;

    return load_flag;
}

bool visual_media::set_as_render_target(){
    bool render_flag = false;

    if (renderer != NULL){
        render_flag = (SDL_SetRenderTarget(renderer, texture) == 0);
    }

    return render_flag;
}

int visual_media::get_x_size(){
    return x_size;
}

void visual_media::set_x_size(int new_x_size){
    x_size = new_x_size;
}

int visual_media::get_y_size(){
    return y_size;
}

void visual_media::set_y_size(int new_y_size){
    y_size = new_y_size;
}

SDL_Color visual_media::get_color(){
    return color_and_transparency;
}

void visual_media::set_color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
    color_and_transparency.r = red;
    color_and_transparency.g = green;
    color_and_transparency.b = blue;
    color_and_transparency.a = alpha;

    //rgb_to_hsl();
}

bool visual_media::set_color_mod(){
    bool mod_flag = (SDL_SetTextureColorMod(texture,color_and_transparency.r,color_and_transparency.g,color_and_transparency.b) == 0);

    return mod_flag;
}

Uint8 visual_media::get_red(){
    return color_and_transparency.r;
}

void visual_media::set_red(Uint8 red){
    color_and_transparency.r = red;
    //rgb_to_hsl();
}

Uint8 visual_media::get_green(){
    return color_and_transparency.g;
}

void visual_media::set_green(Uint8 green){
    color_and_transparency.g = green;
    //rgb_to_hsl();
}

Uint8 visual_media::get_blue(){
    return color_and_transparency.b;
}

void visual_media::set_blue(Uint8 blue){
    color_and_transparency.b = blue;
    //rgb_to_hsl();
}

Uint8 visual_media::get_alpha(){
    return color_and_transparency.a;
}

void visual_media::set_alpha(Uint8 alpha){
    color_and_transparency.a = alpha;
}

bool visual_media::set_alpha_mod(Uint8 bonus_a){
    int calc_a = color_and_transparency.a + (int)bonus_a;

    if (calc_a > 255)
        calc_a = 255;
    else if (calc_a < 0)
        calc_a = 0;

    return (SDL_SetTextureAlphaMod(texture,(Uint8)calc_a) == 0);
}

bool visual_media::set_blend_mode(SDL_BlendMode blend_mode){
    return (SDL_SetTextureBlendMode(texture, blend_mode) == 0);
}

int visual_media::get_element_ID(){
    return element_ID;
}

void visual_media::set_element_ID(int new_ID){
    element_ID = new_ID;
}

int visual_media::compare_element_ID(int other_element_ID){
    return element_ID - other_element_ID;
}

bool visual_media::get_locked(){
    return is_locked;
}

bool visual_media::set_locked(bool lock){
    bool set_flag = true;

    if (lock){
        //CHECKING IF LOCK AREA IS VIABLE
        if (lock_rect.w > 0 && lock_rect.h > 0){
            //LOCK THE TEXTURE
            if (SDL_LockTexture(texture,&lock_rect,&pixels,&pitch) != 0){
                set_flag = false;
                is_locked = true;
            }
        } else {
            //LOCK THE TEXTURE
            if (SDL_LockTexture(texture,NULL,&pixels,&pitch) != 0){
                set_flag = false;
                is_locked = true;
            }
        }
    } else {
        //UNLOCK TEXTURE
        if (pixels != NULL){
            if (lock_rect.w > 0 && lock_rect.h > 0)
            //CHECKING IF LOCK AREA IS VIABLE
                SDL_UpdateTexture(texture, &lock_rect, pixels, pitch);
            else
                SDL_UpdateTexture(texture, NULL, pixels, pitch);

            SDL_UnlockTexture(texture);
            pixels = NULL;
            pitch = 0;
            is_locked = false;
        } else {
            set_flag = false;
        }
    }

    return set_flag;
}

Uint32 visual_media::get_pixel(unsigned int x, unsigned int y){
    if (is_locked){
        Uint32 *pixel_arr = (Uint32*)pixels;

        return pixel_arr[(y*(pitch/4))+x];
    } else
        return 0;
}

void visual_media::set_pixel(Uint32 new_pixel, unsigned int x, unsigned int y){
    Uint32* pixel_arr = (Uint32*)pixels;

    pixel_arr[(y*(pitch/4))+x] = new_pixel;
}

void* visual_media::get_pixels(){
    return pixels;
}

void visual_media::set_pixels(void* new_pixels){
    if (pixels != NULL){
        memcpy(pixels,new_pixels,pitch*y_size);
    }
}

int visual_media::get_pitch(){
    return pitch;
}

int visual_media::get_pixel_length(){
    return (y_size*(pitch/4));
}

SDL_Rect visual_media::get_lock_rect(){
    return lock_rect;
}

void visual_media::set_lock_rect(SDL_Rect new_lock){
    if (!is_locked)
        lock_rect = new_lock;
}

