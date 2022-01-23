all : testRead testWrite PWM_test

#binary files
testRead : I2CRead_test.o UARTI2Cfun.o
  gcc -o testRead I2CRead_test.o UARTI2Cfun.o -lwiringPi

testWrite : testThread.o UARTI2Cfun.o
  gcc -o testWrite testThread.o UARTI2Cfun.o -lwiringPi

PWM_test : PWM_test.c
  gcc -o PWM_test PWM_test.c -lwiringPi -lpthread

#.o files
I2CRead_test.o : I2CRead_test.c 
  gcc -c -Wall I2CRead_test.c -lwiringPi

testThread.o : testThread.c 
  gcc -c -Wall testThread.c -lwiringPi

UARTI2Cfun.o : UARTI2Cfun.c 
  gcc -c -Wall UARTI2Cfun.c -lwiringPi

#clean
clean :
  rm -f testRead testWrite PWM_test *.o

cleanBinary :
  rm -f testRead testWrite PWM_test
