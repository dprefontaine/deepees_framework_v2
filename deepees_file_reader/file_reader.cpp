#include "file_reader.h"

file_reader::file_reader(unsigned int max_data_length){
    file = NULL;
    data = new Sint32[max_data_length];
    max_length = max_data_length;

    for (unsigned int i = 0; i < max_data_length; i++)
        data[i] = -1;
}

file_reader::~file_reader(){
    delete data;
}

bool file_reader::find_file(std::string file_name, const char *setting){
    //FIND THE FILE
    //
    file = SDL_RWFromFile(file_name.c_str(),setting);

    return (file != NULL);
}

bool file_reader::read_bin_file(std::string file_name){
    bool read_flag = true;

    //FIND THE FILE
    //
    if (find_file(file_name, "r+b")){
        unsigned int file_size = SDL_RWsize(file)/sizeof(Sint64);
        unsigned int i;
        Sint32 result = 0;

        //READING FILE
        //
        for (i = 0; (i < max_length && i < file_size); i++){
            SDL_RWread(file, &result,sizeof(Sint32),1);
            data[i] = result;
        }

        SDL_RWclose(file);
        file = NULL;

    } else {
        std::cout << "Failed to read file of " << file_name.c_str() << " : " << SDL_GetError() << std::endl;
        read_flag = false;
    }

    return read_flag;
}

bool file_reader::write_bin_file(std::string file_name){
    //CHECK IF FILE EXISTS
    //
    bool write_flag = true;

    if (find_file(file_name, "w+b")){
        //unsigned break_threshold = line_break;

        //ADD CONTENTS TO THE FILE
        //
        for (unsigned int i = 0; (i < max_length); i++){
            SDL_RWwrite(file,&data[i],sizeof(Sint32),1);
        }

        SDL_RWclose(file);
        file = NULL;

    } else {
        std::cout << "The file directory already exists!" << std::endl;
        write_flag = false;
    }

    return write_flag;
}

void file_reader::new_vector_serial(std::string serial_name){
    vector_serials.insert(std::pair<std::string, std::vector<int>>(serial_name, std::vector<int>()));
}

void file_reader::new_string_serial(std::string serial_name){
    string_serials.insert(std::pair<std::string, std::string>(serial_name, ""));
}

void file_reader::new_int_serial(std::string serial_name){
    int_serials.insert(std::pair<std::string, int>(serial_name, -1));
}

void file_reader::new_bool_serial(std::string serial_name){
    bool_serials.insert(std::pair<std::string, bool>(serial_name, false));
}

void file_reader::read_data_to_vector(std::string vector_name, unsigned int start_point, unsigned int end_point){
    VECTOR_SERIAL::iterator vector_serial = vector_serials.find(vector_name);

    if (vector_serial != vector_serials.end()){
        unsigned int reading_index = start_point;

        while (reading_index < end_point && reading_index < max_length){
            vector_serial->second.push_back((int)data[reading_index]);
            reading_index++;
        }
    }
}

void file_reader::read_data_to_string(std::string string_name, unsigned int start_point, unsigned int end_point){
    STRING_SERIAL::iterator string_serial = string_serials.find(string_name);

    if (string_serial != string_serials.end()){
        //BUILD STRING FROM LINE OF INTS
        unsigned int reading_index = start_point;
        std::stringstream ss;
        char data_char;

        while (reading_index < end_point && reading_index < max_length){
            data_char = (char)data[reading_index];
            ss << data_char;
            reading_index++;
        }

        string_serial->second = ss.str();
    }
}

void file_reader::read_data_to_int(std::string int_name, unsigned int index){
    INT_SERIAL::iterator int_serial = int_serials.find(int_name);

    if (int_serial != int_serials.end() && index < max_length){
        int_serial->second = (int)data[index];
    }
}

void file_reader::read_data_to_bool(std::string bool_name, unsigned int index){
    BOOL_SERIAL::iterator bool_serial = bool_serials.find(bool_name);

    if (bool_serial != bool_serials.end() && index < max_length){
        bool_serial->second = (bool)data[index];
    }
}

void file_reader::write_vector_to_data(std::string vector_name, unsigned int start_point){
    VECTOR_SERIAL::iterator vector_serial = vector_serials.find(vector_name);

    if (vector_serial != vector_serials.end()){
        unsigned int writing_index = 0;

        while ((start_point+writing_index) < max_length && writing_index < vector_serial->second.size()){
            data[start_point+writing_index] = (Sint32)vector_serial->second.at(writing_index);
            writing_index++;
        }
    }
}

void file_reader::write_string_to_data(std::string string_name, unsigned int start_point){
    STRING_SERIAL::iterator string_serial = string_serials.find(string_name);

    if (string_serial != string_serials.end()){
        const char* string_data = string_serial->second.c_str();
        unsigned int string_index, string_length;
        string_length = string_serial->second.length();

        for (string_index = 0; (string_index < string_length && (start_point+string_index) < max_length); string_index++){
            data[start_point+string_index] = (Sint32)string_data[string_index];
        }
    }
}

void file_reader::write_int_to_data(std::string int_name, unsigned int index){
    INT_SERIAL::iterator int_serial = int_serials.find(int_name);

    if (int_serial != int_serials.end() && index < max_length){
         data[index] = (Sint32)int_serial->second;
    }
}

void file_reader::write_bool_to_data(std::string bool_name, unsigned int index){
    BOOL_SERIAL::iterator bool_serial = bool_serials.find(bool_name);

    if (bool_serial != bool_serials.end() && index < max_length){
         data[index] = (Sint32)bool_serial->second;
    }
}

std::vector<int> file_reader::get_serial_vector(std::string vector_name){
    VECTOR_SERIAL::iterator vector_serial = vector_serials.find(vector_name);

    if (vector_serial != vector_serials.end()){
        return vector_serial->second;
    } else {
        return std::vector<int>();
    }
}

std::string file_reader::get_serial_string(std::string string_name){
    STRING_SERIAL::iterator string_serial = string_serials.find(string_name);

    if (string_serial != string_serials.end()){
        return string_serial->second;
    } else {
        return "";
    }
}

int file_reader::get_serial_int(std::string int_name){
    INT_SERIAL::iterator int_serial = int_serials.find(int_name);

    if (int_serial != int_serials.end()){
        return int_serial->second;
    } else {
        return -1;
    }
}

bool file_reader::get_serial_bool(std::string bool_name){
    BOOL_SERIAL::iterator bool_serial = bool_serials.find(bool_name);

    if (bool_serial != bool_serials.end()){
        return bool_serial->second;
    } else {
        return false;
    }
}
