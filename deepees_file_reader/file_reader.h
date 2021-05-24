#ifndef INT_READER_H_INCLUDED
#define INT_READER_H_INCLUDED

#include <iostream>
#include <map>

#include <SDL.h>
#include <string>
#include <vector>
#include <sstream>

///I'M GOING TO MAKE BIGGER PLANS AT SOME POINT TO EXPAND ON ALL OF THIS IN THE FUTURE

///FOR NOW, HOWEVER, IT'S A FILLER DLL FOR WHENEVER I GET BETTER AT FILE MANAGEMENT AND PARSING

class file_reader{
public:
    //CONSTRUCTOR
    //
    file_reader(unsigned int data_length = 1);

    //DESTRUCTOR
    //
    ~file_reader();

    //FILE SEARCHING
    //
    bool find_file(std::string file_name, const char *setting);

    //FILE READING
    //
    bool read_bin_file(std::string file_name);

    //FILE WRITING
    //
    bool write_bin_file(std::string file_name);

    ///**NOTE: INSTEAD OF SEEING THE ARRAY OF DATA, DATA IS BASED ON MAPS INTERPRETING SAID DATA
    //SERIAL OR DATA INTERPRET MANAGER
    //
    void new_vector_serial(std::string serial_name);
    void new_string_serial(std::string serial_name);
    void new_int_serial(std::string serial_name);
    void new_bool_serial(std::string serial_name);

    void read_data_to_vector(std::string vector_name, unsigned int start_point, unsigned int end_point);
    void read_data_to_string(std::string string_name, unsigned int start_point, unsigned int end_point);
    void read_data_to_int(std::string int_name, unsigned int index);
    void read_data_to_bool(std::string bool_name, unsigned int index);

    void write_vector_to_data(std::string vector_name, unsigned int start_point);
    void write_string_to_data(std::string string_name, unsigned int start_point);
    void write_int_to_data(std::string int_name, unsigned int index);
    void write_bool_to_data(std::string bool_name, unsigned int index);

    std::vector<int> get_serial_vector(std::string vector_name);
    std::string get_serial_string(std::string string_name);
    int get_serial_int(std::string int_name);
    bool get_serial_bool(std::string bool_name);

private:
    SDL_RWops* file;

    //DATA ITSELF
    Sint32* data;

    //INTERPRETATION OF THE DATA
    typedef std::map<std::string, std::vector<int>> VECTOR_SERIAL;
    VECTOR_SERIAL vector_serials;

    typedef std::map<std::string, std::string> STRING_SERIAL;
    STRING_SERIAL string_serials;

    typedef std::map<std::string, int> INT_SERIAL;
    INT_SERIAL int_serials;

    typedef std::map<std::string, bool> BOOL_SERIAL;
    BOOL_SERIAL bool_serials;

    unsigned int max_length;
};

#endif // INT_READER_H_INCLUDED
