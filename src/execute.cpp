#include "execute.h"


execute::execute(std::vector<byte> _binary) {
    pc = 0;
    memset(&reg, 0, sizeof(reg_u) * 4);
    memset(&memory, 0, MEM_SIZE - 1);
    working = true;

    cout << "Executing..." << endl;
    
    uint size = (uint)_binary.size();
    for (uint i = 0; i < size; i++) {
        memory[i] = _binary[i];
    }
    
    while (working == true && pc < size) {
        uint _pc = pc;
        uint _arg1 = memory[_pc + 1]; //argument (1)
 
        switch (memory[pc]) {
            // +-------+
            // |  SET  |
            // +-------+
            case BIN::SET_RB: pc += 3; reg[_arg1].r[0] = memory[_pc + 2]; break;
            case BIN::SET_RA: pc += 4; reg[_arg1].r[0] = memory[memory[_pc + 2] * 256 + memory[_pc + 3]]; break;
            case BIN::SET_RR: pc += 3; reg[_arg1].r[0] = reg[memory[_pc + 2]].r[0]; break;
            case BIN::SET_XB: pc += 3; reg[_arg1].rx = memory[_pc + 2]; break;
            case BIN::SET_XW: pc += 4; reg[_arg1].rx = memory[_pc + 2] * 256 + memory[_pc + 3]; break;
            case BIN::SET_XA: pc += 4; reg[_arg1].rx = memory[memory[_pc + 2] * 256 + memory[_pc + 3]] * 256 + memory[memory[_pc + 2] * 256 + memory[_pc + 3] + 1]; break;
            case BIN::SET_XR: pc += 3; reg[_arg1].rx = reg[memory[_pc + 2]].r[0]; break;
            case BIN::SET_XX: pc += 3; reg[_arg1].rx = reg[memory[_pc + 2]].rx; break;
            case BIN::SET_AB: pc += 4; memory[_arg1 * 256 + memory[_pc + 2]] = memory[_pc + 3]; break;
            case BIN::SET_AW: pc += 5; memory[_arg1 * 256 + memory[_pc + 2]] = memory[_pc + 3] * 256 + memory[_pc + 4]; break;
            case BIN::SET_AA: pc += 5; memory[_arg1 * 256 + memory[_pc + 2]] = memory[memory[_pc + 3] * 256 + memory[_pc + 4]]; break;
            case BIN::SET_AR: pc += 4; memory[_arg1 * 256 + memory[_pc + 2]] = reg[memory[_pc + 3]].r[0]; break;
            case BIN::SET_AX: pc += 4; memory[_arg1 * 256 + memory[_pc + 2]]     = reg[memory[_pc + 3]].r[0];
                                       memory[_arg1 * 256 + memory[_pc + 2] + 1] = reg[memory[_pc + 3]].r[1]; break;

            /*
            //ADD
            case BIN::ADD_RV: {
                reg[memory[pc + 1]].r += memory[pc + 2];
                pc += 3;
            } break;
            case BIN::ADD_RR: {
                reg[memory[pc + 1]].r += reg[memory[pc + 2]].r;
                pc += 3;
            } break;
            case BIN::ADD_RA: {
                int _offset = memory[pc + 2] * 256 + memory[pc + 3];
                reg[memory[pc + 1]].r += memory[_offset];
                pc += 4;
            } break;

            //SUB
            case BIN::SUB_RV: {
                reg[memory[pc + 1]].r -= memory[pc + 2];
                pc += 3;
            } break;
            case BIN::SUB_RR: {
                reg[memory[pc + 1]].r -= reg[memory[pc + 2]].r;
                pc += 3;
            } break;
            case BIN::SUB_RA: {
                int _offset = memory[pc + 2] * 256 + memory[pc + 3];
                reg[memory[pc + 1]].r -= memory[_offset];
                pc += 4;
            } break;

            //MUL
            case BIN::MUL_RV: {
                reg[memory[pc + 1]].r *= memory[pc + 2];
                pc += 3;
            } break;
            case BIN::MUL_RR: {
                reg[memory[pc + 1]].r *= reg[memory[pc + 2]].r;
                pc += 3;
            } break;
            case BIN::MUL_RA: {
                int _offset = memory[pc + 2] * 256 + memory[pc + 3];
                reg[memory[pc + 1]].r *= memory[_offset];
                pc += 4;
            } break;

            //DIV
            case BIN::DIV_RV: {
                reg[memory[pc + 1]].r /= memory[pc + 2];
                pc += 3;
            } break;
            case BIN::DIV_RR: {
                reg[memory[pc + 1]].r /= reg[memory[pc + 2]].r;
                pc += 3;
            } break;
            case BIN::DIV_RA: {
                int _offset = memory[pc + 2] * 256 + memory[pc + 3];
                reg[memory[pc + 1]].r /= memory[_offset];
                pc += 4;
            } break;

            //MOD
            case BIN::MOD_RV: {
                reg[memory[pc + 1]].r %= memory[pc + 2];
                pc += 3;
            } break;
            case BIN::MOD_RR: {
                reg[memory[pc + 1]].r %= reg[memory[pc + 2]].r;
                pc += 3;
            } break;
            case BIN::MOD_RA: {
                int _offset = memory[pc + 2] * 256 + memory[pc + 3];
                reg[memory[pc + 1]].r %= memory[_offset];
                pc += 4;
            } break;

            case BIN::MOD_RR: {
                reg[memory[pc + 1]].r %= reg[memory[pc + 2]].r;
                pc += 3;
            } break;
            //JUMPS
            case BIN::JMP_A: {
                int _offset = memory[pc + 1] * 256 + memory[pc + 2];
                pc = _offset;
            } break;*/

            // Callback
            case BIN::SYSTEM: {
                byte _action = memory[pc + 1];

                switch (_action) {
                    case 1: //exit
                        working = false;
                        cout << "PROGRAM EXITED WITH RETURN VALUE: " << to_string(memory[pc + 2]) << endl;
                    break;
                    case 2: {// print
                        //arg[1]: address
                        int _offset = memory[pc + 2] * 256 + memory[pc + 3];
                        
                        for (int i = 0; memory[_offset + i] != 0; i++) {
                            cout << memory[_offset + i];
                        }
                    } break;
                    default:
                        cout << "[ERROR]: WRONG SYS ACTION! (" << to_string(memory[pc + 2]) << ")" << endl;
                        working = false;
                }
                pc += 4;
            } break;
            default:
                cout << "[ERROR::" << to_string(pc) << "]: UNKNOWN OPCODE " << to_string(memory[pc]) << endl;
                working = false;
        }
    }

    if (_debug == true) {
        for (int i = 0; i < 4; i++) {
            cout << reg_str[i + 4] << ": " << to_string(reg[i].rx) << " [" << to_string(reg[i].r[0]) << ", " << to_string(reg[i].r[1]) << "]" << endl;
        }

        cout << "STACK: ";
        while (!data.empty()) {
            cout << " " << data.top();
            data.pop();
        }
        cout << endl;
    }
}

execute::~execute() {
    while (!data.empty()) {
        data.pop();
    }
}