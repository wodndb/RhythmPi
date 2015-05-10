#ifndef RP_GPIO_H
#define RP_GPIO_H

/////
//     +--------------------------------------------------------------+
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
//     +----------+------+-------------------+-------+-------+--------+

#define ROT_RIGHT 0x100
#define ROT_LEFT  0x200

// Initialize wiringPi pin mode
void initPinMode(void);

// GPIO stat for input signal
int inputGPIOStat(void);

// Setting GPIO stat for output signal
void setOutputGPIO(int gpio_input_stat);

// Print GPIO input stat
void printGPIOStat(int prev_gpio_stat, int cur_gpio_stat);

// Check roatation of rotary encoder
int checkRotDirection(int prev_gpio_stat, int cur_gpio_stat, int fx_num);

#ifndef