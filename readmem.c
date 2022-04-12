#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	
	int memID = -1;
	if (strcmp(argv[1],"PWM0")==0) {memID = 10;}
	else if (strcmp(argv[1],"PWM1")==0) {memID = 11;}
	else if (strcmp(argv[1],"PWM2")==0) {memID = 12;}
	else if (strcmp(argv[1],"PWM3")==0) {memID = 13;}
	else if (strcmp(argv[1],"PWM4")==0) {memID = 14;}
	else {
		printf("Unknown variable, try PWM0-4\n");
		exit(EXIT_FAILURE);
	}
	
	int shmID = shmget(memID,sizeof(int),0);
	int *mem = shmat(shmID,NULL,0);
	printf("Read at shmID %d : %d\n",shmID,*mem);
	exit(EXIT_SUCCESS);
	
}
