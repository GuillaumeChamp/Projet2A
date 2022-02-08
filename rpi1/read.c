#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include "read.h"

#define LIDAR_I2C_ADDR  0x62
#define LIDAR_CMD_REG   0x00
#define LIDAR_MODE      0x04
#define LIDAR_FRM_REG   0x11 // Free Running Mode
#define LIDAR_FRM       0xFF

#define LIDAR_LOWBYTE_REG   0x10
#define LIDAR_HIGHBYTE_REG  0x0f
#define LIDAR_STATUS_REG    0x01

void LIDAR_read(void) {
  int fd = wiringPiI2CSetup(LIDAR_I2C_ADDR);
  if (fd == -1) {
    printf("Setup problem\n");
    exit(EXIT_FAILURE);
  }
  
  int frm_activation = wiringPiI2CWriteReg8(fd,LIDAR_FRM_REG,LIDAR_FRM);
  if (frm_activation == -1) {
    printf("Problem with setting up free running mode\n");
    exit(EXIT_FAILURE);
  }
  
  int askMeasure = wiringPiI2CWriteReg8(fd,LIDAR_CMD_REG,LIDAR_MODE);
  if (askMeasure == -1) {
    printf("Problem with measure asking\n");
    exit(EXIT_FAILURE);
  }
  
  int new_measure;
  while(1) {
//    askMeasure = wiringPiI2CWriteReg8(fd,LIDAR_CMD_REG,LIDAR_MODE); // line to add if no in free running mode
    new_measure = readI2C(fd);
    printf("I2C read = %d\n", new_measure);
    sleep(1);
  }
}

int readI2C(int fd) {
/*  int busy = wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0x01;
  while(busy) {
    printf("Busy\n");
    busy =wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0x01;
  }
  printf("Not busy\n");
*/
  // bloc to add if not in free running mode
  
if (wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0b01000000) == 0b01000000) {
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
  if ((fd=serialOpen(port,115200) < 0) return -1;
  char ans = serialGetchar(fd);
  serialClose(fd);
  return ans;
}

