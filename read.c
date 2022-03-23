#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include "read.h"

#define LIDAR_LOWBYTE_REG   0x10
#define LIDAR_HIGHBYTE_REG  0x0f
#define LIDAR_STATUS_REG    0x01


int readI2C(int fd) {
  if ((wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0b01000000) == 0b01000000) {
    printf("System error during measurement\n");
    exit(EXIT_FAILURE);
  }

  int lowbyte = wiringPiI2CReadReg8(fd,LIDAR_LOWBYTE_REG);
  int highbyte = wiringPiI2CReadReg8(fd,LIDAR_HIGHBYTE_REG);
  if (lowbyte == -1) {printf("Lowbyte reading error\n"); exit(EXIT_FAILURE);}
  if (highbyte == -1) {printf("Highbyte reading error\n"); exit(EXIT_FAILURE);}
  
  int ans = lowbyte + 256*highbyte;
  return ans;
}


char readUART() {
  int fd;
  char* port = "/dev/serial0";
  if ((fd=serialOpen(port,115200)) < 0){printf("Problem opening UART port\n"); exit(EXIT_FAILURE);}
  char ans = serialGetchar(fd);
  serialClose(fd);
  return ans;
}

float bytestof(uint8_t bytes[]) {
  // for 4 bytes big endian arrays
  // res = s*(2^p)*m
  
  // sign
  int s;
  if (bytes[0]>>7) {s = -1;}
  else {s = 1;}
  
  // power
  int p = ((bytes[0]&&0x80)<<1) + (bytes[1]>>7);
  
  // mantisse
  int m = ((bytes[1]&&0x80)<<16) + (bytes[2]<<8) + bytes[3];
  
  return s*pow(2,p)*m;
}
