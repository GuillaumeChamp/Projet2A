/* Largely based on softPwm.c from wiringPi library */

#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

#include <wiringPi.h>
#include "softPwm.h"

#define	MAX_PIN		27
#define FREQ_MAX	1000


static volatile int freq			[MAX_PIN];
static volatile pthread_t threads	[MAX_PIN];
static volatile int newPin = -1;


/* Thread to do the actual  output */

static void *ouiThread (void *arg)
{
	int pin;
	struct sched_param param;

	param.sched_priority = sched_get_priority_max (SCHED_RR);
	pthread_setschedparam (pthread_self (), SCHED_RR, &param);

	pin = *((int *)arg);
	free (arg);

	pin = newPin;
	newPin = -1;

	piHiPri (90);

	while(1) {
		int frequency = freq[pin];
		if (frequency != 0 {
			digitalWrite(pin,HIGH);
			delayMicroseconds((unsigned int) 100000/(2*frequency));
			digitalWrite(pin,LOW);
			delayMicroseconds((unsigned int) 100000/(2*frequency));
		}
	}

	return NULL ;
}


/* Write a freq value to the given pin */

void ouiWrite (int pin, int frequency) {

	if (((pin>=2)&(pin<=7))|((pin>=21)&(pin<=23))|(pin==26)) {
		if (frequency<=0) {frequency=1;}
		else {
			if (frequency>FREQ_MAX) {frequency=FREQ_MAX;}
		}
		freq[pin]=frequency;
	}
	
}


/* Create a new oui thread */

int ouiCreate (int pin, int initialFreq) {
	int res ;
	pthread_t my_thread ;
	int *passPin ;

	if (((pin>=2)&(pin<=7))|((pin>=21)&(pin<=23))|(pin==26)) {return -1;}

	if (freq[pin] != 0)	{return -1;}

	passPin = malloc (sizeof (*passPin)) ;
	if (passPin == NULL) {return -1;}

	digitalWrite(pin,LOW);
	pinMode(pin,OUTPUT);

	freq[pin] = initialFreq ;

	*passPin = pin;
	newPin = pin;
	res = pthread_create(&my_thread, NULL, ouiThread, (void *)passPin);

	if (res != 0) {return res;}

	while (newPin != -1) {delay (1);}
	threads[pin] = my_thread ;

	return res;
	}


/* Stop an existing softPWM thread */

void ouiStop (int pin)
{
	if (((pin>=2)&(pin<=7))|((pin>=21)&(pin<=23))|(pin==26)) {
		if (range [pin] != 0) {
			pthread_cancel(threads[pin]);
			pthread_join(threads[pin], NULL);
			freq[pin] = 0;
			digitalWrite(pin,LOW);
		}
	}
}
