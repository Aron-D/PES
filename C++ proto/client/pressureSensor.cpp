#include "pressureSensor.h"

PressureSensor::PressureSensor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void PressureSensor::update(double val) {
	value = val;
}

string PressureSensor::prepare(string info) {
	return "REQ_PRS";
}
