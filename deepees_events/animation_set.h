#ifndef ANIMATION_SET_H_INCLUDED
#define ANIMATION_SET_H_INCLUDED

#include "animation_player.h"
#include <map>

//FOR MANAGING MULTIPLE ANIMATIONS WHICH MAY BE UNDER ONE SPRITE INSTANCE

class animation_set{
public:
    animation_set();
    ~animation_set();

    void add_animation_state(std::string new_animation_state, bool continuous, bool playback);
    void set_animation_state(std::string animation_state);
    std::string get_animation_state();

    bool update_animation(unsigned int ticks);

    int get_animation_frame_index(std::string animation);
    void reset_animation(std::string animation);

    void add_frame_to_animation(std::string animation, unsigned int new_frame_ticks, SDL_Rect new_rect);
    SDL_Rect get_frame();

    void set_playing(bool set_play);
    bool get_playing();

private:
    std::map<std::string, animation_player> animations;

    std::string current_animation = "";

    bool is_playing = false;
};

#endif // ANIMATION_SET_H_INCLUDED
