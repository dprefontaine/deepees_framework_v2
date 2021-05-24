#include "room_procedure.h"

room_procedure::room_procedure(int (*new_proc)(), bool play, bool pause, int mouse_input, int proc_ID){
    procedure = new_proc;
    is_play = play;
    is_pause = pause;
    proc_flag = 0;
    mouse_input_ID = mouse_input;
    procedure_ID = proc_ID;
}

room_procedure::~room_procedure(){}

bool room_procedure::procedure_do(bool playing, bool paused){
    if ((is_play && playing) || (is_pause && paused)){
        proc_flag = procedure();

        if (proc_flag < 0)
            return false;
        else
            return true;
    } else
        return true;
}

bool room_procedure::get_play(){
    return is_play;
}

bool room_procedure::get_pause(){
    return is_pause;
}

int room_procedure::get_flag(){
    return proc_flag;
}

bool room_procedure::comp_flag(int obs_flag){
    return proc_flag == obs_flag;
}

int room_procedure::get_interface(){
    return interface_ID;
}

bool room_procedure::comp_interface(int obs_interface){
    return interface_ID == obs_interface;
}

void room_procedure::set_input_ID(int new_input){
    mouse_input_ID = new_input;
}

int room_procedure::get_input_ID(){
    return mouse_input_ID;
}

bool room_procedure::comp_input_ID(int obs_input){
    return mouse_input_ID == obs_input;
}

void room_procedure::set_ID(int new_ID){
    procedure_ID = new_ID;
}

int room_procedure::get_ID(){
    return procedure_ID;
}

bool room_procedure::comp_ID(int obs_ID){
    return procedure_ID == obs_ID;
}
