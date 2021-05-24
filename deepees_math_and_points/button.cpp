#include "button.h"

button::button(int new_element_ID){
        state = 0;
        active = false;
        element_ID = new_element_ID;
};

int button::get_state(){
    return state;
}

void button::set_state(int new_state){
    state = new_state;
}

bool button::get_active(){
    return active;
}

void button::set_active(bool new_active){
    active = new_active;
}

int button::get_element_ID(){
    return element_ID;
}

void button::set_element_ID(int new_element_ID){
    element_ID = new_element_ID;
}

int button::compare_element_ID(button button_obs){
    return element_ID - button_obs.get_element_ID();
}

int button::compare_to(button other_button){
    int compare_num = compare_element_ID(other_button);

    if (compare_num == 0)
        return state - other_button.get_state();
    else
        return compare_num;
}
