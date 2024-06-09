#include "adminPanel.h"
#include <sstream>

AdminPanel::AdminPanel(std::string n, std::string i, std::string m): Device(n, i, m) {}

void AdminPanel::update(double val) {
	value = val;
}

string AdminPanel::prepare(string info) {
    char temp[10], hum[10], co2[10], mst[10], log[50];
    std::sscanf(info.c_str(), "%s, %s, %s, %s, %s", temp, hum, co2, mst, log);
    stringstream rstream;
    rstream << "Temp: " << temp << " - Hum: " << hum << " - CO2: " << co2 << "MST: " << mst << log;
    return rstream.str();
}
