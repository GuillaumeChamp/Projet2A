
#include <wiringPi.h>
#include <stdio.h>
#include "write.h"
PI_THREAD (SendData)
{
        piHiPri(20);
        while(1){
                send('8');
                send('e');
                send('A');
        }
}

int main (void)
{
        wiringPiSetup();
        int i = piHiPri(10);
        int x  = piThreadCreate(SendData);
        printf("started p = %d %d\n",i,x);
        while(1)
        {

        }
        return 0;

}

