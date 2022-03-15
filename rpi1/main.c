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

long xacc; // Float 32 : 4 bytes long single precision floating point
long yacc;
long zacc; 
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
        char rdBuffer[buffLen]; // will receive the values read from the MT
        char wrBuffer[buffLen]; // will contain the values to be send to the MT

        int configDone = 0;
        while (!configDone) {
                
                memset(rdBuffer, 0, buffLen*sizeof(char));
                memset(wrBuffer, 0, buffLen*sizeof(char));
                
                // receiving data
                if (serialDataAvail(fd) == -1) {
                        printf("Problem reading if data available\n");
                        exit(EXIT_FAILURE);
                } 
                while (serialDataAvail(fd) == 0) {
                        printf("No data available\n");
                }
                int rdCount = 0;
                while (serialDataAvail(fd) > 0) {
                        rdBuffer[rdCount] = serialGetchar(fd);
                        rdCount++;
                }
                
                // interpreting received data and answer in consequence
                if ((rdBuffer[4] == '3')&&(rdBuffer[5] == 'E')) { // WakeUp signal
                        wrBuffer[0] = 'FAFF3000D1'; // GoToConfig signal
                        serialPuts(wrBuffer);
                }
                
                if ((rdBuffer[4] == '3')&&(rdBuffer[5] == '1')) { // GoToConfigAck signal
                        wrBuffer[0] = 'FAFF18010200'; // SetBaudrate signal
                        serialPuts(wrBuffer);
                } 
                
                if ((rdBuffer[4] == '1')&&(rdBuffer[5] == '9')) { // SetBaudrateAck signal
                        wrBuffer[0] = 'FAFFC0024020006401'; // SetOutputConfiguration signal
                        serialPuts(wrBuffer);
                }
                
                if ((rdBuffer[4] == 'C')&&(rdBuffer[5] == '0')) { // SetOutputConfigurationAck signal
                        wrBuffer[0] = 'FAFF100001'; // GoToMeasurement signal
                        serialPuts(wrBuffer);
                }
                
                if ((rdBuffer[4] == '1')&&(rdBuffer[5] == '1')) { // GoToMeasurementAck signal
                        configDone = 1;
                }
        
        }
        
        while(1){ // read MTData2 
                
                memset(rdBuffer, 0, buffLen*sizeof(char));
                
                // receiving data
                while (serialDataAvail(fd) == -1) {
                        printf("No data available\n");
                }
                int rdCount = 0;
                while (serialDataAvail(fd) > -1) {
                        rdBuffer[rdCount] = serialGetchar(fd);
                        rdCount++;
                }
                
                if ((rdBuffer[4] == '3')&&(rdBuffer[5] == '6')) { // MTData
                        char tempxacc[4];
                        char tempyacc[4];
                        char tempzacc[4];
                        char *eptr;
                        strncpy(tempxacc,rdBuffer[16],4*sizeof(char));
                        strncpy(tempyacc,rdBuffer[20],4*sizeof(char));
                        strncpy(tempzacc,rdBuffer[24],4*sizeof(char));
                        xacc = strtol(tempxacc, &eptr, 4);
                        yacc = strtol(tempyacc, &eptr, 4);
                        zacc = strtol(tempzacc, &eptr, 4);
                }
                printf(" xacc : %ld\n yacc : %ld\n zacc : %ld\n",xacc,yacc,zacc);
        }
        
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
