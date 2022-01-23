#include <wiringSerial.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define LIDAR_LOWBYTE_REG 0x10
#define LIDAR_HIGHBYTE_REG 0x0f
#define LIDAR_STATUS_REG 0x01

int sendUART(char c) {
  int fd;
  char*port = "/dev/serial0";
  if ((fd=serialOpen(port,110) < 0) {return 1;}
  serialPutChar(fd,c);
  serialClose(fd);
}

int sendI2C(int c) {
  int fd = wiringPiI2CSetup(0x62);
  wiringPiI2CWrite(fd,c);
  return 0;
}

      //not finished yet (last functions + .h + UARTWrite_test.c)
