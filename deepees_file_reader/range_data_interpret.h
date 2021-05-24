#ifndef RANGE_DATA_INTERPRET_H_INCLUDED
#define RANGE_DATA_INTERPRET_H_INCLUDED

#include <SDL.h>
#include <string>
#include <vector>

enum RANGE_INTERPRET{
    STRING_INTERPRET,
    VECTOR_INTERPRET
};

class range_data_interpret{
public:
    range_data_interpret(RANGE_INTERPRET type, unsigned int start, unsigned int ending){
        interpret_type = type;
        start_index = start;
        end_index = ending;
    }

    RANGE_INTERPRET get_interpretation(){
        return interpret_type;
    }

protected:
    RANGE_INTERPRET interpret_type;
    unsigned int start_index, end_index;
};

class vector_data_interpret : public range_data_interpret{
public:
    void read_vector(std::vector<Sint32> data);

private:
    std::vector<int> vector_interpret = std::vector<int>();
};

class string_data_interpret : public range_data_interpret{
public:
    void read_vector(std::vector<Sint32> data);

private:
    std::string string_interpret = "";
};

#endif // RANGE_DATA_INTERPRET_H_INCLUDED
