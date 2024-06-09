#include "moistureSensor.h"

MoistureSensor::MoistureSensor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void MoistureSensor::update(double val) {
	value = val;
}

string MoistureSensor::prepare(string info) {
	return "REQ_MSTR";
}
