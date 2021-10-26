#include "decisionMaker.h"
#include <stdio.h>
#include <wiringPi.h>

#define LED     1

int main (void)
{
  printf ("Raspberry Pi Quick2Wire blink\n") ;

  wiringPiSetup () ;
  pinMode (LED, OUTPUT) ;

  for (;;)
  {
    digitalWrite (LED, HIGH) ;  // On
    delay (500) ;               // mS
    digitalWrite (LED, LOW) ;   // Off
    delay (500) ;
  }
  return 0 ;
}
