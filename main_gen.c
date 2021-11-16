
#include <wiringPi.h>
#include <stdio.h>
#include "write.h"

int main (void)
{
        wiringPiSetup();
        printf("started\n");
        for(int i =0 ;i==100000;i++)
        {
                send('A');
                delay(500);
                send('B');
                delay(500);
        }
        return 0;
}

