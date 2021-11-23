
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include "read.h"

int main(void) {
        int serial_port;
        char dat;
        serial_port = serialOpen("/dev/ttyS0",110);
        printf("%d\n",serial_port);
        if (serial_port<0) {
                fprintf(stderr, "Unable to open serial device: %s\n",strerror(errno));
                printf("Error 1");
                return 1;
        }
        if (wiringPiSetup() == -1) {
                fprintf(stdout,"Unable to start wiringPi: %s\n",strerror(errno));
                return 1;
        }
        while (1) {
                if (serialDataAvail(serial_port)>0) {
                //      printf("dans le if\n");
                        dat = serialGetchar(serial_port);       // receive character serially
                        printf("%c",dat);
                        //fflush(stdout);
                }
                else {
//                      printf("No data yet\n");
                }
        }
}



