all : prog

#binary files
prog : read.o main.o
  gcc -Wall -o prog main.o read.o -lwiringPi -lpthread -lm
  
update : update.o
  gcc -Wall -o update update.o
  
readmem : readmem.o
  gcc -Wall -o readmem readmem.o

#.o files
read.o : read.c 
  gcc -c -Wall read.c -lwiringPi

main.o : main.c 
  gcc -c -Wall main.c -lwiringPi -lpthread -lm
  
update.o : update.c
  gcc -c -Wall update.c
  
readmem.o : readmem.c
  gcc -c -Wall readmem.c

#clean
clean :
  rm -f testRead testWrite PWM_test *.o
