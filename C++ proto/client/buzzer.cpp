#include "buzzer.h"

Buzzer::Buzzer(std::string n, std::string i, std::string m): Device(n, i, m) {}

void Buzzer::update(double val) {
	value = val;
}

string Buzzer::prepare(string info) {
	try {
		if(std::atoi(info.c_str()) > 27){
			return "1\n\r";
		} else {
			return "0\n\r";
		}
	} catch (...) {}
}
