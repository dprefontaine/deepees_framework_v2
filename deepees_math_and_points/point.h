#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

//COORDINATE STUFF
//
class point{
public:
    point(int new_x = 0, int new_y = 0);

    ~point();

    int get_x();
    void set_x(int new_x = 0);

    int get_y();
    void set_y(int new_y = 0);

private:
    int x,y;
};

#endif // POINT_H_INCLUDED
