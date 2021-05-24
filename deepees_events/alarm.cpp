#include "alarm.h"

alarm::alarm(int new_threshold){
    milliseconds = 0;
    warn_count = 0;
    play_threshold = new_threshold;
}

alarm::~alarm(){};

int alarm::alarm_set(int ticks_given){
    //ADDING TICKS TO MILLISECONDS PILE FIRST
    //
    milliseconds+= ticks_given;

    //GETTING COUNTS FROM HOW MANY THRESHOLDS FIT INTO MS
    //
    while (milliseconds > play_threshold){
    //ADD A COUNT
        ++warn_count;
        milliseconds-= play_threshold;
    }

    return warn_count;
}

void alarm::reset_ticks(){
    //SETTING TICKS BACK TO 0
    //
    milliseconds = 0;
}

int alarm::get_threshold(){
    return play_threshold;
}

void alarm::set_threshold(int new_threshold){
    play_threshold = new_threshold;
    milliseconds = 0;
}

int alarm::get_warning(){
    return warn_count;
}

void alarm::reset_warning(){
    warn_count = 0;
}
