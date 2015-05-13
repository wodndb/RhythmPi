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
/// \ file :  rpGpio.h
/// \ brief : Librarys for handling GPIO signal about game controller
//

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

//
/// \brief Initialize wiringPi pin mode
//
/// \param void                 this fuction has no parameter;
//
void initPinMode(void);

//
/// \brief GPIO stat for input signal
//
/// \param void                 this fuction has no parameter;
/// \return                     GPIO stat for input signal
int inputGPIOStat(void);

//
/// \brief Setting GPIO stat for output signal
//
/// \param gpio_input_stat      gpio status about input signal (read)
//
void setOutputGPIO(int gpio_input_stat);

//
/// \brief Print GPIO input stat (for debugging)
//
/// \param prev_gpio_stat       Previous gpio status about input signal (read)
/// \param cur_gpio_stat   		Current  gpio status about input signal (read)
///
///                             * This function need two of param for check
///                             rotary encoder
//
void printGPIOStat(int prev_gpio_stat, int cur_gpio_stat);

//
/// \brief Check roatation of rotary encoder
//
/// \param prev_gpio_stat       Previous gpio status about input signal (read)
/// \param cur_gpio_stat   		Current  gpio status about input signal (read)
/// \param fx_num               fx button type
/// \return                     Rotate direction of rotary encoder
///                             -1        : error or stoped
///                             ROT_RIGHT : direction is right (ClockWise)
///                             ROT_LEFT  : direction is left (ConterClockWise)
int checkRotDirection(int prev_gpio_stat, int cur_gpio_stat, int fx_num);

#ifndef //RP_GPIO_H