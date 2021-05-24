#ifndef LOOP_PROCEDURE_H_INCLUDED
#define LOOP_PROCEDURE_H_INCLUDED

//THIS IS FOR PROCEDURES WHICH ARE CALLED BY LOOPS

class loop_procedure{
public:
    //CONSTRUCTOR
    loop_procedure(int (*new_proc)(unsigned int), bool has_time, bool play, bool pause, int proc_ID);
    //DESTRUCTOR
    ~loop_procedure();

    //CARRYING OUT PROCEDURE
    bool procedure_do(unsigned int ticks, bool playing, bool paused);

    //PLAY AND PAUSE ACCESS
    bool get_play();
    bool get_pause();

    //FLAG ACCESSOR, MUTATOR, AND COMPARER
    int get_flag();
    bool comp_flag(int obs_flag);

    //ID ACCESSOR, MUTATOR, AND COMPARER
    void set_ID(int new_ID);
    int get_ID();
    bool comp_ID(int obs_ID);

private:
    int (*procedure)(unsigned int);

    bool is_play;
    bool is_pause;

    bool needs_time;
    int proc_flag;
    int procedure_ID;
};

#endif // LOOP_PROCEDURE_H_INCLUDED
