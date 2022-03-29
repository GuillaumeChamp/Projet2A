#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	
	if (argc!=3) {
		printf("Syntax error : ./update variable value\n");
		printf("Variables = PWM0-4\n");
	}
	
	int varID = -1;
	if (strcmp(argv[1],"PWM0")==0) {memID = 10;}
	else if (strcmp(argv[1],"PWM1")==0) {memID = 11;}
	else if (strcmp(argv[1],"PWM2")==0) {memID = 12;}
	else if (strcmp(argv[1],"PWM3")==0) {memID = 13;}
	else if (strcmp(argv[1],"PWM4")==0) {memID = 14;}
	else {
		printf("Unknown variable, try PWM0-4\n");
		exit(EXIT_FAILURE);
	}
	
	int value = 0;
	int shmID = shmget(varID,sizeof(int),IPC_CREAT|0666);
	int *mem = shmat(shmID,NULL,0);
	char input[3];
	strcpy(input,argv[2]);
	for (int i=0;i<3;i++) {
		if (input[i]>47) {
			value = value*10 + (int)input[i] - 48; // based on ASCII codes
		}
	}
	*mem = value;
	printf("New value of %s is %d\n",argv[1],value);
	exit(EXIT_SUCCESS);
	
}
