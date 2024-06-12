#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <pigpio.h>
#include <time.h>

//i2c libraries
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>

#define TRUE 1 
#define FALSE 0 
#define PORT 8080 

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

typedef struct {
	char* name;
	double value;
} device;

void i2cBusAansturing(device sensors[], device actuators[]) {
	char line[50];
	double readVar;
	int err, dev_fd;
    const char* i2c_device = "/dev/i2c-1";
    uint8_t message;
    uint8_t ontvangenBytes[4];
    static int druksensorSequenceTeller = 0;
	
	/*
	 * Start proces microcontroller 1
	 */ 
	dev_fd = open(i2c_device, O_RDWR);
	ioctl(dev_fd, I2C_SLAVE, SLAVE1);

	/*
	 * Temperatuursensor uitlezen:
	 * Vraagt temperatuursensor waarde op van microcontroller 1
	 * 	Message flags:
	 * 0x01 - Request temperatuursensor
	 */ 
	message = 0x01;
	write(dev_fd, &message, 1);
	read(dev_fd, ontvangenBytes, 4);
    sensors[2].value = atof(ontvangenBytes);
    printf("%s : %.1lf \n", sensors[2].name, sensors[2].value);
    memset(ontvangenBytes, 0, sizeof(ontvangenBytes));
	usleep(100000);
        
    /*
     * Luchtvochtigheidssensor uitlezen:
     * Vraagt Luchtvochtigheidssensor waarde op van microcontroller 1
     * 	Message flags:
     * 0x05 - Request luchtvochtigheidssensor
     */ 
    message = 0x05;
	write(dev_fd, &message, 1);
	read(dev_fd, ontvangenBytes, 4);
    sensors[3].value = atof(ontvangenBytes);
    printf("%s : %.1lf \n", sensors[3].name, sensors[3].value);
    memset(ontvangenBytes, 0, sizeof(ontvangenBytes));
    usleep(100000);

    /*
     * CO2-sensor uitlezen:
     * Vraagt CO2-sensor waarde op van microcontroller 1
     * 	Message flags:
     * 0x06 - Request CO2-sensor
     */ 
	message = 0x06;
	write(dev_fd, &message, 1);
	read(dev_fd, ontvangenBytes, 4);
    sensors[6].value = atof(ontvangenBytes);
    printf("%s : %.1lf \n", sensors[6].name, sensors[6].value);
    memset(ontvangenBytes, 0, sizeof(ontvangenBytes));
	usleep(100000);
        
    /*
     * Grondvochtigheidssensor uitlezen:
     * Vraagt grondvochtigheidssensor waarde op van microcontroller 1
     * 	Message flags:
     * 0x07 - Request grondvochtigheidssensor
     */ 
    message = 0x07;
	write(dev_fd, &message, 1);
	read(dev_fd, ontvangenBytes, 4);
	sensors[7].value = atof(ontvangenBytes);
    printf("%s : %.1lf \n", sensors[7].name, sensors[7].value); 
    memset(ontvangenBytes, 0, sizeof(ontvangenBytes));       
	usleep(100000);
		    
	/*
	 * Einde proces microcontroller 1
	 */ 
	close(dev_fd);
	
	/*
	 * Start proces microcontroller 2
	 */ 
	dev_fd = open(i2c_device, O_RDWR);
	ioctl(dev_fd, I2C_SLAVE, SLAVE2);
		
	/*
	 * Bedrade bewegingssensor uitlezen:
	 * Vraagt bewegingssensor waarde op van microcontroller 2
	 * 	Message flags:
	 * 0x10 - Request bewegingssensor
	 */ 
	message = 0x10;
	write(dev_fd, &message, 1);
	read(dev_fd, &ontvangenBytes, 1);
	sensors[4].value = atof(ontvangenBytes);
    printf("%s : %.1lf \n", sensors[4].name, sensors[4].value);
    memset(ontvangenBytes, 0, sizeof(ontvangenBytes));
    usleep(100000);

	/*
	 * Bedrade druksensor uitlezen:
	 * Vraagt druksensor waarde op van microcontroller 2 en zet
	 * de ledstrip uit als de druksensor 3 keer achter elkaar
	 * aangeeft dat er op gedrukt wordt
	 * 	Message flags:
	 * 0x11 - Request druksensor
	 */ 
	message = 0x11;
	write(dev_fd, &message, 1);
	read(dev_fd, &ontvangenBytes, 1);
	sensors[5].value = atof(ontvangenBytes);
                
    /* 
     * Zet licht pas aan na 3 opeenvolgende signalen om stray zeroes te
     * voorkomen
     */ 
    if(sensors[5].value == 0){
        druksensorSequenceTeller++;
        if (druksensorSequenceTeller > 2) {
            actuators[1].value = 1;
        }
    } 
    if (sensors[5].value > 0){ 
        druksensorSequenceTeller = 0;
        actuators[1].value = 0;
    }
        
    printf("%s : %.2lf \n", sensors[5].name, sensors[5].value);
    memset(ontvangenBytes, 0, sizeof(ontvangenBytes));
    usleep(100000);

    /* 
     * Bedrade ledstrip aansturing:
     * Stuurt bericht naar microcontroller 2 om bedrade ledstrip
     * aan of uit te zetten
     * 	Message flags:
     * 0x12 - LED aan, daymode
     * 0x13 - LED aan, nightmode
     * 0x14 - LED uit
     */ 
    if(sensors[4].value == 1 && sensors[5].value != 0) {
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		if (tm.tm_hour > 8 && tm.tm_hour < 22) {
			message = 0x12;
		} else {
			message = 0x13;
		} 
        write(dev_fd, &message, 1);
    }
        
	if(actuators[1].value != 0) {
		message = 0x14;
        write(dev_fd, &message, 1);
	}

	/*
	 * Einde proces microcontroller 2
	 */ 
	close(dev_fd);
} 

