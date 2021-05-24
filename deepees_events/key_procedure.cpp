#include "key_procedure.h"

key_procedure::key_procedure(int (*new_proc)(), bool play, bool pause, int input, int proc_ID){
    procedure = new_proc;
    is_play = play;
    is_pause = pause;
    proc_flag = 0;
    input_ID = input;
    input_flag = 0;
    procedure_ID = proc_ID;
}

key_procedure::~key_procedure(){}

bool key_procedure::procedure_do(bool playing, bool paused){
    if ((is_play && playing) || (is_pause && paused)){
        proc_flag = procedure();

        if (proc_flag < 0)
            return false;
        else
            return true;
    } else
        return true;
}

bool key_procedure::get_play(){
    return is_play;
}

bool key_procedure::get_pause(){
    return is_pause;
}

int key_procedure::get_flag(){
    return proc_flag;
}

bool key_procedure::comp_flag(int obs_flag){
    return proc_flag == obs_flag;
}

void key_procedure::set_input_ID(int new_input){
    input_ID = new_input;
}

int key_procedure::get_input_ID(){
    return input_ID;
}

bool key_procedure::comp_input_ID(int obs_input){
    return input_ID == obs_input;
}

int key_procedure::get_input_flag(){
    return input_ID;
}

bool key_procedure::comp_input_flag(int obs_input){
    return input_ID == obs_input;
}

void key_procedure::set_ID(int new_ID){
    procedure_ID = new_ID;
}

int key_procedure::get_ID(){
    return procedure_ID;
}

bool key_procedure::comp_ID(int obs_ID){
    return procedure_ID == obs_ID;
}
