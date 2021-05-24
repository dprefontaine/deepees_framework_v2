#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED

#include <SDL.h>
#include <SDL_mixer.h>

class music {
public:
    //Constructor
    //
    music(int loops = 0, int ID = 0);
    //Destructor
    //
    ~music();
    //Load sound
    //
    bool load_music(const char sound_id[]);
    //Wipe sound
    //
    void wipe();
    //Play
    //
    void play();
    void fade_play(int ms);
    //
    //LOOP ACCESSOR AND MUTATOR
    //
    int get_loops();
    void set_loops(int new_loops);
    //
    //ACTIVE
    //
    bool get_active();
    void set_active(bool active = false);
    //
    //ID ACCESSORS, MUTATORS, AND COMPARERS
    //
    int get_element_ID();
    int compare_element_ID(int other_ID);

private:
    Mix_Music *music_data;
    //
    int loops;
    //
    bool is_active;
    int element_ID;
};

#endif // SOUND_H_INCLUDED
