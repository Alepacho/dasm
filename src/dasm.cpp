#include "define.h"
#include "analyzer.h"
#include "generator.h"
#include "execute.h"

bool _execute = true;
bool _output  = false;
bool _debug   = false;

int main(int argc, char *argv[]) {
	cout << "Dasm!" << endl;
	//
	
	if (argc > 1) {
		

		if (argc != 2) {
			for (int i = argc - 1; i >= 2; i--) {
				string _arg = string(argv[i]);
				if (_arg.compare("-debug")  == 0) _debug   = true;
				if (_arg.compare("-nexec")  == 0) _execute = false;
				if (_arg.compare("-output") == 0) _output  = true;
			}
		}

		if (string(argv[1]).find(".dasm") != string::npos) {
			ifstream file(argv[1], ifstream::in);
			if (file.is_open()) {
				cout << "Assembling: " << argv[1] << endl;
				// Reading lines
				string line;
				string _code = "";
				for (int i = 0; !file.eof(); i++) {
					std::getline(file, line);
					_code.append(line + "\n");
				}

				// make all chars uppercase
				for (int i = 0; i < (int)_code.length(); i++) {
					_code[i] = toupper(_code[i]);
				}

				// Making tokens
				analyzer _anal(_code); // Yes.
				if(_anal.proceed() == false) {
					generator _gen;
					
					if (_gen.proceed(_anal.token_list) == false) {
						if (_execute) execute _exec(_gen.binary);
						if (_output) {
							string file_out_name = string(argv[1]).substr(0, string(argv[1]).size() - 5) + ".dbin";
							cout << file_out_name << endl;

							ofstream out(file_out_name, ofstream::out | ofstream::binary);

							if (out.is_open()) {
								for (uint i = 0; i < _gen.binary.size(); i++) {
									byte _b = _gen.binary[i];
									out.write((char*)&_b, 1);
								}
							} else cout << "Can't create this file!" << endl;
							out.close();
						}
					}
				}

				file.close();
			} else cout << "File doesn't exist!" << endl;
		} else if (string(argv[1]).find(".dbin") != string::npos) {
			ifstream file(argv[1], ifstream::in | ofstream::binary);
			if (file.is_open()) {
				std::vector<byte> binary;

				for (int i = 0; !file.eof(); i++) {
					byte _b;
					file.read((char*)&_b, 1);
					binary.push_back(_b);
				}

				//cout << binary.size() << endl;

				if (_execute) execute _exec(binary);
			}
		} else cout << "Please use only .dasm / .dbin files." << endl;
	} else cout << "Type dasm *file*.dasm -exec" << endl;
	
	return 0;
}
