#include "motionSensor.h"

MotionSensor::MotionSensor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void MotionSensor::update(double val) {
	value = val;
}

string MotionSensor::prepare(string info) {
	return "REQ_MOT";
}
