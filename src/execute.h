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
        uint pc;
        reg_u reg[4];
        byte memory[MEM_SIZE];
        bool working;
        stack<word> data;
    public:
        execute(std::vector<byte> _binary);
       ~execute();


};

#endif //EXECUTE_H