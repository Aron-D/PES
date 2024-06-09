#include "rfidSensor.h"

RFIDSensor::RFIDSensor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void RFIDSensor::update(double val) {
	value = val;
}

string RFIDSensor::prepare(string info) {
	return "REQ_RFID";
}
