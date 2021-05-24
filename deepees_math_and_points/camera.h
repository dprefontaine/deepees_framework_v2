#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

//THIS IS ALL BASED IN PIXEL POSITIONS

//CAMERA IS ALWAYS CENTERED IN A VIEWPORT

class camera {
public:
    //CONSTRUCTORS
    //
    //extended
    camera(int new_x_pos = 0, int new_y_pos = 0, int new_x_size = 0, int new_y_size = 0);
    //
    ~camera();
    //
    //POSITION ACCESSORS AND MUTATORS
    //
    //x
    int get_x_pos();
    void set_x_pos(int new_x_pos = 25);
    //y
    int get_y_pos();
    void set_y_pos(int new_y_pos = 25);
    //DIMENSION ACCESSORS AND MUTATORS
    //
    //x
    int get_x_size();
    void set_x_size(int new_x_size = 50);
    //y
    int get_y_size();
    void set_y_size(int new_y_size = 50);

private:
    //POSITIONS
    //
    int x_pos, y_pos;
    //DIMENSIONS
    //
    int x_size, y_size;
};

#endif // CAMERA_H_INCLUDED
