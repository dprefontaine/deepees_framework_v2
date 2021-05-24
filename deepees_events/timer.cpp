#include "timer.h"

timer::timer(int ind){
    play_ticks = 0;
    pause_ticks = 0;
    is_playing = false;
    is_paused = true;
    //
    element_ID = ind;
}

timer::~timer(){

}

void timer::play(){
    is_playing = true;
    is_paused = false;

    play_ticks = SDL_GetTicks();
    pause_ticks = 0;
}

void timer::pause(){
    if (is_playing && !(is_paused)){
        is_paused = true;

        pause_ticks = SDL_GetTicks() - play_ticks;
        play_ticks = 0;
    }
}

void timer::unpause(){
    if (is_playing && is_paused){
        is_playing = true;
        is_paused = false;

        play_ticks = SDL_GetTicks() - pause_ticks;
        pause_ticks = 0;
    }
}

void timer::stop(){
    is_playing = false;
    is_paused = false;

    play_ticks = 0;
    pause_ticks = 0;
}

void timer::reset(){
    pause();
    unpause();
}

Uint32 timer::get_ticks(){
    Uint32 time = 0;

    if (is_playing){
        if (is_paused){
            time = pause_ticks;
        } else {
            time = SDL_GetTicks() - play_ticks;
        }
    }

    return time;
}

bool timer::get_playing(){
    return is_playing;
}

bool timer::get_paused(){
    return is_paused;
}

int timer::get_element_ID(){
    return element_ID;
}

void timer::set_element_ID(int new_element_ID){
    element_ID = new_element_ID;
}

int timer::compare_element_ID(timer timer_obs){
    return element_ID - timer_obs.get_element_ID();
}
