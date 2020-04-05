#include "generator.h"

generator::generator() {
    error = false;
}

generator::~generator() {

}

bool generator::proceed(std::vector<token_s> _token_list) {
    cout << "Generating binary..." << endl;

    for (int i = 0; i < (int)_token_list.size(); i++) {
        token_s _token = _token_list[i];
        switch (_token.type) {
            case TOKEN::OPCODE: {
                byte _op = (byte)stoi(_token.value) + 1;
                switch (_op) {
                    // +-------+
                    // |  SET  |
                    // +-------+
                    case OP::SET: {
                        token_s _arg1 = _token_list[i + 1];
                        token_s _arg2 = _token_list[i + 2];

                        if (_arg1.type == TOKEN::REGISTER) {
                            if (is_r(stoi(_arg1.value))) {
                                switch (_arg2.type) {
                                    case TOKEN::BYTE:       binary.push_back(BIN::SET_RB); break;
                                    case TOKEN::ADDRESS:    binary.push_back(BIN::SET_RA); break;
                                    case TOKEN::REGISTER: {
                                        if (is_r(stoi(_arg2.value))) {
                                            binary.push_back(BIN::SET_RR);
                                        } else if (is_rx(stoi(_arg2.value))) {
                                            get_error(i, "CAN'T SET REGISTER R TO REGISTER RX!");
                                        } else get_error(i, "EXPECTED REGISTER R, GOT ", reg_str[stoi(_arg2.value)]);
                                    } break;
                                    default: get_error(i, "EXPECTED REGISTER / ADDRESS / BYTE, GOT ", token_str[_arg1.type]);
                                }
                            } else if (is_rx(stoi(_arg1.value))) {
                                switch (_arg2.type) {
                                    case TOKEN::BYTE:       binary.push_back(BIN::SET_XB); break;
                                    case TOKEN::WORD:       binary.push_back(BIN::SET_XW); break;
                                    case TOKEN::ADDRESS:    binary.push_back(BIN::SET_XA); break;
                                    case TOKEN::REGISTER: {
                                        if (is_r(stoi(_arg2.value))) {
                                            binary.push_back(BIN::SET_XR);
                                        } else if (is_rx(stoi(_arg2.value))) {
                                            binary.push_back(BIN::SET_XX);
                                        } else get_error(i, "EXPECTED REGISTER R/RX, GOT ", reg_str[stoi(_arg2.value)]);
                                    } break;
                                    default: get_error(i, "EXPECTED REGISTER / ADDRESS / BYTE / WORD, GOT ", token_str[_arg1.type]);
                                }
                            } else get_error(i, "EXPECTED REGISTER R/RX, GOT ", reg_str[stoi(_arg1.value)]);
                        } else if (_arg1.type == TOKEN::REGISTER) {
                            switch (_arg2.type) {
                                case TOKEN::BYTE:       binary.push_back(BIN::SET_AB); break;
                                case TOKEN::WORD:       binary.push_back(BIN::SET_AW); break;
                                case TOKEN::ADDRESS:    binary.push_back(BIN::SET_AA); break;
                                case TOKEN::REGISTER: {
                                        if (is_r(stoi(_arg2.value))) {
                                            binary.push_back(BIN::SET_AR);
                                        } else if (is_rx(stoi(_arg2.value))) {
                                            binary.push_back(BIN::SET_AX);
                                        } else get_error(i, "EXPECTED REGISTER R/RX, GOT ", reg_str[stoi(_arg2.value)]);
                                    } break;
                                default: get_error(i, "EXPECTED REGISTER / ADDRESS / BYTE, GOT ", token_str[_arg1.type]);
                            }
                        } else get_error(i, "EXPECTED REGISTER / ADDRESS, GOT ", token_str[_arg1.type]);
                    } break;


                    case OP::ADD: {
                        token_s _t1 = _token_list[i + 1];
                        token_s _t2 = _token_list[i + 2];

                        if (_t1.type == TOKEN::REGISTER) {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::ADD_RR);
                            }
                            if (_t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::ADD_RV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::ADD_RA);
                            }
                        } else {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::ADD_AR);
                            }
                            if (_t2.type == TOKEN::WORD || _t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::ADD_AV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::ADD_AA);
                            }
                        }
                    } break;
                    case OP::SUB: {
                        token_s _t1 = _token_list[i + 1];
                        token_s _t2 = _token_list[i + 2];

                        if (_t1.type == TOKEN::REGISTER) {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::SUB_RR);
                            }
                            if (_t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::SUB_RV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::SUB_RA);
                            }
                        } else {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::SUB_AR);
                            }
                            if (_t2.type == TOKEN::BYTE || _t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::SUB_AV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::SUB_AA);
                            }
                        }
                    } break;
                    case OP::MUL: {
                        token_s _t1 = _token_list[i + 1];
                        token_s _t2 = _token_list[i + 2];

                        if (_t1.type == TOKEN::REGISTER) {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::MUL_RR);
                            }
                            if (_t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::MUL_RV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::MUL_RA);
                            }
                        } else {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::MUL_AR);
                            }
                            if (_t2.type == TOKEN::BYTE || _t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::MUL_AV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::MUL_AA);
                            }
                        }
                    } break;
                    case OP::DIV: {
                        token_s _t1 = _token_list[i + 1];
                        token_s _t2 = _token_list[i + 2];

                        if (_t1.type == TOKEN::REGISTER) {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::DIV_RR);
                            }
                            if (_t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::DIV_RV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::DIV_RA);
                            }
                        } else {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::DIV_AR);
                            }
                            if (_t2.type == TOKEN::BYTE || _t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::DIV_AV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::DIV_AA);
                            }
                        }
                    } break;
                    case OP::MOD: {
                        token_s _t1 = _token_list[i + 1];
                        token_s _t2 = _token_list[i + 2];

                        if (_t1.type == TOKEN::REGISTER) {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::MOD_RR);
                            }
                            if (_t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::MOD_RV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::MOD_RA);
                            }
                        } else {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::MOD_AR);
                            }
                            if (_t2.type == TOKEN::BYTE || _t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::MOD_AV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::MOD_AA);
                            }
                        }
                    } break;
                    /*case OP::: {
                        token_s _t1 = _token_list[i + 1];
                        token_s _t2 = _token_list[i + 2];

                        if (_t1.type == TOKEN::REGISTER) {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::_RR);
                            }
                            if (_t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::_RV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::_RA);
                            }
                        } else {
                            if (_t2.type == TOKEN::REGISTER) {
                                binary.push_back(BIN::_AR);
                            }
                            if (_t2.type == TOKEN::BYTE || _t2.type == TOKEN::BYTE) {
                                binary.push_back(BIN::_AV);
                            }
                            if (_t2.type == TOKEN::ADDRESS) {
                                binary.push_back(BIN::_AA);
                            }
                        }
                    } break;*/
                    case OP::PUSH: {
                        token_s _next = _token_list[i + 1];

                        if (_next.type == TOKEN::REGISTER)    binary.push_back(BIN::PUSH_R);
                        if (_next.type == TOKEN::BYTE)        binary.push_back(BIN::PUSH_V);
                        if (_next.type == TOKEN::WORD)        binary.push_back(BIN::PUSH_V);
                        if (_next.type == TOKEN::ADDRESS)     binary.push_back(BIN::PUSH_A);
                    } break;
                    case OP::POP: {

                    } break;
                    case OP::JMP: {
                        token_s _next = _token_list[i + 1];
                        if (_next.type == TOKEN::ADDRESS) {
                            binary.push_back(BIN::JMP_A);
                        } else get_error(i, "EXPECTED ADDRESS, GOT ", token_str[_next.type]);
                    } break;
                    case OP::SYS: {
                        token_s _next = _token_list[i + 1];
                        if (_next.type == TOKEN::BYTE) {
                            binary.push_back(BIN::SYSTEM);
                        } else get_error(i, "EXPECTED BYTE, GOT ", token_str[_next.type]);
                    } break;
                    default:
                        get_error(i, "UNKNOWN OPCODE TYPE: ", to_string(_token.type));
                }
            } break;
            case TOKEN::ADDRESS:
                word _value;
                if (_token.value.find("0X") == 0) {
                    _value = stoi(_token.value, 0, 16);
                } else {
                    bool _flag = false;
                    int _add = 0;
                    for (int s = 0; s < (int)_token_list.size(); s++) {
                        token_s _t = _token_list[s];

                        switch (_t.type) {
                            case TOKEN::SECTION: {
                                if (_token.value.compare(_t.value) == 0) {
                                    _flag = true;
                                    _value = s + _add;
                                    //cout << to_string(s) << " | " << to_string(_add) << endl;
                                } else _add--;
                            } break;
                            case TOKEN::WORD:
                            case TOKEN::ADDRESS:   _add++; break;
                            case TOKEN::DATA_TYPE: _add--; break;
                            case TOKEN::STRING: {
                                _add += _token.value.length() - 1;
                            } break;
                        }
                    }
                    if (_flag == false) _value = stoi(_token.value);
                }

                binary.push_back((byte)(_value >> (1 * 8)));
                binary.push_back((byte)(_value >> (0 * 8)));
            break;
            case TOKEN::REGISTER:
                binary.push_back((byte)stoi(_token.value));
            break;
            case TOKEN::BYTE: {
                byte _value;
                if (_token.value.find("0X") == 0) {
                    _value = stoi(_token.value, 0, 16);
                } else {
                    _value = stoi(_token.value);
                }

                binary.push_back(_value);
            } break;
            case TOKEN::WORD: {
                token_s _reg = _token_list[i - 1];

                word _value;
                if (_token.value.find("0X") == 0) {
                    _value = stoi(_token.value, 0, 16);
                } else {
                    _value = stoi(_token.value);
                }

                //
                binary.push_back((byte)(_value >> (1 * 8)));
                binary.push_back((byte)(_value >> (0 * 8)));
            } break;
            case TOKEN::STRING: {
                for (int c = 0; c < (int)_token.value.length(); c++) {
                    binary.push_back((byte)_token.value[c]);
                }
                //value = _token.value;
            } break;
        }
    }

    if (_debug == true) {
        cout << "Binary size is: " << binary.size() << endl;
        for (int i = 0; i < (int)binary.size(); i++) { //to_string(binary[i])
            cout << to_string(binary[i]) << ' ';
        }
        cout << endl;
    }

    return error;
}