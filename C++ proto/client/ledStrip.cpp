#include "ledStrip.h"
#include <sstream>

LEDStrip::LEDStrip(std::string n, std::string i, std::string m): Device(n, i, m) {}

void LEDStrip::update(double val) {
	value = val;
}

string LEDStrip::prepare(string info) {
    stringstream rstream;
    rstream << "SET_" << value;
    return rstream.str();
}
