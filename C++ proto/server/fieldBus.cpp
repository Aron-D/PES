#include "fieldBus.h" 
#include <string>

//i2c libraries
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>

/*
 * Microcontroller 1
 * - Temperatuursensor
 * - Luchtvochtigheidsensor
 * - CO2-sensor
 * - Grondvochtigheidssensor
 */ 
#define SLAVE1 0x10
/*
 * Microcontroller 2
 * - Druksensor
 * - Ledstrip
 * - Bewegingssensor
 */ 
#define SLAVE2 0x20


FieldBus::FieldBus(uint8_t ID): i2cDevice("/dev/i2c-1"), devFd(0), slaveID(ID), message(0x0){

	devices.push_back(make_unique<Device>("Temperature", "10.42.0.1", "TEMP"));
	devices.push_back(make_unique<Device>("Humidity", "10.42.0.1", "HUM"));
	devices.push_back(make_unique<Device>("CO2", "10.42.0.1", "CO2"));
	devices.push_back(make_unique<Device>("Moisture", "10.42.0.1", "MSTR"));
	devices.push_back(make_unique<Device>("Pressure", "10.42.0.1", "PRS"));
	devices.push_back(make_unique<Device>("MotionEmbedded", "10.42.0.1", "MOV_E"));
	devices.push_back(make_unique<Device>("LEDStripEmbedded", "10.42.0.1", "LED_E"));

}

void FieldBus::updateAll(){
	
}

void FieldBus::requestVal(string flag){
	string response;
	if (flag, "hoi"){
		//ack
	} else if (flag == "TEMP")  {
		sprintf(response, "%s", devices[0]->getValue());
	} else if (strcmp(flag, "HUM") {
		sprintf(response, "%s", devices[1]->getValue());
	} else if (strcmp(flag, "CO2") {
		sprintf(response, "%s", devices[2]->getValue());
	} else if (strcmp(flag, "MSTR") {
		sprintf(response, "%s", devices[3]->getValue());
	} else if (strcmp(flag, "PRS") {
		sprintf(response, "%s", devices[4]->getValue());
	} else if (strcmp(flag, "MOV_E") {
		sprintf(response, "%s", devices[5]->getValue());
	} else if (strcmp(flag, "LED_E") {
		sprintf(response, "%s", devices[6]->getValue());
	} else {
		printf("Unknown flag \"%s\" received, ignoring...\n", flag);
	}
	return response;
}
