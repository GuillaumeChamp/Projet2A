#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include "UARTI2Cfun.h"

#define LIDAR_I2C_ADDR  0x62
#define LIDAR_CMD_REG   0x00
#define LIDAR_MODE      0x04
#define LIDAR_FRM_REG   0x11 // Free Running Mode
#define LIDAR_FRM       0xFF

int main(void) {
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
