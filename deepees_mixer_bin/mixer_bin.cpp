#include "mixer_bin.h"

//CONSTRUCTOR
mixer_bin::mixer_bin(){
    music_media_bin = std::vector<music>();
    sound_media_bin = std::vector<sound>();
    //channels = 0;
}

//DESTRUCTOR
mixer_bin::~mixer_bin(){
    //WIPING VISUAL MEDIA (SETTING RENDERER TO NULL SHOULD BE PASSED BY ALL)
    //FREEING MEMORY
    //
    //TEMP VECTORS
    std::vector<music>().swap(music_media_bin);
    std::vector<sound>().swap(sound_media_bin);
}

sound& mixer_bin::sound_get(int index){
    return sound_media_bin.at(index);
}

int mixer_bin::audio_media_bin_length(){
    return sound_media_bin.size();
}

///MUSIC MANAGER

int mixer_bin::music_new(int loops, int ID){
    //RETURN THE ID OF THE NEW MUSIC OBJECT
    music_media_bin.push_back(music(loops, ID));

    return music_media_bin.size()-1;
}

bool mixer_bin::music_load(unsigned int index, const char music_path[]){
    if (index < music_media_bin.size()){
        return music_media_bin[index].load_music(music_path);
    } else
        return false;
}

Uint8 mixer_bin::music_get_volume(){
    return Mix_VolumeMusic(-1);
}

Uint8 mixer_bin::music_set_volume(Uint8 new_volume){
    return Mix_VolumeMusic(new_volume);
}

int mixer_bin::music_get_loops(unsigned int index){
    if (index < music_media_bin.size()){
        return music_media_bin.at(index).get_loops();
    } else
        return -1;
}

bool mixer_bin::music_set_loops(unsigned int index, int new_loops){
    if (index < music_media_bin.size()){
        music_media_bin.at(index).set_loops(new_loops);
        return true;
    } else
        return false;
}

bool mixer_bin::music_get_active(unsigned int index){
    if (index < music_media_bin.size()){
        return music_media_bin.at(index).get_active();
    } else
        return false;
}

bool mixer_bin::music_set_active(unsigned int index, bool active){
    if (index < music_media_bin.size()){
        music_media_bin.at(index).set_active(active);
        return true;
    } else
        return false;
}

bool mixer_bin::music_play(unsigned int index){
    if (index < music_media_bin.size()){
        music_media_bin.at(index).play();
        return true;
    } else
        return false;
}

bool mixer_bin::music_fade_play(unsigned int index, unsigned int fade_ms){
    if (index < music_media_bin.size()){
        music_media_bin.at(index).fade_play(fade_ms);
        return true;
    } else
        return false;
}

int mixer_bin::music_find_by_ID(int element_ID){
    unsigned int music_search = 0;

    while (music_search < music_media_bin.size()){
        if (music_media_bin.at(music_search).compare_element_ID(element_ID) == 0)
            return music_search;

        music_search++;
    }

    return -music_search;
}

bool mixer_bin::music_clear(unsigned int index){
    if (index < music_media_bin.size()){
        music_media_bin.erase(music_media_bin.begin() + index);
        return true;
    } else
        return false;
}

void mixer_bin::pause_music(){
    Mix_PauseMusic();
}

bool mixer_bin::is_paused(){
    return (Mix_PausedMusic() == 1);
}

void mixer_bin::resume_music(){
    Mix_ResumeMusic();
}

bool mixer_bin::is_playing(){
    return (Mix_PausedMusic() == 0);
}

void mixer_bin::halt_music(){
    Mix_HaltMusic();
}

bool mixer_bin::fade_halt_music(unsigned int fade_ms){
    return Mix_FadeOutMusic(fade_ms);
}

///SOUND MANAGER

int mixer_bin::sound_new(int channel, int loops, Uint8 volume, int ID){
    //RETURN THE INDEX OF THE NEW SOUND
    sound_media_bin.push_back(sound(channel, loops, volume, ID));

    return sound_media_bin.size()-1;
}

bool mixer_bin::sound_load(unsigned int index, const char sound_path[]){
    //Check if index exists
    //
    if (index < sound_media_bin.size()){
        return sound_media_bin[index].load_sound(sound_path);
    } else {
        //std::cout << "This index is not available!" << std::endl;
        return false;
    }
}

int mixer_bin::sound_get_channel(unsigned int index){
    if (index < sound_media_bin.size()){
        return sound_media_bin[index].get_channel();
    } else
        return -1;
}

bool mixer_bin::sound_set_channel(unsigned int index, int new_channel){
    if (index < sound_media_bin.size()){
        sound_media_bin[index].set_channel(new_channel);
        return true;
    } else
        return false;
}

Uint8 mixer_bin::sound_get_volume(unsigned int index){
    if (index < sound_media_bin.size()){
        return sound_media_bin[index].get_volume();
    } else
        return 0;
}

bool mixer_bin::sound_set_volume(unsigned int index, Uint8 volume){
    if (index < sound_media_bin.size()){
        sound_media_bin[index].set_volume(volume);
        return true;
    } else
        return false;
}

int mixer_bin::sound_get_loops(unsigned int index){
    if (index < sound_media_bin.size()){
        return sound_media_bin[index].get_loops();
    } else
        return 0;
}

bool mixer_bin::sound_set_loops(unsigned int index, int new_loops){
    if (index < sound_media_bin.size()){
        sound_media_bin[index].set_loops(new_loops);
        return true;
    } else
        return false;
}

bool mixer_bin::sound_get_active(unsigned int index){
    if (index < sound_media_bin.size())
        return sound_media_bin[index].get_active();
    else
        return false;
}

bool mixer_bin::sound_set_active(unsigned int index, bool active) {
    //CHECK IF INDEX IS AVAILABLE
    //
    if (index < sound_media_bin.size()){
        sound_media_bin[index].set_active(active);
        return true;
    } else
        return false;
}

bool mixer_bin::sound_play(unsigned int index){
    //Check if index exists
    //
    if (index < sound_media_bin.size()){
        sound_media_bin[index].play();
        return true;
    } else {
        //std::cout << "This index is not available!" << std::endl;
        return false;
    }
}

bool mixer_bin::sound_fade_play(unsigned int index, int fade_ms){
    //Check if index exists
    //
    if (index < sound_media_bin.size()){
        sound_media_bin[index].fade_play(fade_ms);
        return true;
    } else {
        //std::cout << "This index is not available!" << std::endl;
        return false;
    }
}

int mixer_bin::sound_find_by_ID(int element_ID){
    int i = 0;

    while (i < int(sound_media_bin.size())){
        if (sound_media_bin[i].compare_element_ID(element_ID) == 0)
            return i;
        i++;
    }

    return -i;
}

bool mixer_bin::sound_clear(unsigned int index){
    //Check if index exists
    //
    if (index < sound_media_bin.size()){
        sound_media_bin.erase(sound_media_bin.begin() + index);
        return true;
    }
    else
        return false;
}
