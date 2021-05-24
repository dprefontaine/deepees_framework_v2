#include "loop_procedure.h"

loop_procedure::loop_procedure(int (*new_proc)(unsigned int), bool has_time, bool play, bool pause, int proc_ID){
    procedure = new_proc;
    needs_time = has_time;
    is_play = play;
    is_pause = pause;
    procedure_ID = proc_ID;
    proc_flag = 0;
}

loop_procedure::~loop_procedure(){}

bool loop_procedure::procedure_do(unsigned int ticks, bool playing, bool paused){
    if ((is_play && playing) || (is_pause && paused)){
        if (needs_time)
            proc_flag = procedure(ticks);
        else
            proc_flag = procedure(0);

        if (proc_flag < 0)
            return false;
        else
            return true;
    } else
        return true;
}

bool loop_procedure::get_play(){
    return is_play;
}

bool loop_procedure::get_pause(){
    return is_pause;
}

int loop_procedure::get_flag(){
    return proc_flag;
}

bool loop_procedure::comp_flag(int obs_flag){
    return proc_flag == obs_flag;
}

void loop_procedure::set_ID(int new_ID){
    procedure_ID = new_ID;
}

int loop_procedure::get_ID(){
    return procedure_ID;
}

bool loop_procedure::comp_ID(int obs_ID){
    return procedure_ID == obs_ID;
}
