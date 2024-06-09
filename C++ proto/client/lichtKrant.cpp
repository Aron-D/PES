#include "lichtKrant.h"
#include <sstream>

LichtKrant::LichtKrant(std::string n, std::string i, std::string m): Device(n, i, m) {}

void LichtKrant::update(double val) {
	value = val;
}

string LichtKrant::prepare(string info) {
    char temp[10], hum[10], co2[10];
    std::sscanf(info.c_str(), "%s, %s, %s", temp, hum, co2);
    stringstream rstream;
    rstream << "Temp: " << temp << " - Hum: " << hum << " - CO2: " << co2;
    return rstream.str();
}
