#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>

int read() {
        int fd;
        char ch;
        if (wiringPiSetup()<0)return 1;
        if ((fd = serialOpen("/dev/serial0",110))<0)return 1;
//      printf("start...");
        while(1){
//              if (wiringPiSetup()<0)return 1;
//              if ((fd = serialOpen("/dev/serial0",110))<0)return 1;
                ch = serialGetchar(fd);
                if (ch |= 0) printf("%c",ch);
//              printf("while loop\n");
        }
        serialClose(fd);
        return 0;
}


