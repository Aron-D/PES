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

//i2c libraries
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>

#define TRUE 1 
#define FALSE 0 
#define PORT 8080 
#define SLAVE1 0x10 //temperatuursensor
#define SLAVE2 0x20 //verlichting in kamer


typedef struct{
	char* name;
	double value;
}device;


int main() {

	char line[50];
	double readVar;
	int err, dev_fd;
    const char* i2c_device = "/dev/i2c-1";
    uint8_t message;
    uint8_t ontvangenBytes[2];
    uint8_t vervanging;
    int teller = 0;
	
    device sensors[6] = {
		{"draadlozeKnop", 0},
		{"bedradeKnop", 0},
		{"Temperatuur", 0},
		{"Luchtvochtigheid", 0},
		{"bedradeBewegingsSensor", 0},
		{"bedradeDrukSensor", 0}

	};
	int sensorsLength = sizeof(sensors) / sizeof(sensors[0]);
	
	device actuators[2] = {
		{"bedradeLed", 0},
		{"bedradeLedstrip", 0}

	};	
	int actuatorsLength = sizeof(actuators) / sizeof(actuators[0]);
    
    while(1){
    
	//slave 1: temperatuur en demo led aansturing
		dev_fd = open(i2c_device, O_RDWR);
		ioctl(dev_fd, I2C_SLAVE, SLAVE1);
		
		//temperatuur opvragen
		message = 0x01;
		write(dev_fd, &message, 1);
		read(dev_fd, ontvangenBytes, 2);
		vervanging = (uint8_t)ontvangenBytes[1] - 28;
		sensors[2].value = vervanging;
        printf("%s : %d \n", sensors[2].name, vervanging);
		usleep(100000);
	//Sluit slave 1
	close(dev_fd);
	
    
	//slave 2: kamer verlichting

		dev_fd = open(i2c_device, O_RDWR);
		ioctl(dev_fd, I2C_SLAVE, SLAVE2);
		
		//bedradeBewegingsSensor
		message = 0x10;
		write(dev_fd, &message, 1);
		read(dev_fd, &ontvangenBytes, 1);
		vervanging = (uint8_t)ontvangenBytes[0] - 48;
		sensors[4].value = (double)vervanging;
        printf("%s : %.2lf \n", sensors[4].name, sensors[4].value);
        usleep(100000);


		//bedradeDrukSensor 
		message = 0x11;
		write(dev_fd, &message, 1);
		read(dev_fd, &ontvangenBytes, 1);
		vervanging = (uint8_t)ontvangenBytes[0] - 48;
		sensors[5].value = (double)vervanging;
        //zet licht aan na pas 3 opeenvolgende signalen door stray zero's
        if(sensors[5].value == 0){
            teller++;
            if (teller > 2){
                actuators[1].value = 1;
            }
        } 
        if (sensors[5].value > 0){ 
            teller = 0;
            actuators[1].value = 0;
        }
        
        printf("%s : %.2lf \n", sensors[5].name, sensors[5].value);
        usleep(100000);


        //bedradeLedStrip aansturing:		
			//bedradeLedStrip aan
        if(sensors[4].value == 1 && sensors[5].value != 0){
            message = 0x12; 
            write(dev_fd, &message, 1);
        }
            //bedradeLedstrip uit
		if(actuators[1].value != 0){
			message = 0x13;
            write(dev_fd, &message, 1);
		}
        
		
	//Sluit slave 2
	close(dev_fd);
    }
    return 0;
}
