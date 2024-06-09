#include "device.h"

Device::Device(string n, string i, string m) : deviceName(n), ip(i), message(m), value(0) {}

string Device::getIp() {
	return ip;
}

string Device::getName() {
	return deviceName;
}

double Device::getValue() {
	return value;
}

string Device::getComMSG() {
	return message;
}

/**
 * Builds the message to send through the socket
 * @param info - Possibly required info to build message
 * @return constructed message
 */
string Device::prepare(string info) {
	return "ERR_UNDEF_MSG\n\r";
}
