#ifndef EXECUTE_H 
#define EXECUTE_H

#include "define.h"

#define MEM_SIZE 256 * 256

extern bool _execute;
extern bool _output;
extern bool _debug;

union reg_u {
    word rx;
    byte r[2];
};

struct execute {
    private:
        uint pc = 0;
        reg_u reg[4] = {0};
        byte memory[MEM_SIZE] = {0};
        bool working = true;
        stack<word> data;
    public:
        execute(std::vector<byte> _binary);
       ~execute();


};

#endif //EXECUTE_H