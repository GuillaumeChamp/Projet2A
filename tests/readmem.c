#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//This function is used to check the value of a variable in the shared memory

int main(int argc,char** argv){
        int memid=-1;
        if (strcmp(argv[1],"Ki")==0){
                memid = 1;
        }
        else if(strcmp(argv[1],"Ti")==0) memid = 2;
        else if(strcmp(argv[1],"Td")==0) memid = 3;
        else if(strcmp(argv[1],"PWM")==0) memid = 10;
        else exit(EXIT_FAILURE);
        int shmid = shmget(memid,sizeof(int),0);
        int *mem = shmat(shmid,NULL,0);
        printf("read at shmid 1 : %d\n",*mem);
        exit(EXIT_SUCCESS);
}
