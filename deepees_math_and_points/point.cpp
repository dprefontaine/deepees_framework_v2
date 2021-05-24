#include "point.h"

point::point(int new_x, int new_y){
    x = new_x;
    y = new_y;
}

point::~point(){}

int point::get_x(){return x;}
void point::set_x(int new_x){x = new_x;}

int point::get_y(){return y;}
void point::set_y(int new_y){y = new_y;}
