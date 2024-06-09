#include "fileManager.h"
#include <stdio.h>

#define PATH_SENSORS "/home/blinky/Desktop/OOP/logfiles/sensors.txt"
#define PATH_ACTUATORS "/home/blinky/Desktop/OOP/logfiles/actuators.txt"
#define PATH_LOG "/home/blinky/Desktop/OOP/logfiles/log.txt"

/**
 * Writes device values stored in memory to a corresponding txt file
 * @param devices - List of devices in memory
 * @param fileName
 * @return operation unsuccessful
 */
bool FileManager::writeToFile(vector<unique_ptr<Device>> devices, string fileName) {
	FILE* file;
	if(fileName == "sensorFile") {
		file = fopen("/home/blinky/Desktop/OOP/logfiles/sensors.txt", "w");
		if(file == NULL){
			return 1;
		}		
		for(int i = 0; i < devices.size(); i++){
		fprintf(file, "%s : %.2lf\n", devices[i].get()->getName(), devices[i].get()->getValue());
		}
		fclose(file);
		return 0;
	}
	if(fileName == "actuatorFile") {
		file = fopen("/home/blinky/Desktop/OOP/logfiles/actuators.txt", "w");
		if(file == NULL){
			return 1;
		}		
		for(int i = 0; i < devices.size(); i++){
		fprintf(file, "%s : %.2lf\n", devices[i].get()->getName(), devices[i].get()->getValue());
		}
		fclose(file);
		return 0;
	}
}

/**
 * Reads devices values from txt file to active memory
 * @param devices - List of devices to store values in
 * @param fileName
 * @return operation unsuccessful
 */
bool FileManager::readFromFile(vector<unique_ptr<Device>> devices, string fileName) {
	char line[500];
    FILE* file;
	double readVar;
	
	if(fileName == "sensorFile") {
		file = fopen("/home/blinky/Desktop/OOP/logfiles/sensors.txt", "r");
		if(file == NULL) {
			return 1;
		}
	}
	if(fileName == "actuatorFile") {
		file = fopen("/home/blinky/Desktop/OOP/logfiles/actuators.txt", "r");
		if(file == NULL) {
			return 1;
		}
	}
	while (fscanf(file, "%s : %lf", line, &readVar) == 2) {
		if (line == devices[0].get()->getName()) {
		    devices[0].get()->update(readVar);
		}
        if (line == devices[1].get()->getName()) {
            devices[1].get()->update(readVar);
        }
        if (line == devices[2].get()->getName()) {
            devices[2].get()->update(readVar);
        }
        if (line == devices[3].get()->getName()) {
            devices[3].get()->update(readVar);
        }
        if (line == devices[4].get()->getName()) {
            devices[4].get()->update(readVar);
        }
        if (line == devices[5].get()->getName()) {
            devices[5].get()->update(readVar);
        }
        if (line == devices[6].get()->getName()) {
            devices[6].get()->update(readVar);
        }
        if (line == devices[7].get()->getName()) {
            devices[7].get()->update(readVar);
        }
	}
	fclose(file);	
	return 0;
}

/**
 * Logs RFID checkin attemps to log.txt
 * @param attempt - String containing login ID and timestamp
 * @return operation unsuccessful
 */
bool FileManager::logRFID(string attempt) {
	FILE* file;
	file = fopen("/home/blinky/Desktop/OOP/logfiles/log.txt", "a");
	if (file == NULL){
		return 1;
	}
	std::fprintf(file, attempt);
	fclose(file);
    return 0;
}


FileManager::FileManager() {

}