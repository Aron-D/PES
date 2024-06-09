#include "temperatureSensor.h"

TemperatureSensor::TemperatureSensor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void TemperatureSensor::update(double val) {
	value = val;
}

string TemperatureSensor::prepare(string info) {
	return "REQ_TEMP";
}
	
