#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

class button {
public:
    //CONSTRUCTOR
    button(int new_element_ID = 0);
    //DESTRUCTOR
    ~button(){};
    //STATE ACCESSOR AND MUTATORS
    //
    int get_state();
    void set_state(int new_state = 0);
    //ACTIVE ACCESSOR AND MUTATORS
    //
    bool get_active();
    void set_active(bool new_active = false);
    //ELEMENT ID ACCESSOR, MUTATOR, AND COMPARER
    //
    int get_element_ID();
    void set_element_ID(int new_element_ID = 0);
    int compare_element_ID(button button_obs);
    //COMPARER
    //
    int compare_to(button other_button);

private:
    //BUTTON STATE
    //
    int state;
    //IF ACTIVE
    //
    bool active;
    //REFERENCE ID
    //
    int element_ID;
};

#endif // BUTTON_H_INCLUDED
