#ifndef PROCEDURES_H_INCLUDED
#define PROCEDURES_H_INCLUDED

#pragma once

//THIS IS FOR PROCEDURES WHICH ARE CALLED BY LOOPS
template<typename T>
struct procedure_base{
public:
    procedure_base(){}
    virtual ~procedure_base(){}

    virtual bool procedure_do(){return true;}

    //FLAG ACCESSOR, MUTATOR, AND COMPARER
    inline int get_flag(){
                    return proc_flag;
                }

    //ID ACCESSOR, MUTATOR, AND COMPARER
    inline int get_ID(){
                    return procedure_ID;
                }

    inline bool comp_ID(int obs_ID){
                    return procedure_ID == obs_ID;
                }


    int (*procedure)(T);

    bool is_play;
    bool is_pause;

    int proc_flag;
    int procedure_ID;
};

template<typename T,typename A,typename S,typename D,typename F>
struct room_state_base : public procedure_base<T>{
    int (*procedure)(T, A, S, D, F);
};

///LOOP PROCEDURE
struct loop_procedure : public procedure_base<unsigned int>{
public:
    //CONSTRUCTOR
    loop_procedure(int (*new_proc)(unsigned int), bool has_time, bool play, bool pause, int proc_ID){
        procedure = new_proc;
        needs_time = has_time;
        is_play = play;
        is_pause = pause;
        procedure_ID = proc_ID;
        proc_flag = 0;
    }

    //DESTRUCTOR
    virtual ~loop_procedure(){}

    //CARRYING OUT PROCEDURE
    inline bool procedure_do(unsigned int ticks, bool playing, bool paused){
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


private:
    bool needs_time;
};


///ROOM PROCEDURE
struct room_procedure : public procedure_base<unsigned int>{
public:
    //CONSTRUCTOR
    room_procedure(int (*new_proc)(unsigned int), bool play, bool pause, int mouse_input, int proc_ID){
        procedure = new_proc;
        is_play = play;
        is_pause = pause;
        proc_flag = 0;
        mouse_input_ID = mouse_input;
        procedure_ID = proc_ID;
    }

    //DESTRUCTOR
    virtual ~room_procedure(){}

    //CARRYING OUT PROCEDURE
    inline bool procedure_do(unsigned int mouse_button, bool playing, bool paused){
        if ((is_play && playing) || (is_pause && paused)){
            proc_flag = procedure(mouse_button);

            if (proc_flag < 0)
                return false;
            else
                return true;
        } else
            return true;
    }

    //INPUT ID ACCESSOR, MUTATOR, COMPARER   (FOR KEY STATES)
    inline int get_input_ID(){
        return mouse_input_ID;
    }

    inline bool comp_input_ID(int other_input){
        return mouse_input_ID == other_input;
    }

private:
    int interface_ID;
    int mouse_input_ID;
};



///ROOM STATE
struct room_state_procedure : public room_state_base<int, int, int, int, int>{
public:
    //CONSTRUCTOR
    room_state_procedure(int (*new_proc)(int, int, int, int, int), bool play, bool pause, int proc_ID){
        procedure = new_proc;
        is_play = play;
        is_pause = pause;
        proc_flag = 0;
        procedure_ID = proc_ID;
    }

    //DESTRUCTOR
    ~room_state_procedure(){}

    //CARRYING OUT PROCEDURE
    inline bool procedure_do(int mouse_state, int x_pos, int y_pos, int x_motion, int y_motion, bool playing, bool paused){
        if ((is_play && playing) || (is_pause && paused)){
            proc_flag = procedure(mouse_state, x_pos, y_pos, x_motion, y_motion);

            if (proc_flag < 0)
                return false;
            else
                return true;
        } else
            return true;
    }

    //INPUT ID ACCESSOR, MUTATOR, COMPARER   (FOR KEY STATES)
    inline int get_input_ID();

    //INPUT FLAG ACCESSOR, MUTATOR, COMPARER
    inline int get_input_flag(){
        return input_ID;
    }

private:
    int input_ID;
};




///KEY PROCEDURE

#include <string>

struct key_push{
    public:
        key_push(int key, bool repeat){
            scan_code = key;
            was_repeat = repeat;
        }

        std::string to_string(){
        std::string scan_char = std::to_string(scan_code);
        std::string repeat_char = std::to_string(was_repeat);

        return scan_char+", "+repeat_char;
    }

    int scan_code = 0;
    bool was_repeat = false;
};


struct key_procedure : public procedure_base<bool>{
public:
    //CONSTRUCTOR
    key_procedure(int (*new_proc)(bool), bool play, bool pause, int input, int proc_ID){
        procedure = new_proc;
        is_play = play;
        is_pause = pause;
        proc_flag = 0;
        input_ID = input;
        procedure_ID = proc_ID;
    }

    //DESTRUCTOR
    virtual ~key_procedure(){}

    //CARRYING OUT PROCEDURE
    inline bool procedure_do(bool repeat, bool playing, bool paused){
        if ((is_play && playing) || (is_pause && paused)){
            proc_flag = procedure(repeat);

            if (proc_flag < 0)
                return false;
            else
                return true;
        } else
            return true;
    }

    inline int get_input_ID(){
        return input_ID;
    }

    inline bool comp_input_ID(int other_input){
        return input_ID == other_input;
    }

private:
    int input_ID;
};

///KEY STATE PROCEDURE
struct key_state_procedure : public procedure_base<bool>{
public:
    //CONSTRUCTOR
    key_state_procedure(int (*new_proc)(bool), bool play, bool pause, int input, int proc_ID){
        procedure = new_proc;
        is_play = play;
        is_pause = pause;
        proc_flag = 0;
        input_ID = input;
        procedure_ID = proc_ID;
    }

    //DESTRUCTOR
    virtual ~key_state_procedure(){}

    //CARRYING OUT PROCEDURE
    bool procedure_do(bool key_map_state, bool playing, bool paused){
        if ((is_play && playing) || (is_pause && paused)){
            proc_flag = procedure(key_map_state);

            if (proc_flag < 0)
                return false;
            else
                return true;
        } else
            return true;
    }

    //INPUT ID ACCESSOR, MUTATOR, COMPARER   (FOR KEY STATES)
    inline int get_input_ID(){
        return input_ID;
    }

private:
    int (*procedure)(bool);

    int input_ID;
};


#endif // PROCEDURES_H_INCLUDED
