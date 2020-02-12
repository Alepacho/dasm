#ifndef ANALYZER_H 
#define ANALYZER_H

#include "define.h"

extern bool _execute;
extern bool _output;
extern bool _debug;

struct analyzer {
    private:
        string source;
        bool error = false;
        int line = 0;
    public:
        std::vector<token_s> token_list;

        analyzer(string _source);
       ~analyzer();

        void dump();
        bool proceed();
};

#endif //ANALYZER_H