// Client side C program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define PORT 8080
#define TRUE 1
#define DOOR_UNLOCK_TIME 5

#pragma warning(disable : 4496)
#pragma warnings(disable : 4996)

//client Pi, ontvangt waarde van draadloze wemos en stuurt deze door naar server die led aan doet

typedef struct{
	char* name;
	double value;
}device;

char rfid[50];

void turnLedOn(){
	printf("De led gaat nu aan\n");
}

void controlActuators(device* actuators){

	if(actuators[0].value > 0){
		turnLedOn();
	}
}

int writeToFile (FILE* file, device devices[], const char* fileName, int lengte){
	
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
	}
}

int logRFID(char* attempt){
	FILE* file;
	file = fopen("/home/blinky/Desktop/log.txt", "a");
	if (file == NULL){
		return 1;
	}
	fprintf(file, attempt);
	fclose(file);
}

	
int readFromFile(FILE* file, device devices[], const char* fileName){
	char line[50];
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
	return 0;
}

void handleMsg(char* key, char* value, int s, device* sensors, device* actuators) {
	char* msg = "OK\n";
	if (strcmp(key, "Ack") == 0) {
		
	} else if (strcmp(key, "Temperatuur") == 0) {
		sensors[2].value = atof(value);
	} else if (strcmp(key, "Luchtvochtigheid") == 0) {
		sensors[3].value = atof(value);
	} else if (strcmp(key, "draadlozeKnop") == 0) {
		sensors[0].value = atof(value);
		actuators[0].value = atof(value);
	} else if (strcmp(key, "bedradeKnop") == 0) {
		sensors[1].value = atof(value);
		actuators[0].value = atof(value);
	} else if (strcmp(key, "draadlozeLed") == 0) {
		actuators[0].value = atof(value);
	} else if (strcmp(key, "bedradeLed") == 0) {
		actuators[1].value = atof(value);
	} else if (strcmp(key, "draadlozeBewegingsSensor") == 0) {
		actuators[2].value = atof(value); //draadlozeLedStrip
	} else if (strcmp(key, "Toegang") == 0){
		strcpy(rfid, value);
	} else if (strcmp(key, "co2Sensor") == 0) {
		sensors[6].value = atof(value); //co2sensor
	} else if (strcmp(key, "Buzzer") == 0) {
		sensors[7].value = atof(value); //buzzer wemos
	} else if (strcmp(key, "Deurstatus") == 0) {
		actuators[3].value = atof(value); //servo wemos
	}else {
		printf("Unknown key received \"%s\" detected...\n", key);
		msg = "ERROR - INVALIDFLAG";
	}
	
	//send(msg, s, strlen(msg), 0);
}

