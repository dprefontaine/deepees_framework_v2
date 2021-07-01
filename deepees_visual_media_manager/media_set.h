#ifndef Z_MEDIA_H_INCLUDED
#define Z_MEDIA_H_INCLUDED

//THIS FILE IS JUST TO MAKE LINKING EASIER TO MANAGE
//
#include <iostream>
#include <vector>

#include <animation_set.h>
#include <math_set.h>
#include <visual_media.h>
#include <point.h>
#include <button.h>

//POINTING MEDIA
//

///NOTE FOR FUTURE: MIGHT RESERVE ROOM AND PANEL RELATION ONLY FOR VIEWPORTS, WHERE IMAGES CAN BE SHARED UNIVERSALLY AMONG PANELS
struct image_point{
public:
    image_point(int visual_media_point, int render_spot_room_point, int render_spot_panel_point, int render_spot_index_point, int element_ID);
    ~image_point();
    int visual_media_point;
    int render_spot_room_point;
    int render_spot_panel_point;
    int render_spot_index_point;
    int element_ID;
    animation_set* image_animations = NULL;
};

///FOR POINTING TO IMAGE SOURCES ONLY
struct image_source_point{
    image_source_point(int visual_media_point);
    int visual_media_point;
};


//BUNDLING EVENT STUFF
//
struct button_bundle{
public:
    button_bundle(int element_ID = 0);

    ~button_bundle();

    int search_rectangle(SDL_Rect obs_rect);

    void move_rectangles(int x_move = 0, int y_move = 0);

    bool is_over(int mouse_x, int mouse_y, int vp_x, int vp_y);

    //FOR GETTING RELATIVE COORDINATES
    int get_x_over();
    int get_y_over();

    button m_button;
    std::vector<SDL_Rect> rectangles;
    int x_over, y_over;
};

//BUNDLING VISUALS (FOR REPEATED RENDERING)
//
struct rendering_bundle{
public:
    rendering_bundle(int new_pointer, SDL_Rect spot = {0,0,0,0}, double new_rotation = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE, int layer = 0, int ID = 0);

    ~rendering_bundle();

    bool render_bundle(visual_media* media_to_render, int x_pos = 0, int y_pos = 0, int x_size = 1, int y_size = 1);

    bool search_spot(SDL_Rect given_spot);

    int search_point(int x, int y);

    int visual_media_pointer;
    SDL_Rect render_spot;
    std::vector<point> render_points;

    bool is_active;
    double rotation;
    SDL_RendererFlip flip_state;
    int rendering_layer;
    int element_ID;
};

#endif // Z_MEDIA_H_INCLUDED
