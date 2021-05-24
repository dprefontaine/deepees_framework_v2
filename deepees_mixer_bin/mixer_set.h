#ifndef Z_MIXER_SET_H_INCLUDED
#define Z_MIXER_SET_H_INCLUDED

//THIS FILE IS JUST TO MAKE LINKING EASIER TO MANAGE FOR MIXER CLASS
//

#include <vector>
#include <sound.h>
#include <music.h>

struct mixer_point{
public:
    mixer_point(int index, int element_ID);

    int index;
    int element_ID;
};

#endif // Z_MEDIA_H_INCLUDED
