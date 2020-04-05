#include "analyzer.h"

analyzer::analyzer(string _source) {
    source = _source;
    error = false;
    line = 0;
}

analyzer::~analyzer() {
    token_list.clear();
}

void analyzer::dump() {
    cout << source << endl;
}

bool analyzer::proceed() {
    bool comment = false;
    bool address = false;
    int  dtype   = DTYPE::DB;
    //bool _string = false;
    int b = 0; //begin

    for (int i = 0; i < (int)source.length(); i++) {
        if (source[i] == '\n') {
            dtype = DTYPE::DB;
            line++;
        }
        if (comment == false /*&& _string == false*/) {
            switch (source[i]) {
                case '#': {
                    comment = true;
                    continue;
                } break;
                case '%': {
                    address = true;
                    b = i + 1;
                    continue;
                } break;

                case '"': {
                    long unsigned int f = source.find('"', i + 1);
                    if (f != string::npos) {
                        //cout << "Found!\n";
                        string _val = source.substr(i + 1, f - i - 1);
                        token_s _token = {.type = TOKEN::STRING, .value = _val};
                        token_list.push_back(_token);
                        i = f + 1; b = i + 1;
                    }
                } break;
                case '.': {
                    int s = 0; while (source[i - s - 1] == ' ') s++;
                    string _val = source.substr(b - s, i - b);
                    token_s _token = {.type = TOKEN::SECTION, .value = _val};
                    token_list.push_back(_token);
                    b = i + 1;
                } break;
                case ':': {
                    int s = 0; while (source[i - s - 1] == ' ') s++;
                    string _val = source.substr(b - s, i - b);

                    bool _flag = false;
                    for (int c = 0; c < op_str_size; c++) {
                        if (_val.compare(op_str[c]) == 0) {
                            _val = to_string(c);

                            token_s _token = {.type = TOKEN::OPCODE, .value = _val};
                            token_list.push_back(_token);
                            
                            _flag = true;
                            break;
                        }
                    }

                    b = i + 1;
                    if (_flag == false) cout << "[ERROR]: UNKNOWN OPCODE: " << _val << endl;
                } break;
                case ',': 
                case ';': {
                    int s = 0; while (source[i - s - 1] == ' ') s++;
                    string _val = source.substr(b - s, i - b);
                    //cout << _val << endl;

                    bool _flag = false;

                    // CHECK IF IT'S REGISTER
                    for (int r = 0; r < reg_str_size; r++) {
                        if (_val.compare(reg_str[r]) == 0) {
                            if (address == true) {
                                cout << "[ERROR::" << line << "]: REGISTERS CAN'T BE ADDRESSED!" << endl;
                                error = true;
                                address = false;
                            }
                            _val = to_string(r);
                            token_s _token = {.type = TOKEN::REGISTER, .value = _val};
                            token_list.push_back(_token);

                            _flag = true;
                            break;
                        }
                    }
                    
                    // CHECK IF IT'S DATA TYPE
                    if (_flag == false) {
                        for (int d = 0; d < dtype_str_size; d++) {
                            if (_val.compare(dtype_str[d]) == 0) {
                                if (address == true) {
                                    cout << "[ERROR::" << line << "]: DATA TYPE FLAG CAN'T BE ADDRESSED!" << endl;
                                    error = true;
                                    address = false;
                                }

                                _val = to_string(d);
                                token_s _token = {.type = TOKEN::DATA_TYPE, .value = _val};
                                token_list.push_back(_token);
                                dtype = d;

                                _flag = true;
                                break;
                            }
                        }
                    }

                    // CHECK IF IT'S ADDRESS
                    if (_flag == false) {
                        token_s _token;
                        if (address == true) {
                            _token = (token_s){.type = TOKEN::ADDRESS, .value = _val};
                            address = false;
                        } else {
                            token_s _prev = token_list[token_list.size() - 1];
                            
                            bool _flag2 = false;
                            if (_prev.type == TOKEN::REGISTER) {
                                if (is_r(stoi(_prev.value))) {
                                    if (stoi(_val) > 255) {
                                        cout << "[ERROR::" << line << "]: CAN'T USE WORD VALUE FOR BYTE REGISTER!" << endl;
                                        error = true;
                                    }
                                    _token = (token_s){.type = TOKEN::BYTE, .value = _val};
                                    _flag2 = true;
                                } else if (is_rx(stoi(_prev.value))) {
                                    if (stoi(_val) > 65535) {
                                        cout << "[ERROR::" << line << "]: TOO BIG VALUE FOR WORD REGISTER!" << endl;
                                        error = true;
                                    }
                                    _token = (token_s){.type = TOKEN::WORD, .value = _val};
                                    _flag2 = true;
                                }
                            }

                            // CHECK IF IT'S BYTE OR WORD
                            if (_flag2 == false) {
                                switch (dtype) {
                                    case DTYPE::DB: {
                                        if (stoi(_val) > 255) {
                                            cout << "[ERROR::" << line << "]: TOO BIG VALUE!" << endl;
                                            error = true;
                                        }
                                        _token = (token_s){.type = TOKEN::BYTE, .value = _val};
                                    } break;
                                    case DTYPE::DW: {
                                        if (stoi(_val) > 65535) {
                                            cout << "[ERROR::" << line << "]: TOO BIG VALUE!" << endl;
                                            error = true;
                                        }
                                        _token = (token_s){.type = TOKEN::WORD, .value = _val};
                                    } break;
                                }
                            }
                        }
                        token_list.push_back(_token);
                    }

                    if (source[i] == ';') {
                        dtype = DTYPE::DB;
                    }

                    b = i + 1;
                } break;

                case '\t':
                case ' ':
                case '\n':
                    b++;
                break;
            }
        } else {
            if (source[i] == '\n') {
                comment = false;
                if (address == true) {
                    cout << "[ERROR::" << line << "]: WRONG ADDRESS USAGE!" << endl;
                    error = true;
                    address = false;
                }

                b = i + 1;
            }

            /*if (source[i] == '"') {
                _string = false;

            }*/
        }
    }

    if (_debug == true) {
        for (int i = 0; i < (int)token_list.size(); i++) {
            token_s _token = token_list[i];

            string type = token_str[_token.type];
            string value = _token.value;
            switch (_token.type) {
                case TOKEN::OPCODE:
                    value = op_str[stoi(_token.value)];
                break;
                case TOKEN::REGISTER:
                    value = reg_str[stoi(_token.value)];
                break;
                case TOKEN::DATA_TYPE:
                    value = dtype_str[stoi(_token.value)];
                break;
            }

            cout << i << ". " << type << ": " << "'" << value << "' ";
        }
        cout << endl;
    }
    return error;
}