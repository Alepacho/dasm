#ifndef DEFINE_H 
#define DEFINE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int uint;

// binary opcodes
namespace BIN {
    enum {
        NOTHING,
        //SET           // +-------------+------+---------------------------+
        SET_RB,         // | r*  = byte  | 0x00 | 
        SET_RA,         // | r*  = mem   | 0x01 | gives val from memory
        SET_RR,         // | r*  = r*    | 0x02 | 
        SET_XB,         // | r*x = byte  | 0x03 | 
        SET_XW,         // | r*x = word  | 0x03 | 
        SET_XA,         // | r*x = mem   | 0x04 | gives val from memory
        SET_XR,         // | r*x = r*    | 0x05 | 
        SET_XX,         // | r*x = r*x   | 0x06 | 
        SET_AB,         // | mem = byte  | 0x07 | set memory to val
        SET_AW,         // | mem = word  | 0x07 | set memory to val
        SET_AA,         // | mem = mem   | 0x09 |
        SET_AR,         // | mem = r*    | 0x08 | set memory to reg
        SET_AX,         // | mem = r*x   | 0x08 | set memory to reg
        //ADD           // +-------------+------+---------------------------+
        ADD_RV,         // | r*  += val  | 0x0A |
        ADD_RA,         // | r*  += mem  | 0x0B |
        ADD_RR,         // | r*  += r*   | 0x0C |
        ADD_AV,         // | mem += val  | 0x0D |
        ADD_AR,         // | mem += r*   | 0x0E |
        ADD_AA,         // | mem += mem  | 0x0F |
        //SUB           // +-------------+------+---------------------------+
        SUB_RV,         // | r*  -= val  | 0x10 |
        SUB_RA,         // | r*  -= mem  | 0x11 |
        SUB_RR,         // | r*  -= r*   | 0x12 |
        SUB_AV,         // | mem -= val  | 0x13 |
        SUB_AR,         // | mem -= r*   | 0x14 |
        SUB_AA,         // | mem -= mem  | 0x15 |
        //MUL           // +-------------+------+---------------------------+
        MUL_RV,         // | r*  *= val  | 0x16 |
        MUL_RA,         // | r*  *= mem  | 0x17 |
        MUL_RR,         // | r*  *= r*   | 0x18 |
        MUL_AV,         // | mem *= val  | 0x19 |
        MUL_AR,         // | mem *= r*   | 0x1A |
        MUL_AA,         // | mem *= mem  | 0x1B |
        //DIV           // +-------------+------+---------------------------+
        DIV_RV,         // | r*  /= val  | 0x16 |
        DIV_RA,         // | r*  /= mem  | 0x17 |
        DIV_RR,         // | r*  /= r*   | 0x18 |
        DIV_AV,         // | mem /= val  | 0x19 |
        DIV_AR,         // | mem /= r*   | 0x1A |
        DIV_AA,         // | mem /= mem  | 0x1B |
        //MOD           // +-------------+------+---------------------------+
        MOD_RV,         // | r*  %= val  | 0x16 |
        MOD_RA,         // | r*  %= mem  | 0x17 |
        MOD_RR,         // | r*  %= r*   | 0x18 |
        MOD_AV,         // | mem %= val  | 0x19 |
        MOD_AR,         // | mem %= r*   | 0x1A |
        MOD_AA,         // | mem %= mem  | 0x1B |
        //PUSH          // +-------------+------+---------------------------+
        PUSH_V,         // |             |      |
        PUSH_A,         // |             |      |
        PUSH_R,         // |             |      |
        //POP           // +-------------+------+---------------------------+
        POP_V,          // |             |      |
        POP_A,          // |             |      |
        POP_R,          // |             |      |
        //JMP           // +-------------+------+---------------------------+
        JMP_A,          // |             | 0x   |
        //OTHER         // +-------------+------+---------------------------+
        SYSTEM,         // | act, arg    |      |
                        // +-------------+------+---------------------------+
        COUNT           // total binary opcodes count
    };
}

namespace OP {
    enum {
        NTH,    // nothing yet
        SET,    // a = b
        ADD,    // a + b
        SUB,    // a - b
        MUL,    // a * b
        DIV,    // a / b
        MOD,    // a % b
        PUSH,   // push in stack
        POP,    // pop in stack
        JMP,    // jump to address
        SYS     // system callback function
    };
}

// rax, rbx, rcx, rdx - 16 bit registers
// ra,  rb,  rc,  rd  - 8  bit registers (half of 16 bit regs)
namespace REG {
    enum {
        RA,  RB,  RC,  RD,  // 0x00,   0x01,   0x02,   0x03
        RAX, RBX, RCX, RDX  // 0x0000, 0x0001, 0x0002, 0x0003
    };
}

namespace DTYPE {
    enum {
        DB,
        DW
    };
}

namespace TOKEN {
    enum {
        OPCODE,
        ADDRESS,
        SECTION,
        REGISTER,
        DATA_TYPE, //data type
        BYTE,
        WORD,
        STRING
    };
}

struct token_s {
    byte type;
    string value;
};

static const string token_str[8] = {"OPCODE", "ADDRESS", "SECTION", "REGISTER", "DATA TYPE", "BYTE", "WORD", "STRING"};
static const int token_str_size = 8;
static const string op_str[10]  = {"SET", "ADD", "SUB", "MUL", "DIV", "MOD", "PUSH", "POP", "JMP", "SYS"};
static const int op_str_size = 10;
static const string reg_str[8] = {"RA", "RB", "RC", "RD", "RAX", "RBX", "RCX", "RDX"};
static const int reg_str_size = 8;
static const string dtype_str[2] = {"DB", "DW"};
static const int dtype_str_size = 2;

static bool is_r (byte _reg) { if (_reg < 4) return true; else return false; };
static bool is_rx(byte _reg) { if (_reg >= 4 && _reg < 8) return true; else return false; };

#endif //DEFINE_H