int writeToFile (FILE* file, device devices[], const char* fileName, int lengte) {
	
	/*
	 * Schrijf de device waardes naar sensors.txt
	 */ 
	if(strcmp(fileName, "sensorFile") == 0) {
		file = fopen("/home/blinky/Desktop/sensors.txt", "w");
		if(file == NULL) {
			return 1;
		}		
		for(int i = 0; i < lengte; i++) {
			fprintf(file, "%s : %.2lf\n", devices[i].name, devices[i].value);
		}
		fclose(file);
		return 0;
	}
	
	/*
	 * Schrijf de device waardes naar actuators.txt 
	 */
	if(strcmp(fileName, "actuatorFile") == 0) {
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
	
int readFromFile(FILE* file, device devices[], const char* fileName) {
	char line[50];
	double readVar;
	
	/*
	 * Lees de sensorfile uit
	 */ 
	if(strcmp(fileName, "sensorFile") == 0) {
		file = fopen("/home/blinky/Desktop/sensors.txt", "r");
		if(file == NULL){
			return 1;
		}
	}
	
	/*
	 * Lees de actuatorfile uit
	 */ 
	if(strcmp(fileName, "actuatorFile") == 0){
		file = fopen("/home/blinky/Desktop/actuators.txt", "r");
		if(file == NULL){
			return 1;
		}
	}
	
	/*
	 * Ga de ingelezen data af en vul de devices struct in
	 */ 
	while (fscanf(file, "%s : %lf", line, &readVar) == 2) {
		if(strcmp(line, devices[0].name) == 0) {
			devices[0].value = readVar;
		}
		if(strcmp(line, devices[1].name) == 0) {
			devices[1].value = readVar;
		}
		if(strcmp(line, devices[2].name) == 0) {
			devices[2].value = readVar;
		}
		if(strcmp(line, devices[3].name) == 0) {
			devices[3].value = readVar;
		}
	}
	
	fclose(file);	
	return 0;
}

void handleMsg(char* key, double value, int s, device* sensors, device* actuators) {
	char* msg = "OK\n";
	if (strcmp(key,"hoi") == 0) {
		//ack
	} else if (strcmp(key, "Temperatuur") == 0) {
		sensors[2].value = value;
	} else if (strcmp(key, "Luchtvochtigheid") == 0) {
		sensors[3].value = value;
	} else if (strcmp(key, "draadlozeKnop") == 0) {
		sensors[0].value = value;
		actuators[1].value = value;
	} else if (strcmp(key, "bedradeKnop") == 0) {
		sensors[1].value = value;
		actuators[0].value = value;
	} else {
		printf("Unknown key \"%s\" received, ignoring...\n", key);
		msg = "ERROR - INVALIDFLAG";
	}
	
	send(s, msg, strlen(msg), 0);
}

int main(int argc, char* argv[]) {
	FILE* sensorFile;
	FILE* actuatorFile;
	char* sensorFileName = "sensorFile";
	char* actuatorFileName = "actuatorFile";
	
	device sensors[8] = {
		{"draadlozeKnop", 0},
		{"bedradeKnop", 0},
		{"Temperatuur", 0},
		{"Luchtvochtigheid", 0},
		{"bedradeBewegingsSensor", 0},
		{"bedradeDrukSensor", 0},
		{"co2Sensor", 0},
		{"grondgevoeligeSensor", 0}

	};
	int sensorsLength = sizeof(sensors) / sizeof(sensors[0]);
	
	device actuators[2] = {
		{"bedradeLed", 0},
		{"bedradeLedstrip", 0}

	};	
	
	int actuatorsLength = sizeof(actuators) / sizeof(actuators[0]);
	int opt = TRUE;
	int master_socket, addrlen, new_socket, client_socket[30],
		max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[1025]; //data buffer of 1K 
	int readOk = 0; //foutcontrole op lezen en schrijven

	//set of socket descriptors 
	fd_set readfds;

	//a message 
	char* message = "ECHO Daemon v1.0 \r\n";

	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
	}

	//create a master socket 
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
		sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);

	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection 
	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	//loop socket connecties
	while (TRUE) {
		//lees bestand met waardes
		readOk = readFromFile(sensorFile, sensors, sensorFileName);
		if (readOk == 1){
			printf("Unable to read from sensor file\n");
		}
		
		readOk = readFromFile(actuatorFile, actuators, actuatorFileName);
		if (readOk == 1){
			printf("Unable to read from actuator file\n");
		}
	
		char hello[500];
		sprintf(hello , "%s : %.2lf, %s : %.2lf, %s : %.2lf, %s : %.2lf, %s : %.2lf, %s : %.1lf", sensors[1].name, sensors[1].value, sensors[2].name, sensors[2].value, sensors[3].name, sensors[3].value, sensors[6].name, sensors[6].value, sensors[5].name, sensors[5].value, sensors[7].name, sensors[7].value);
		
		//clear the socket set 
		FD_ZERO(&readfds);

		//add master socket to set 
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set 
		for (i = 0; i < max_clients; i++)
		{
			//socket descriptor 
			sd = client_socket[i];

			//if valid socket descriptor then add to read list 
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function 
			if (sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		/*if (activity == 1){
			perror("select");
			exit(EXIT_FAILURE);
		}*/

		if ((activity < 0) && (errno != EINTR)) {
			printf("select error");
		}

		//If something happened on the master socket, 
		//then it's an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) {
			if ((new_socket = accept(master_socket,
				(struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
				(address.sin_port));

			//send new connection greeting message 
			if (send(new_socket, hello, strlen(hello), 0) != strlen(hello))	{
				perror("send");
			}

			puts("Welcome message sent successfully");

			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) {
				//if position is empty 
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					break;
				}
			}
		}

		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds))	{
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((valread = read(sd, buffer, 1024)) == 0) {
					//Somebody disconnected , get his details and print 
					getpeername(sd, (struct sockaddr*)&address, \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n",
						inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse 
					close(sd);
					client_socket[i] = 0;
				}

				//Echo back the message that came in 
				else {
					//print buffer
					printf("%s\n", buffer); 
					//set the string terminating NULL byte on the end 
					//of the data read 
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
					
					/*
					 * Ga de ontvangen data af en pas de devices aan met de nieuwe waardes
					 */ 
					const char delimiter[] = ",";
					char* token = strtok(buffer, delimiter);
					while (token != NULL) {
						char str[50];
						double val;
						sscanf(token, "%s : %lf", str, &val);
						handleMsg(str, val, sd, sensors, actuators);
						token = strtok(NULL, delimiter);
					}
		
					printf("\n");									
				}
			}
		}

        i2cBusAansturing(sensors, actuators); //gebruikt de waardes van de devices
        
		readOk = writeToFile(sensorFile, sensors, sensorFileName, sensorsLength);
		if (readOk == 1) {
			printf("Could not write to file\n");
		}
		readOk = writeToFile(actuatorFile, actuators, actuatorFileName, actuatorsLength);
		if (readOk == 1) {
			printf("Could not write to file\n");
		}
    
	}
	return 0;
}
