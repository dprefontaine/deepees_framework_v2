#ifndef ALARM_H_INCLUDED
#define ALARM_H_INCLUDED

#include "timer.h"

class alarm{
public:
    //CONSTRUCTOR
    alarm(int new_threshold);
    //DESTRUCTOR
    ~alarm();
    //ALARM MANAGER
    //
    int alarm_set(int ticks_given);
    void reset_ticks();

    //THRESHOLD ACCESSOR AND MUTATOR
    //
    int get_threshold();
    void set_threshold(int new_threshold = 0);

    //WARNING MANAGER
    int get_warning();
    void reset_warning();

private:
    //TIME VARIABLES
    //
    int milliseconds;
    int play_threshold;

    //WARNING
    //
    int warn_count;
};

#endif // ALARM_H_INCLUDED
