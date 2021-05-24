#ifndef ROOM_PROCEDURE_H_INCLUDED
#define ROOM_PROCEDURE_H_INCLUDED

//THIS IS FOR PROCEDURES CALLED BY MOUSE ACTIVITY IN A ROOM

class room_procedure{
public:
    //CONSTRUCTOR
    room_procedure(int (*new_proc)(), bool play, bool pause, int mouse_input, int proc_ID);
    //DESTRUCTOR
    ~room_procedure();

    //CARRYING OUT PROCEDURE
    bool procedure_do(bool playing, bool paused);

    //PLAY AND PAUSE ACCESS
    bool get_play();
    bool get_pause();

    //FLAG ACCESSOR, MUTATOR, COMPARER
    int get_flag();
    bool comp_flag(int obs_flag);

    //INPUT ACCESSOR, MUTATOR, COMPARER
    int get_interface();
    bool comp_interface(int obs_interface);

    //INPUT ID ACCESSOR, MUTATOR, COMPARER   (FOR KEY STATES)
    void set_input_ID(int new_input);
    int get_input_ID();
    bool comp_input_ID(int obs_input);

    //ID ACCESSOR, MUTATOR, COMPARER
    void set_ID(int new_ID);
    int get_ID();
    bool comp_ID(int obs_ID);

private:
    int (*procedure)();

    bool is_play;
    bool is_pause;

    int proc_flag;
    int interface_ID;
    int mouse_input_ID;
    int procedure_ID;
};

#endif // ROOM_PROCEDURE_H_INCLUDED
