#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <SDL.h>
#include <SDL_mixer.h>

class sound {
public:
    //Constructor
    //
    sound(int channel = -1, int loops = 0, Uint8 volume = 100, int ID = 0);
    //Destructor
    //
    ~sound();
    //Load sound
    //
    bool load_sound(const char sound_id[]);
    //Wipe sound
    //
    void wipe();
    //Play
    //
    void play();
    void fade_play(int ms = 0);
    //
    //CHANNEL ACCESSOR AND MUTATOR
    //
    int get_channel();
    void set_channel(int new_channel);
    //
    //VOLUME ACCESSOR AND MUTATOR
    //
    Uint8 get_volume();
    void set_volume(Uint8 new_volume);
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
    //ID ACCESSORS, MUTATORS, AND COMPARERS
    //
    int get_element_ID();
    int compare_element_ID(int other_ID);

private:
    Mix_Chunk *chunk;
    //
    int preferred_channel;
    Uint8 volume;
    int loops;
    //
    bool is_active;
    int element_ID;
};

#endif // SOUND_H_INCLUDED
