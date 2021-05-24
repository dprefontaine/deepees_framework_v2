#include "music.h"

music::music(int loops, int ID){
    music_data = NULL;
    music::loops = loops;
    is_active = false;
    element_ID = ID;
}

music::~music(){
    wipe();
}

bool music::load_music(const char music_id[]){
    bool load_flag = true;

    wipe();

    if (!(load_flag = (music_data = Mix_LoadMUS(music_id)))){
        //std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
    }

    return load_flag;
}

void music::wipe(){
    if (music_data != NULL)
        Mix_FreeMusic(music_data);
}

void music::play(){
    if (music_data != NULL && is_active)
        Mix_PlayMusic(music_data,loops);
}

void music::fade_play(int ms){
    if (music_data != NULL && is_active)
        Mix_FadeInMusic(music_data,loops, ms);
}

int music::get_loops(){
    return loops;
}

void music::set_loops(int new_loops){
    loops = new_loops;
}

bool music::get_active(){
    return is_active;
}

void music::set_active(bool active){
    is_active = active;
}

int music::get_element_ID(){
    return element_ID;
}

int music::compare_element_ID(int other_ID){
    return element_ID - other_ID;
}
