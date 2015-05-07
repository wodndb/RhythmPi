#include <stdio.h>
#include <wiringPi.h>

#define LED1	4	// BCM_GPIO 23
#define VCC	5

#define SW	1	// BCM_GPIO 18

int main(void) {
	if (wiringPiSetup() == -1) {
		return 1;
	}

	pinMode(SW, INPUT);
	pinMode(LED1, OUTPUT);
	pinMode(VCC, OUTPUT);
	
	digitalWrite(VCC, 1);

	while(1) {
		printf("%d", digitalRead(SW));
		if(digitalRead(SW) == 1) {
			digitalWrite(LED1, 1);
		}
		else if(digitalRead(SW) == 0) {
			digitalWrite(LED1, 0);
		}
	}

	return 0;
}	
