#ifndef RESPONSE_PROCEDURE_H_INCLUDED
#define RESPONSE_PROCEDURE_H_INCLUDED

//THIS IS FOR PROCEDURES CALLED WHEN A KEY IS PRESSED

class key_procedure{
public:
    //CONSTRUCTOR
    key_procedure(int (*new_proc)(), bool play, bool pause, int input, int proc_ID);
    //DESTRUCTOR
    ~key_procedure();

    //CARRYING OUT PROCEDURE
    bool procedure_do(bool playing, bool paused);

    //PLAY AND PAUSE ACCESS
    bool get_play();
    bool get_pause();

    //FLAG ACCESSOR, MUTATOR, COMPARER
    int get_flag();
    bool comp_flag(int obs_flag);

    //INPUT ID ACCESSOR, MUTATOR, COMPARER   (FOR KEY STATES)
    void set_input_ID(int new_input);
    int get_input_ID();
    bool comp_input_ID(int obs_input);

    //INPUT FLAG ACCESSOR, MUTATOR, COMPARER
    int get_input_flag();
    bool comp_input_flag(int obs_input);

    //ID ACCESSOR, MUTATOR, COMPARER
    void set_ID(int new_ID);
    int get_ID();
    bool comp_ID(int obs_ID);

private:
    int (*procedure)();

    bool is_play;
    bool is_pause;

    int proc_flag;
    int input_ID;
    int input_flag;
    int procedure_ID;
};


#endif
