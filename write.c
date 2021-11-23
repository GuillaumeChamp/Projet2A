#include <wiringSerial.h>
#include <wiringPiI2C.h>

int send(char c){
        int filedescriptor;
        char* port = "/dev/serial0";
        if((filedescriptor = serialOpen(port,600)) < 0) return 1;
        serialPutchar(filedescriptor,c);
        serialClose(filedescriptor);
        return 0;
}
int I2Csend(int c){
        int filedescriptor = wiringPiI2CSetup(0x20);
        wiringPiI2CWrite(filedescriptor,c);
        return 0;
}
char read(){
        int fd;
        char* port = "/dev/serial0";
        if ((fd = serialOpen(port,600))<0) return 1;
        char ans = serialGetchar(fd);
        serialClose(fd);
        return ans;
}
