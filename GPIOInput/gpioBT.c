#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

/////                                                                 /////
//     +--------------------------------------------------------------+  //
//     |      DESCRIPTION ABOUT MAPPING OF WIRING PI PIN NUMBER       |
//     +--------+--------+-------------------+-------+-------+--------+
//     |        | WIRING |                   |       |       |        |
//     |  NAME  | PI PIN |        DESC       |   1   |   0   |  MODE  |
//     +--------+--------+-------------------+-------+-------+--------+
#define STT_BTN     28   //   Start Button   | INPUT |  NONE |  INPUT |
#define STT_LED     29   // Start Button LED |   ON  |  OFF  | OUTPUT |
                         //------------------+-------+-------+--------+
                         //            BT BUTTON MACRO SET            |
                         //------------------+-------+-------+--------+
#define BT1_BTN	     1   //    BT1 Button    | INPUT |  NONE |  INPUT |
#define BT1_LED	    11   //  BT1 Button LED  |   ON  |  OFF  | OUTPUT |
#define BT2_BTN      2   //    BT2 Button    | INPUT |  NONE |  INPUT |
#define BT2_LED     12   //  BT2 Button LED  |   ON  |  OFF  | OUTPUT |
#define BT3_BTN      3   //    BT3 Button    | INPUT |  NONE |  INPUT |
#define BT3_LED     13   //  BT3 Button LED  |   ON  |  OFF  | OUTPUT |
#define BT4_BTN      4   //    BT4 Button    | INPUT |  NONE |  INPUT |
#define BT4_LED     14   //  BT4 Button LED  |   ON  |  OFF  | OUTPUT |
                         //------------------+-------+-------+--------+
                         //            FX BUTTON MACRO SET            |
                         //------------------+-------+-------+--------+
#define FX1_BTN      5   //    FX1 Button    | INPUT |  NONE |  INPUT |
#define FX1_LED     15   //  FX1 Button LED  |   ON  |  OFF  | OUTPUT |
#define FX2_BTN      6   //    FX2 Button    | INPUT |  NONE |  INPUT |
#define FX2_LED     16   //  FX2 Button LED  |   ON  |  OFF  | OUTPUT |
                         //------------------+-------+-------+--------+
                         //        NB ROTARY ENCODER MACRO SET        |
                         //------------------+-------+-------+--------+
#define NB1_PIN_A    7   //     NB1 A PIN    |   ON  |  OFF  |  INPUT |
#define NB1_PIN_B    8   //     NB1 B PIN    |   ON  |  OFF  |  INPUT |
#define NB2_PIN_A    9   //     NB2 A PIN    |   ON  |  OFF  |  INPUT |
#define NB2_PIN_B   10   //     NB2 B PIN    |   ON  |  OFF  |  INPUT |
//     +----------+------+-------------------+-------+-------+--------+  //
/////                                                                 /////

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

// This function returns input state of GPIO
int inputGPIOStat(void) {
	int gpio_input_stat = 0x00;
	int pin;
	
	for( pin = 0; pin < 11; pin++ ) {
		gpio_input_stat |= digitalRead( readPinArray[pin] ) << pin;
	}

	return gpio_input_stat;
}

void setOutputGPIO(int gpio_input_stat) {
	int pin;

	for( pin = 4; pin < 11; pin++ ) {
		digitalWrite( writePinArray[pin], (gpio_input_stat & (0x01 << pin)) >> pin );
	}
}

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

void printGPIOStat(int prev_gpio_stat, int cur_gpio_stat) {
	int pin;
	int fx1 = checkRotDirection(prev_gpio_stat, cur_gpio_stat, 1);
	int fx2 = checkRotDirection(prev_gpio_stat, cur_gpio_stat, 2);

	for (pin = 10; pin >= 0; pin--) {
		printf("| %s ", (gpio_input_stat & (0x01 << pin)) >> pin == 0x01 ? "##" : "  ");
		
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

int main(void) {
	int gpioStat;
	int prevGpioStat = inputPIOStat();
	initPinMode();
	while( 1 ) {
		gpioStat = inputGPIOStat();
		setOutputGPIO(gpioStat);
		printGPIOStat(prevGpioStat, gpioStat);
		prevGpioStat = gpioStat;
	}

	return 0;
}	