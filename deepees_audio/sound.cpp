#include "sound.h"

sound::sound(int channel, int loops, Uint8 volume, int ID){
    chunk = NULL;
    sound::preferred_channel = channel;
    sound::volume = volume;
    sound::loops = loops;
    is_active = false;
    element_ID = ID;
}

sound::~sound(){
    wipe();
}

bool sound::load_sound(const char sound_id[]){
    bool load_flag = true;

    wipe();

    if (!(load_flag = (chunk = Mix_LoadWAV(sound_id)))){
        //NOTIFY ERROR
    } else {
        chunk->volume = volume;
    }

    return load_flag;
}

void sound::wipe(){
    if (chunk != NULL)
        Mix_FreeChunk(chunk);
}

void sound::play(){
    if (chunk != NULL && is_active)
        Mix_PlayChannel(preferred_channel,chunk,loops);
}

void sound::fade_play(int ms){
    if (chunk != NULL && is_active)
        Mix_FadeInChannel(preferred_channel,chunk,loops, ms);
}

int sound::get_channel(){
    return preferred_channel;
}

void sound::set_channel(int new_channel){
    preferred_channel = new_channel;
}

Uint8 sound::get_volume(){
    return volume;
}

void sound::set_volume(Uint8 new_volume){
    volume = new_volume;

    //SET NEW VOLUME
    if (chunk != NULL)
        chunk->volume = volume;
}

int sound::get_loops(){
    return loops;
}

void sound::set_loops(int new_loops){
    loops = new_loops;
}

bool sound::get_active(){
    return is_active;
}

void sound::set_active(bool active){
    is_active = active;
}

int sound::get_element_ID(){
    return element_ID;
}

int sound::compare_element_ID(int other_ID){
    return element_ID - other_ID;
}
