#include <wiringPi.h>
#include <stdio.h>
#include "write.h"

PI_THREAD (SendData)
{
        piHiPri(8);
        while(1){
                send('8');
                send('e');
                send('A');
        }
}

PI_THREAD (SendDataI2C)
{
        piHiPri(25);
        while(1){
                I2Csend(1000);
                I2Csend(400);
                I2Csend(20);
        }
}

int main (void)
{
        wiringPiSetup();
        int i = piHiPri(10);
        int x = piThreadCreate(SendData);
        //int y = piThreadCreate(SendDataI2C);
        printf("started p = %d ,thread : %d \n",i,x);
        while(1)
        {
                printf("In = %c \n",read());
        }
        return 0;
}
