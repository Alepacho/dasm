#ifndef GENERATOR_H 
#define GENERATOR_H

#include "define.h"

extern bool _execute;
extern bool _output;
extern bool _debug;

struct generator {
    private:
        bool error;

        void get_error(uint pos, string text1, string text2 = "") {
            cout << "[ERROR::" << pos << "] " << text1 << text2 << endl;
            error = true;
        }
    public:
        std::vector<byte> binary;

        generator();
       ~generator();

        bool proceed(std::vector<token_s> _token_list);
};

#endif //GENERATOR_H