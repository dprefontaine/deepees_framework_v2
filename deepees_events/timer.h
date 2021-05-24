#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <SDL.h>

class timer{
public:
    //Constructor
    timer(int ind = 0);
    //Destructor
    ~timer();
    //Play / pause / unpause / stop / reset
    //
    void play();
    void pause();
    void unpause();
    void stop();
    void reset();
    //Getting time
    //
    Uint32 get_ticks();

    //Getting timer states
    bool get_playing();
    bool get_paused();

    //ID ACCESSORS, MUTATORS, AND COMPARERS
    //
    int get_element_ID();
    void set_element_ID(int new_element_ID = 0);
    int compare_element_ID(timer timer_obs);


private:
    //Ticks
    //
    int play_ticks;
    int pause_ticks;
    //States
    //
    bool is_playing;
    bool is_paused;
    //Reference ID
    //
    int element_ID;
};

#endif // TIMER_H_INCLUDED
