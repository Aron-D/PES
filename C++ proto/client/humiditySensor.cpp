#include "humiditySensor.h"

HumiditySensor::HumiditySensor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void HumiditySensor::update(double val) {
	value = val;
}

string HumiditySensor::prepare(string info) {
	return "REQ_HUM";
}
