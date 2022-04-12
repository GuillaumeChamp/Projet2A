#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//This methode is used to change the value of a variable
//Can be used to test motor by using the PWM

int main(int argc,char** argv){
        if(argc!=3){
                printf("syntaxe error : update var value\n");
                printf("var : Ki,Ti,Td\n");
                exit(EXIT_FAILURE);
        }
        int varid=-1;
        if (strcmp(argv[1],"Ki")==0){
                varid = 1;
        }
        else if (strcmp(argv[1],"Ti")==0) varid = 2;
        else if (strcmp(argv[1],"Td")==0) varid = 3;
        else if (strcmp(argv[1],"PWM")==0) varid = 10;
        else{
                printf("variable name unknow try Ki Ti PWM or Td");
                exit(EXIT_FAILURE);
        }
        float value=0;
        int shmid = shmget(varid,sizeof(float),IPC_CREAT|0666);
        float *mem;
        char input[3];
        mem = shmat(shmid,NULL,0);
        strcpy(input,argv[2]);
        int vir = 0;
        int number=0;
        for(int i=0;i<3;i++){
                if(input[i]>47){
                        value = value*10+(int)input[i]-48;
                        number++;
                }
                else if (input[i]==44||input[i]==46) vir = i;
        };
        if (vir!=0) value = value/ pow(10,number-1);
        printf("new value of %s is %f\n",argv[1],value);
        *mem = value;
        exit(EXIT_SUCCESS);
}
