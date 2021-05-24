#ifndef MIXER_BIN_H_INCLUDED
#define MIXER_BIN_H_INCLUDED

///THIS IS TO SAVE ON MEDIA CODE FOR BOTH ROOMS AND INTERFACE BLOCKS AND MAKE IT EASIER TO MODIFY MEDIA IF NEEDED

#include "mixer_set.h"

class mixer_bin {
public:
    ///CONSTRUCTOR
    mixer_bin();

    ///DESTRUCTOR
    ~mixer_bin();

    //FOR GETTING AUDIO OUTSIDE OF MIXER BIN
    //
    music& music_get(int index = 0);
    sound& sound_get(int index = 0);

    int music_media_bin_length();
    int audio_media_bin_length();

    ///MUSIC MIXER MANAGER
    //
    int music_new(int loops = 0, int ID = 0);
    bool music_load(unsigned int index = 0, const char music_path[] = {});
    Uint8 music_get_volume();
    Uint8 music_set_volume(Uint8 new_volume = 0);
    int music_get_loops(unsigned int index = 0);
    bool music_set_loops(unsigned int index = 0, int new_loops = 0);
    bool music_get_active(unsigned int index = 0);
    bool music_set_active(unsigned int index = 0, bool active = false);
    bool music_play(unsigned int index = 0);
    bool music_fade_play(unsigned int index = 0, unsigned int fade_ms = 0);
    int music_find_by_ID(int element_ID = 0);
    bool music_clear(unsigned int index = 0);

    void pause_music();
    bool is_paused();
    void resume_music();
    bool is_playing();

    void halt_music();
    bool fade_halt_music(unsigned int fade_ms = 0);
    //
    ///SOUND MIXER MANAGER
    //
    int sound_new(int channel = -1, int loops = 0, Uint8 volume = 50, int ID = 0);
    bool sound_load(unsigned int index = 0, const char sound_path[] = {});
    int sound_get_channel(unsigned int index = 0);
    bool sound_set_channel(unsigned int index = 0, int new_channel = -1);
    Uint8 sound_get_volume(unsigned int index = 0);
    bool sound_set_volume(unsigned int index = 0, Uint8 volume = 50);
    int sound_get_loops(unsigned int index = 0);
    bool sound_set_loops(unsigned int index = 0, int new_loops = 0);

    bool sound_get_active(unsigned int index = 0);
    bool sound_set_active(unsigned int index = 0, bool active = false);

    bool sound_play(unsigned int index = 0);
    bool sound_fade_play(unsigned int index = 0, int fade_ms = 0);
    int sound_find_by_ID(int element_ID = 0);
    bool sound_clear(unsigned int index = 0);

    //
    ///CHANNELS
    //
    /*
    void allocate_channels(int new_channels);
    bool set_channel_vol(int channel_index, int channel_volume);
    */

    //
    ///EFFECTS
    //

private:
    //AUDIO BIN
    std::vector<music> music_media_bin;
    std::vector<sound> sound_media_bin;

    //CHANNELS
    /*
    int channels;
    */
};

#endif // MEDIA_BIN_H_INCLUDED
