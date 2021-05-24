#include "camera.h"

camera::camera(int new_x_pos, int new_y_pos, int new_x_size, int new_y_size){
    x_pos = new_x_pos;
    y_pos = new_y_pos;
    x_size = new_x_size;
    y_size = new_y_size;
}

camera::~camera(){}

int camera::get_x_pos(){
    return x_pos;
}

void camera::set_x_pos(int new_x_pos){
    x_pos = new_x_pos;
}

int camera::get_y_pos(){
    return y_pos;
}

void camera::set_y_pos(int new_y_pos){
    y_pos = new_y_pos;
}

int camera::get_x_size(){
    return x_size;
}

void camera::set_x_size(int new_x_size){
    x_size = new_x_size;
}

int camera::get_y_size(){
    return y_size;
}

void camera::set_y_size(int new_y_size){
    y_size = new_y_size;
}
