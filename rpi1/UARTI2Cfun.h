#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdio.h>

int sendUART(char c);
int sendI2C(int c);
char readUART();
int readI2C(int fd);
int readUARTv2();
