all : main

#binary files
prog : read.o main.o
  gcc -Wall -o prog main.o read.o -lwiringPi -lpthread -lm

#.o files
read.o : read.c 
  gcc -c -Wall read.c -lwiringPi

main.o : main.c 
  gcc -c -Wall main.c -lwiringPi -lpthread -lm

#clean
clean :
  rm -f testRead testWrite PWM_test *.o

cleanBinary :
  rm -f testRead testWrite PWM_test
