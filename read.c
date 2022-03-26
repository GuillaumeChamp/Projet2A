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
  
// bloc to add if not in free running mode
/*int busy = wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0x1;
  while (busy) {
    printf("Busy\n");
    busy = wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0x1;
  }
  printf("Not busy\n");
*/
  
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
  int p = ((bytes[0]&0x7F)<<1) + ((bytes[1]&0x80)>>7) - 127;
  
  // mantisse
  float m =1;
  for (int i=0;i<7;i++) {
    m += ((bytes[1]>>(6-i))&0x1)*pow(2,-i-1);
  }
  for (int i=0;i<8;i++) {
    m += ((bytes[2]>>(7-i))&0x1)*pow(2,-i-8);
    m += ((bytes[3]>>(7-i))&0x1)*pow(2,-i-16);
  }
  
  printf("s=%d, p=%d, m=%f\n",s,p,m);
  return s*pow(2,p)*m;
}
