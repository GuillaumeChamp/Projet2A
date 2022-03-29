#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <wiringPi.h>
#include <softPwm.h>

#include "read.h"

#define LIDAR_I2C_ADDR  0x62
#define LIDAR_FRM_REG   0x11 // Free Running Mode
#define LIDAR_FRM       0xFF

float xacc; // Float 32 : 4 bytes long single precision floating point
float yacc;
float zacc;
float xgyr;
float ygyr;
float zgyr;
double i2cResult = 0;
double uartI2Csum = 0;

int UART_received = 0;

PI_THREAD (ReadUart){
        piHiPri(15);
        
        // initialisation of the rpi port
        int fd;
        char* port = "/dev/serial0";
        if ((fd = serialOpen(port, 115200)) < 0) {
                printf("Problem opening UART port\n"); 
                exit(EXIT_FAILURE);
        }
                
        int buffLen = 50;
        uint8_t rdBuffer[buffLen]; // will receive the values read from the MT
        
        while(1){ // read MTData2 
                memset(&rdBuffer[0], 0, buffLen*sizeof(uint8_t));
                
                // receiving data
                if (serialDataAvail(fd) == -1) {
                        printf("Problem reading of data available\n");
                }
                while (serialDataAvail(fd) == 0) {
                        //printf("No data available\n");        
                }
                int rdCount = 0;
                int message_len = 35; // see doc for details
                while (rdCount < message_len) {
                        if ((serialDataAvail(fd) > 0) {
                                rdBuffer[rdCount] = serialGetchar(fd);
                                if ((rdCount==0)&&(rdBuffer[rdCount]==0xFA)) {rdCount--;} // so that every buffer starts with 0xFA
                                printf("%x ", rdBuffer[rdCount]);
                                rdCount++;
                        }
                }
                printf("\n");
                
                if (rdBuffer[2] == 0x36) { // MTData
                        uint8_t tempxacc[4];
                        uint8_t tempyacc[4];
                        uint8_t tempzacc[4];
                        uint8_t tempxgyr[4];
                        uint8_t tempygyr[4];
                        uint8_t tempzgyr[4];
                        for (int i=0;i<4;i++) { // see doc for details for the values
                                tempxacc[i]=rdBuffer[7+i];
                                tempyacc[i]=rdBuffer[11+i];
                                tempzacc[i]=rdBuffer[15+i];
                                tempxgyr[i]=rdBuffer[22+i];
                                tempygyr[i]=rdBuffer[26+i];
                                tempzgyr[i]=rdBuffer[30+i];
                        }
                        // conversion to the actual values
                        xacc = bytestof(tempxacc);
                        yacc = bytestof(tempyacc);
                        zacc = bytestof(tempzacc);
                        xgyr = bytestof(tempxgyr);
                        ygyr = bytestof(tempygyr);
                        zgyr = bytestof(tempzgyr);
                }
                printf(" xacc : %f\n yacc : %f\n zacc : %f\n",xacc,yacc,zacc);
                printf(" xgyr : %f\n ygyr : %f\n zgyr : %f\n",xgyr,ygyr,zgyr);
        }
        free(rdBuffer);
        serialClose(fd);
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
