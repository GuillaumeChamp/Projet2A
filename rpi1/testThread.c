#include <wiringPi.h>
#include <stdio.h>
#include "UARTI2Cfun.h"

PI_THREAD (SendDataUART) {
  piHiPri(8);
  while(1) {
    sendUART('8');
    sendUART('e');
    sendUART('A');
  }
}

PI_THREAD (SendDataI2C) {
  piHiPri(25);
  while(1) {
    sendI2C(1000);
    sendI2C(400);
    sendI2C(20);
  }
}

int main(void) {
  wiringPiSetup();
  int i = piHiPri(10);
  int x = piThreadCreate(SendDataUART);
  int y = piThreadCreate(SendDataI2C);
  printf("started p = %d, thread : %d\n",i,x);
  while(1) {
    printf("In = %c\n",readUARTv2());
  }
  return 0;
}
