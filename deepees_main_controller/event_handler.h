#ifndef EVENT_HANDLER_H_INCLUDED
#define EVENT_HANDLER_H_INCLUDED



#include <vector>

class event_handler{
public:
    event_handler(){}

    virtual ~event_handler(){
        std::vector<key_procedure>().swap(key_down_procedures);
        std::vector<key_procedure>().swap(key_up_procedures);
        std::vector<key_state_procedure>().swap(key_state_procedures);
        std::vector<room_procedure>().swap(mouse_procedures);
    }

    int update_key_state_procedures(const Uint8* new_key_states);
    int update_key_down_procedures();
    int update_key_up_procedures();
    int update_mouse_procedures(std::queue<int> listen_queue);

    void add_to_key_down_queue(unsigned int key_input, bool repeat);
    void add_to_key_up_queue(unsigned int key_input, bool repeat);

private:
    struct event_key{
    public:
        unsigned int key_code = 0;
        bool is_on = false;
    };

    Uint8* key_states;

    std::queue<unsigned int> key_down_queue;
    std::vector<key_procedure> key_down_procedures = std::vector<key_procedure>();

    std::queue<unsigned int> key_up_queue;
    std::vector<key_procedure> key_up_procedures = std::vector<key_procedure>();


    //CONTROL
    std::vector<key_procedure> key_procedures = std::vector<key_procedure>();
    int max_key_responses = 50;
    std::vector<room_procedure> mouse_procedures = std::vector<room_procedure>();
    int max_mouse_responses = 50;
};

#endif // EVENT_HANDLER_H_INCLUDED
