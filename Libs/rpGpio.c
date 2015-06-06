///
//  KOREA UNIVERSITY OF TECHNOLOGY AND EDUCATION
//  SCHOOL OF COMPUTER SCIENCE AND ENGINEERING
//  EMBEDDED APPLICATION AND PRACTICE 2015 SPRING TERM PROJECT
//  RHYTHMPI : Rhythm game for raspberry pi
//  url: http://www.koreatech.ac.kr    : Official Univ. home page
//       http://cse.koreatech.ac.kr    : Official Dept. home page
//
// Author : 
//  SCHOOL OF COMPUTER SCIENCE AND ENGINEERING
//  2012136116 JEONG, JAE-U
//  wodndb@koreatech.ac.kr
//

//
/// \ file :  rpGpio.c
/// \ brief : Librarys for handling GPIO signal about game controller
//

///
//  Includes
//
#include "rpGpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define ROT_RIGHT 0x100
#define ROT_LEFT  0x200

// Pin of wiringPi for read
int readPinArray[11] = { NB2_PIN_B, NB2_PIN_A, NB1_PIN_B, NB1_PIN_A,
                         FX2_BTN  , FX1_BTN  ,
                         BT4_BTN  , BT3_BTN  , BT2_BTN  , BT1_BTN  ,
                         STT_BTN};

// Pin of wiringPi for writing
int writePinArray[11] = { 0, 0, 0, 0,
                          FX2_LED, FX1_LED,
                          BT4_LED, BT3_LED, BT2_LED, BT1_LED,
                          STT_LED};

int ROT_DATA[4] = { 0, 1, 3, 2 };

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
// initPinMode()
//
//    Initialize wiringPi pin mode
//
void initPinMode(void) {
	int pin;

	if( wiringPiSetup() == -1) {
		printf("Wiring pi Setting is wrong!\n");
		printf("Exit program (from rpNote.c file \n");
		exit(1);
	}

	//Setting pinMode to INPUT for all of buttons and encoders
	for( pin = 0; pin < 11; pin++ ) {
		pinMode( readPinArray[pin], INPUT );
	}

	// Setting pinMode to OUTPUT for all of LED in buttons
	// NOTE: Index of pinArray 0~3 is encoder, So for-loop number is 4 to 10.
	for( pin = 4; pin < 11; pin++ ) {
		pinMode( writePinArray[pin], OUTPUT );
	}
}

///
// inputGPIOStat()
//
//    GPIO stat for input signal
//
int inputGPIOStat(void) {
	int gpio_input_stat = 0x00;
	int pin;
	
	// Read GPIO INPUT signal for all of buttons and encoders
	for( pin = 0; pin < 11; pin++ ) {
		gpio_input_stat |= digitalRead( readPinArray[pin] ) << pin;
	}

	return gpio_input_stat;
}

///
// setOutputGPIO()
//
//    Setting GPIO stat for output signal
//
void setOutputGPIO(int gpio_input_stat) {
	int pin;

	// Setting OUTPUT GPIO signal for all of LED in buttons
	// NOTE: Index of pinArray 0~3 is encoder, So for-loop number is 4 to 10.
	for( pin = 4; pin < 11; pin++ ) {
		digitalWrite( writePinArray[pin], (gpio_input_stat & (0x01 << pin)) >> pin );
	}
}

///
// checkRotDirection()
//
//    Check roatation of rotary encoder
//
int checkRotDirection(int prev_gpio_stat, int cur_gpio_stat, int fx_num ) {
	int prev_loc; 
	int cur_loc;

	if(fx_num == 1) {
		prev_loc = (prev_gpio_stat & 0x03);
		cur_loc = (cur_gpio_stat & 0x03);
	} 
	else if(fx_num == 2) {
		prev_loc = (prev_gpio_stat & 0x0c) >> 2;
		cur_loc = (cur_gpio_stat & 0x0c) >> 2;
	}
	else {
		printf("Incorrect Error!\n");
		return -1;
	}

	if(prev_loc == cur_loc) {
		// No Changes
	}
	else if(cur_loc == ROT_DATA[(ROT_DATA[prev_loc] + 1) % 4]) {
		return ROT_RIGHT;
	}
	else if(cur_loc == ROT_DATA[(ROT_DATA[prev_loc] + 3) % 4]) {
		return ROT_LEFT;
	}
	else {
		return -1;
	}
}

///
// printGPIOStat()
//
//    Print GPIO input stat (for debugging)
//
void printGPIOStat(int prev_gpio_stat, int cur_gpio_stat) {
	int pin;
	int fx1 = checkRotDirection(prev_gpio_stat, cur_gpio_stat, 1);
	int fx2 = checkRotDirection(prev_gpio_stat, cur_gpio_stat, 2);

	for (pin = 10; pin >= 0; pin--) {
		printf("| %s ", (cur_gpio_stat & (0x01 << pin)) >> pin == 0x01 ? "##" : "  ");
		
		if(fx1 == ROT_LEFT) {
			printf("| ◀◀ ");
		}
		else if(fx1 == ROT_RIGHT) {
			printf("| ▶▶ ");
		}
		else {
			printf("|  ●  ");
		}

		if(fx2 == ROT_LEFT) {
			printf("| ◀◀ ");
		}
		else if(fx2 == ROT_RIGHT) {
			printf("| ▶▶ ");
		}
		else {
			printf("|  ●  ");
		}
	}
	printf("|\n");
}
