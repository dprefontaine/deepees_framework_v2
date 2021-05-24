#ifndef DEEPEES_FRAMEWORK_H_INCLUDED
#define DEEPEES_FRAMEWORK_H_INCLUDED

//THIS IS FOR INCLUDING ALL THE FRAMEWORK STUFF BEFOREHAND INTO MAIN CONTROLLER

#ifndef DLL_STUFF
#define DLL_STUFF

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <queue>
#include <string>

#endif

//Events
#include <timer.h>
#include <alarm.h>
#include <animation_player.h>

#include <loop_procedure.h>
#include <key_procedure.h>
#include <room_procedure.h>


//Math and Points
#include <camera.h>
#include <button.h>
#include <file_reader.h>
#include <math_set.h>

//Window
#include <window.h>

//Visual media
#include <visual_media.h>

//Audio media
#include <sound.h>
#include <music.h>

//Mix manager
#include <mixer_set.h>
#include <mixer_bin.h>

//Visual media
#include <media_set.h>
#include <media_bin.h>
#include <room.h>
#include <panel_interface.h>

//Controllers
#include <room_control.h>
#include <main_controller.h>


#endif // DEEPEES_FRAMEWORK_H_INCLUDED
