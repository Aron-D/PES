#include "wirelessPi.h"
#include "temperatureSensor.h"
#include "humiditySensor.h"
#include "co2Sensor.h"
#include "moistureSensor.h"
#include "pressureSensor.h"
#include "buzzer.h"
#include "ledStrip.h"
#include "motionSensor.h"
#include "servoMotor.h"
#include "rfidSensor.h"
#include "lichtKrant.h"
#include "adminPanel.h"
#include <unistd.h>

#define TRUE 1
#define IP_EMBEDPI "10.42.0.1"
#define IP_ADMIN "10.42.0.148"
#define IP_LEDSTRIP "10.42.0.25"
#define IP_BUZZER "10.42.0.108"
#define IP_RFID "10.42.0.123"
#define IP_SERVO "10.42.0.102"
#define IP_TICKER "10.42.0.27"
#define IP_MOTION "10.42.0.83"

WirelessPi::WirelessPi() {
	socket = *new ClientSocket();
	handler = *new MessageHandler();
	manager = *new FileManager();
}

/**
 * Main loop of PI
 * Loops through every device, building comm message based on current data,
 * then updates device data based off responses.
 */
void WirelessPi::start() {
    //TODO: RFID/Servo behaviour, timestamps, time in bed
	devices.push_back(make_unique<TemperatureSensor>("Temperature", IP_EMBEDPI, "TEMP"));
	devices.push_back(make_unique<HumiditySensor>("Humidity", IP_EMBEDPI, "HUM"));
	devices.push_back(make_unique<CO2Sensor>("CO2", IP_EMBEDPI, "CO2"));
	devices.push_back(make_unique<MoistureSensor>("Moisture", IP_EMBEDPI, "MSTR"));
	devices.push_back(make_unique<PressureSensor>("Pressure", IP_EMBEDPI, "PRS"));
	devices.push_back(make_unique<MotionSensor>("Motion", IP_MOTION, "MOV"));
	devices.push_back(make_unique<LEDStrip>("LEDStrip", IP_LEDSTRIP, "LED"));
	devices.push_back(make_unique<ServoMotor>("ServoMotor", IP_SERVO, "SERV"));
	devices.push_back(make_unique<RFIDSensor>("RFIDSensor", IP_RFID, "RFID"));
    devices.push_back(make_unique<Buzzer>("Buzzer", IP_BUZZER, "BUZZ"));
	devices.push_back(make_unique<LichtKrant>("LichtKrant", IP_TICKER, "LKRT"));
	devices.push_back(make_unique<AdminPanel>("AdminPanel", IP_ADMIN, "ADMN"));
	
	manager.readFromFile(devices, "sensorFile");
    string deviceIP, deviceMsg, receivedMsg;
	
	while(TRUE) {
        ///All devices that always send the same comm string
        for (int i = 0; i < 8; i++) {
            deviceIP = devices[i]->getIp();
            deviceMsg = devices[i]->prepare("");
            receivedMsg = socket.communicate(deviceIP, deviceMsg);
            handler.handleMessage(devices[i].get(), receivedMsg);
            usleep(50000);
        }

        ///BUZZ
        deviceIP = devices[8]->getIp();
        char buzzerInfo[100];
        std::sprintf(buzzerInfo, "%lf", devices[0]->getValue());
        deviceMsg = devices[8]->prepare(buzzerInfo);
        receivedMsg = socket.communicate(deviceIP, deviceMsg);
        handler.handleMessage(devices[8].get(), receivedMsg);
        usleep(50000);
		
		///TICK
		deviceIP = devices[9]->getIp();
		char tickerInfo[100];
		std::sprintf(tickerInfo, "%lf, %lf, %lf", devices[0]->getValue(), devices[1]->getValue(), devices[2]->getValue());
		deviceMsg = devices[9]->prepare(tickerInfo);
		receivedMsg = socket.communicate(deviceIP, deviceMsg);
		handler.handleMessage(devices[9].get(), receivedMsg);
        usleep(50000);
		
		///ADMIN
		deviceIP = devices[10]->getIp();
		char adminInfo[100];
		std::sprintf(adminInfo, "%lf, %lf, %lf, %lf, %s", devices[0]->getValue(), devices[1]->getValue(), devices[2]->getValue(), devices[3]->getValue(), "Time info not yet supported");
		deviceMsg = devices[10]->prepare("");
		receivedMsg = socket.communicate(deviceIP, deviceMsg);
		handler.handleMessage(devices[10].get(), receivedMsg);
        usleep(50000);
		
		manager.writeToFile(devices, "sensorFile");
		usleep(100000);
	}
}
