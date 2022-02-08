#include <wiringPi.h>
#include <stdio.h>
#include "UARTI2Cfun.h"

PI_THREAD (ReadUart){
        piHiPri(10);
        while(1){
                double data=0;
                int dot = 0;
                int nb = 0;
                char read;
                while ((read=readUART()) !='\n'){
                        if (read == '.') dot = nb;
                        else if (read>47) {
                                nb++;
                                data = data*10 + read;
                        }
                }
                data = data/pow(10,nb+1-dot);
                printf("donnée : %f\n",data);
                //sleep(1);
        }
}

PI_THREAD (PwmManager)
{
        piHiPri(15);
        softPwmCreate(1,50,100);
        while(1){
                int alpha = 0;
                int alpha_file = open("alpha.txt",O_RDWR);
                char *buffer = calloc(3,sizeof(char));
                read(alpha_file,buffer,3);
                for (int i = 0;i<3;i++){
                        if (buffer[i] < 48) break;
                        alpha = alpha*10 + (int) buffer[i]-48;
                }
                softPwmWrite(1,alpha);
                close(alpha_file);
                sleep(2);
        }
}


int main (void)
{
        wiringPiSetup();
        piHiPri(10);
        //int x = piThreadCreate(SendData);
        int y = piThreadCreate(ReadUart);
        //int pwm_thread = piThreadCreate(PwmManager);
        printf("started p = %d\n",y);
        while(1)
        {

        }
        return 0;
}
