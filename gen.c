
#include <wiringSerial.h>

int send(char c){
        int filedescriptor;
        if((filedescriptor = serialOpen("/dev/ttyAMA0",230400)) < 0) return 1;
        serialPutchar(filedescriptor,c);
        return 0;
}
