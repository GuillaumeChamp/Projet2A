
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
                send('B');
        }
        return 0;
}

