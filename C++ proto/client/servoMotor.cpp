#include "servoMotor.h"
#include <sstream>

ServoMotor::ServoMotor(std::string n, std::string i, std::string m): Device(n, i, m) {}

void ServoMotor::update(double val) {
	value = val;
}

string ServoMotor::prepare(string info) {
	stringstream rstream;
    rstream << "SET_" << value;
    return rstream.str();
}
