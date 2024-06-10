#include "fileManager.h"

#define PATH_SENSORS "/home/blinky/Desktop/OOP/logfiles/sensors.txt"
#define PATH_ACTUATORS "/home/blinky/Desktop/OOP/logfiles/actuators.txt"
#define PATH_LOG "/home/blinky/Desktop/OOP/logfiles/log.txt"

/**
 * Writes all device values to corresponding txt file
 */
bool FileManager::writeToFile() {
	//TODO
	/**
	if(strcmp(fileName, "sensorFile") == 0){
		file = fopen("/home/blinky/Desktop/sensors.txt", "w");
		if(file == NULL){
			return 1;
		}		
		for(int i = 0; i < lengte; i++){
		fprintf(file, "%s : %.2lf\n", devices[i].name, devices[i].value);
		}
		fclose(file);
		return 0;
	}
	if(strcmp(fileName, "actuatorFile") == 0){
		file = fopen("/home/blinky/Desktop/actuators.txt", "w");
		if(file == NULL){
			return 1;
		}		
		for(int i = 0; i < lengte; i++){
		fprintf(file, "%s : %.2lf\n", devices[i].name, devices[i].value);
		}
		fclose(file);
		return 0;
	}*/
}

/**
 * Reads in all device values from corresponding txt file
 */
bool FileManager::readFromFile() {
	//TODO
	/**char line[50];
	double readVar;
	
	if(strcmp(fileName, "sensorFile") == 0){
		file = fopen("/home/blinky/Desktop/sensors.txt", "r");
		if(file == NULL){
			return 1;
		}
	}
	if(strcmp(fileName, "actuatorFile") == 0){
		file = fopen("/home/blinky/Desktop/actuators.txt", "r");
		if(file == NULL){
			return 1;
		}
	}
	while (fscanf(file, "%s : %lf", line, &readVar) == 2){
		if(strcmp(line, devices[0].name) == 0){
		devices[0].value = readVar;
		}
		if(strcmp(line, devices[1].name) == 0){
		devices[1].value = readVar;
		}
		if(strcmp(line, devices[2].name) == 0){
		devices[2].value = readVar;
		}
		if(strcmp(line, devices[3].name) == 0){
		devices[3].value = readVar;
		}
	}
	fclose(file);	
	return 0;*/
}

/**
 * Logs RFID checkin attemps to log.txt
 */
bool FileManager::logRFID() {
	//TODO
	/**FILE* file;
	file = fopen("/home/blinky/Desktop/log.txt", "a");
	if (file == NULL){
		return 1;
	}
	fprintf(file, attempt);
	fclose(file);*/
}



