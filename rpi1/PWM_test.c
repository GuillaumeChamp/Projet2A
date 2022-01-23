#include <wiringPi.h>
#include <softPWM.h>

void main() {
  wiringPiSetup();
  softPwmCreate(1,0,100);
  while(1) {
    softPwmWrite(1,50);
  }
}
