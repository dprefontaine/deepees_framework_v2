#include "animation_set.h"

animation_set::animation_set(){
    animations = std::map<std::string, animation_player>();
}

animation_set::~animation_set(){
    std::map<std::string, animation_player>().swap(animations);
}

void animation_set::add_animation_state(std::string new_animation_state, bool continuous, bool playback){
    animations.insert(std::pair<std::string, animation_player>(new_animation_state, animation_player(continuous, playback)));
}

void animation_set::set_animation_state(std::string animation_state){
    current_animation = animation_state;
    reset_animation(animation_state);
}

std::string animation_set::get_animation_state(){
    return current_animation;
}

bool animation_set::update_animation(unsigned int ticks){
    bool update_flag = false;

    if (is_playing){
        try {
            update_flag = animations.at(current_animation).animation_play(ticks);
            is_playing = animations.at(current_animation).animation_at_end();
        } catch (std::out_of_range e){
            update_flag = false;
            is_playing = false;
        }
    }

    return update_flag;
}

int animation_set::get_animation_frame_index(std::string animation){
    try {
        return animations.at(animation).animation_get_frame_index();
    } catch (std::out_of_range e){
        return -1;
    }
}

void animation_set::reset_animation(std::string animation){
    if (animations.find(animation) != animations.end()){
        animations.at(animation).animation_reset();
    }
}

void animation_set::add_frame_to_animation(std::string animation, unsigned int new_frame_ticks, SDL_Rect new_rect){
    if (animations.find(animation) != animations.end()){
        animations.at(animation).add_new_frame(new_frame_ticks, new_rect);
    }
}

SDL_Rect animation_set::get_frame(){
    try {
        return animations.at(current_animation).get_current_frame();
    } catch (std::out_of_range e){
        return {0,0,0,0};
    }
}

void animation_set::set_playing(bool set_play){
    is_playing = set_play;
}

bool animation_set::get_playing(){
    return is_playing;
}
