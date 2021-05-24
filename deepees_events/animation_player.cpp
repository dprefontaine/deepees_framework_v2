#include "animation_player.h"

animation_player::animation_player(bool continuous, bool playback):alarm(0){
    current_frame = 1;

    is_reversed = false;

    is_continuous = continuous;
    is_playback = playback;
}

bool animation_player::animation_play(unsigned int ticks){
    bool play_flag = false;

    //DO ALARM PROCEDURE FIRST
    //
    int alarm_count;

    if ((alarm_count = alarm::alarm_set(ticks)) > 0){
        alarm::reset_warning();

        //CHECK REVERSE PLAY
        if (is_reversed && current_frame > 0){
            current_frame-=alarm_count;
            set_frame_limit();
            set_next_threshold();
            play_flag = true;
        //CHECK FORWARD PLAY
        } else if (current_frame < frames.size() && !(is_reversed)){
            //MUST BE AT THE END THEN
            current_frame+=alarm_count;
            set_frame_limit();
            set_next_threshold();
            play_flag = true;
        }
    }

    return play_flag;
}

void animation_player::set_frame_limit(){
    if (current_frame < 0){
        current_frame = 0;
    } else if (current_frame >= frames.size()){
        current_frame = frames.size()-1;
    }
}

void animation_player::set_next_threshold(){
    set_threshold(frames.at(current_frame).first);
}

bool animation_player::animation_at_end(){
    bool end_flag = true;

    //std::cout << "Animation has reached the end" << std::endl;
    if (!(is_continuous)){
        if (current_frame == frames.size()-1){
            if (is_playback) is_reversed = true;
            else end_flag = false;
        } else if (current_frame == 0){
            if (is_playback && is_reversed)
                end_flag = false;
        }
    } else {
        if (current_frame == frames.size()-1){
            if (is_playback) is_reversed = true;
            else animation_reset();
        } else if (current_frame == 0) {
            if (is_playback) is_reversed = false;
        }
    }

    //CHECKING IF ACTUALLY IN BOUNDS
    //std::cout << current_frame << std::endl;

    return end_flag;
}

int animation_player::animation_get_frame_index(){
    return current_frame;
}

void animation_player::animation_reset(){
    current_frame = 0;
    alarm::set_threshold(frames.at(0).first);
    alarm::reset_ticks();
}

void animation_player::add_new_frame(unsigned int new_frame_ticks, SDL_Rect frame_rect){
    frames.push_back(std::pair<unsigned int, SDL_Rect>(new_frame_ticks,frame_rect));
}

SDL_Rect animation_player::get_current_frame(){
    return frames.at(current_frame).second;
}
