#ifndef ANIMATION_PLAYER_H_INCLUDED
#define ANIMATION_PLAYER_H_INCLUDED

///THIS IS TO HELP CYCLE THROUGH ANIMATIONS

#include "alarm.h"
#include <vector>

class animation_player : public alarm{
public:
    //CONSTRUCTOR
    animation_player(bool continuous = false, bool playback = false);
    //DESTRUCTOR
    ~animation_player(){};

    //PLAYING
    //
    bool animation_play(unsigned int ticks);
    int animation_get_frame_index();
    void animation_reset();

    bool animation_at_end();                //FOR ANIMATION SET'S USE ONLY

    //MAX FRAME ACCESSOR AND MUTATOR
    //
    void add_new_frame(unsigned int new_frame_ticks, SDL_Rect frame_rect);
    SDL_Rect get_current_frame();

private:
    void set_frame_limit();
    void set_next_threshold();

    //FRAME VARIABLES
    //
    //FIRST VAR IS TICKS
    //SECOND VAR IS CHANGE IN FRAME
    std::vector<std::pair<unsigned int, SDL_Rect>> frames;
    unsigned int current_frame;

    //STATES
    //
    bool is_reversed;

    //MODES
    //
    bool is_continuous;
    bool is_playback;
};

#endif // ANIMATION_PLAYER_H_INCLUDED
