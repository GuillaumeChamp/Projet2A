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
  char* port = "/dev/serial0";
  if ((fd=serialOpen(port,110) < 0) {
    return 1;
  }
  serialPutchar(fd,c);
  serialClose(fd);
}

int sendI2C(int c) {
  int fd = wiringPiI2CSetup(0x62);
  wiringPiI2CWrite(fd,c);
  return 0;
}

char readUART() {
  int fd;
  char* port = "/dev/serial0";
  if ((fd=serialOpen(port,600) < 0) {
    return 1;
  }
  char ans = serialGetchar(fd);
  serialClose(fd);
  return ans;
}

int readI2C(int fd) {
  int busy = wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0x01;
  while(busy) {
    printf("Busy\n");
    busy =wiringPiI2CReadReg8(fd,LIDAR_STATUS_REG)&0x01;
  }
  printf("Not busy\n");
  
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

int readUARTv2() { //formerly in read.c file
  int fd;
  char ch;
  if (wiringPiSetup()<0) {return 0;}
  if ((fd=serialOpen("/dev/serial1",110))<0) {return 1;}
  while(1) {
    if (serailDataAvail(fd)>0) {
      ch = serialGetchar(fd);
      /*if (ch!=0)*/ printf("%c\n",ch);
      printf("while loop\n");
    }
  }
  serialClose(fd);
  return 0;
}
