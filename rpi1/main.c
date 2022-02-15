#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <wiringPi.h>
#include <softPwm.h>

#include "read.h"

#define LIDAR_I2C_ADDR  0x62
#define LIDAR_FRM_REG   0x11 // Free Running Mode
#define LIDAR_FRM       0xFF

extern int errno;

double uartResult = 0;
double i2cResult = 0;
double uartI2Csum = 0;

int UART_received = 0;

PI_THREAD (ReadUart){
        piHiPri(15);
        while(1){
                double data = 0;
                int dot = 0;
                int nb = 0;
                char read;
                while ((read=readUART()) !='\n' && nb < 7){
                        if (read == '.') dot = nb;
                        else if (read>47) {
                                nb++;
                                data = data*10 + read;
                        }
                }
                UART_received = 1;
                // printf("UART_received (uart) : %d\n",UART_received);
                data = data/pow(10,nb+1-dot);
                uartResult = data;
                printf("uartResult : %f\n",uartResult);
                uartI2Csum = uartResult + i2cResult;
                printf("SUM = %f\n", uartI2Csum);
        }
}

PI_THREAD (ReadI2C){
        piHiPri(12);
        int fd = wiringPiI2CSetup(LIDAR_I2C_ADDR);
        if (fd == -1) {
                printf("Setup problem\n");
                exit(EXIT_FAILURE);
        }
  
        int frm_activation = wiringPiI2CWriteReg8(fd,LIDAR_FRM_REG,LIDAR_FRM);
        if (frm_activation == -1) {
                printf("Problem with setting up free running mode\n");
                exit(EXIT_FAILURE);
        }
  
        while(1) {
                printf("UART_received (i2c) : %d\n", UART_received);
                if (UART_received) {
                        i2cResult = readI2C(fd);
                        printf("I2C read = %d\n", i2cResult);
                        UART_received = 0;
                } else {
                        printf("no UART received yet\n");
                }
        }
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
        int uart = piThreadCreate(ReadUart);
        int i2c = piThreadCreate(ReadI2C);
        //int pwm = piThreadCreate(PwmManager);
        printf("started : pUart = %d, pI2C = %d\n",uart,i2c);
        while(1) {}
        exit(EXIT_FAILURE);
}
