#include "co2Sensor.h"

CO2Sensor::CO2Sensor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void CO2Sensor::update(double val) {
	value = val;
}

string CO2Sensor::prepare(string info) {
	return "REQ_CO2";
}