int main(int argc, char const* argv[])
{
	//server pi adres: 10.42.0.1
	// wemos test button ip 10.42.0.83
	// wemos test led ip 10.42.0.108
	char* server_ips[] = { "10.42.0.1" , "10.42.0.25", "10.42.0.123", "10.42.0.108", "10.42.0.102", "10.42.0.27", "10.42.0.83" }; // voeg adressen toe per WEMOS
	int num_ips = sizeof(server_ips) / sizeof(server_ips[0]);
	
	FILE* sensorFile;
	FILE* actuatorFile;
	char* sensorFileName = "sensorFile";
	char* actuatorFileName = "actuatorFile";
	//time_t t = time(NULL);
	int isDoorOpen = 0;
	int doorOpenSecond = 0;
	int doorCounter = 0;
	char rfidWaarde[8];
	char RFIDAttempt[500];		
	
	if(sensorFile == NULL || actuatorFile == NULL){
		printf("sensors meow");
		return 1;
	}
	
	device sensors[9] = {
		{"draadlozeKnop", 0},
		{"bedradeKnop", 0},
		{"Temperatuur", 0},
		{"Luchtvochtigheid", 0},
		{"draadlozeBewegingsSensor", 0},
		{"Toegang", 0},
		{"co2Sensor", 0},
		{"Buzzer", 0},
		{"Deurstatus", 0}
	};
	int sensorsLength = sizeof(sensors) / sizeof(sensors[0]);
	
	device actuators[4] = { 
		{"draadlozeLed", 0},
		{"bedradeLed", 0} ,
		{"draadlozeLedStrip", 0},
		{"Servo", 0}
	};	
	int actuatorsLength = sizeof(actuators) / sizeof(actuators[0]);
	
	int socketCounter = 0; //verbinding teller
	int readOk = 0; //foutcontrole op lezen en schrijven
	
	//sockets loopen
	while(TRUE){
		usleep(100000);
		//usleep(1000000); // 1 seconde per nieuwe socket connectie
		//char* serverIp = "10.42.0.83";
		char* serverIp = server_ips[socketCounter % num_ips]; //switch socket servers	
		printf("Verbinding %d, IP: %s\n", socketCounter, serverIp);
			//lees bestand met waardes om deze te versturen
		readOk = readFromFile(sensorFile, sensors, sensorFileName);
		if (readOk == 1){
			printf("uitlezen niet meow\n");
		}
		readOk = readFromFile(actuatorFile, actuators, actuatorFileName);
		if (readOk == 1){
			printf("uitlezen niet meow\n");
		}	
		
			//switch bericht afhankelijk van server (Pi of Wemos)
		char hello[100];

		switch(socketCounter % num_ips) {
			case 0: //server PI
				sprintf(hello , "hoi");
				break;
			case 3: //draadloze buzzer wemos
				if(sensors[2].value > 27){
					sprintf(hello, "1\n\r");
				}
				else {
					sprintf(hello, "0\n\r");
				}
				break;
			case 1: //ledstrip wemos
				sprintf(hello, "%.0lf\n\r", actuators[2].value);
				break;
			case 2: //RFID wemos
				sprintf(hello, "%d\n\r", isDoorOpen);
				break;
			case 4: //servomotor
				//check of de deur al dicht mag
				if (isDoorOpen == 0 && sensors[5].value == 1) {
					isDoorOpen = 1;
					time_t t = time(NULL);
					struct tm tm = *localtime(&t);
					doorOpenSecond = tm.tm_sec;
				}
				if (isDoorOpen == 1) {
					time_t t = time(NULL);
					struct tm tm = *localtime(&t);
					printf("Comparing open second %d to new second %d\n\r", doorOpenSecond, tm.tm_sec);
					if (doorOpenSecond < tm.tm_sec) {
						doorCounter++;
						doorOpenSecond = tm.tm_sec;
						if (doorCounter > 4) {
							doorCounter = 0;
							isDoorOpen = 0;
							sensors[5].value = 0;
						}
					}
				}
				time_t t = time(NULL);
				struct tm* tm = localtime(&t);
				char dateBuffer[500]; 
				if (strftime(dateBuffer, sizeof(dateBuffer), "%Y/%m/%d at %H:%M:%S", tm) == 0){
					printf("tijd niet meow");
					return 1;
				}
		
				if (strcmp(rfid, "D935D814") == 0) {
					sensors[5].value = 1;
					sprintf(RFIDAttempt, "[%s] Valid login attempt on %s \n", rfid, dateBuffer);
					printf("Deze shit is valid\n\r");
				} else if (strcmp(rfid, "") == 0) {
					//ignorar
				} else {
					sensors[5].value = 0;
					sprintf(RFIDAttempt, "[%s] Invalid login attempt on %s \n", rfid, dateBuffer);
					printf("Niet valid maat\n\r");
				}
				if (strcmp(rfid, "") != 0) {
					printf("Ik ga nu loggen: %s\r\n", RFIDAttempt);
					readOk = logRFID(RFIDAttempt);
					if (readOk == 1) {
						printf("uitlezen niet meow\n");
					}
					memset(rfid, 0, sizeof(rfid));
				}
				//printf("Sending to the servomotor: %.0lf\n\r", sensors[5].value);
				sprintf(hello, "%.0lf\n\r", sensors[5].value);
				break;
			case 5: //lichtkrant
				t = time(NULL);
				struct tm tm2 = *localtime(&t);
				char lichtkrantTijd[20];
				sprintf(lichtkrantTijd, "%d:%d", tm2.tm_hour, tm2.tm_min);
				sprintf(hello, "%s - Temp: %.1lf C - Hum: %.1lf - C02: %.0lf\n\r", lichtkrantTijd, sensors[2].value, sensors[3].value, sensors[6].value);
				break;
			case 6:
				sprintf(hello, "geef me je waarde bro");
				break;
		}
		printf("Ik ga sturen: %s\n", hello);
		//test:
		//char* hello = "1\n";
		
			//socket met server RPI
		int status, valread, client_fd;
		struct sockaddr_in serv_addr;
		char buffer[1024] = { 0 };
		if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			return -1;
		}
	
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		// Convert IPv4 and IPv6 addresses from text to binary
		// form

		if (inet_pton(AF_INET, serverIp, &serv_addr.sin_addr) //serverIp bepaalt welke server benadert wordt
			<= 0) {
			printf(
			"\nInvalid address/ Address not supported \n");
			return -1;
		}

		if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
			printf("\nConnection failed\n");
			return -1;
		}
		
		send(client_fd, hello, strlen(hello), 0);
		//printf("Hello message sent\n");
		valread = read(client_fd, buffer, 1024 - 1); // subtract 1 for the null
		// terminator at the end
		printf("%s\n", buffer);
		socketCounter++;		
		const char delimiter[] = ",";
		char* token = strtok(buffer, delimiter);
		while (token != NULL) {
			char str[50];
			char val[50];
			sscanf(token, "%s : %s", str, val);
			handleMsg(str, val, client_fd, sensors, actuators);
			token = strtok(NULL, delimiter);
		}
	
		// closing the connected socket
		close(client_fd);
		
		//schrijf doorgegeven waardes naar bestanden
		readOk = writeToFile(sensorFile, sensors, sensorFileName, sensorsLength);
		if (readOk == 1){
			printf("schrijven niet meow\n");
		}
		readOk = writeToFile(actuatorFile, actuators, actuatorFileName, actuatorsLength);
		if (readOk == 1){
			printf("schrijven niet meow\n");
		}
		printf("gegevens succesvol naar bestand geschreven\n");
				
				//einde socket server RPI
		
		//stuur de actuatoren aan en print waardes structs
		/*for(int i = 0; i< actuatorsLength; i++){
			printf("%s : %.2lf \n", actuators[i].name, actuators[i].value);
		}
		for(int i = 0; i< sensorsLength; i++){
			printf("%s : %.2lf \n", sensors[i].name, sensors[i].value);
		}*/
		
		//controlActuators(actuators); //alleen voor test zonder WEMOS, deze pi stuurt alleen draadloze actuatoren aan.
		
		printf("\n");

	}
	
	return 0;
}
