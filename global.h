//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : GLOBAL.H
// FILE VERSION : 1.0
// PROGRAMMER   : Gursahib Singh
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2019-NOV-01, Gursahib Singh
//   - Initial release
//
#include <stdint.h>
#include <stdbool.h>
//----------------------------------------------------------------------------
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef GLOBAL_H_
#define GLOBAL_H_

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

// SYSTEM FLAGS
//___________________1___________0_
//|7|6|5|4|3|2|ADC SS0|SYSTEM TICK|
#define SYSFLAGS_SYS_TICK           0
#define SYSFLAGS_ADC_SS0            1
#define SYSFLAGS_MOTOR_MODE         3
#define SYSFLAGS_I2C_READY          7
#define SYSFLAGS_READ_PCF           4

#define SUNDAY                  0x01
#define MONDAY                  0x02
#define TUESDAY                 0x03
#define WEDNESDAY               0x04
#define THURSDAY                0x05
#define FRIDAY                  0x06
#define SATURDAY                0x07

#define AM                      0
#define PM                      1


//TIME AND DATE STRUCTURE
typedef struct{
    uint8_t SECONDS;
    uint8_t MINUTES;
    uint8_t HOUR;
    uint8_t WEEKDAY;
    uint8_t DATE;
    uint8_t MONTH;
    uint8_t YEAR_LOW;
    uint8_t YEAR_HIGH;
    _Bool AMPM;
}TIMEDATE;

TIMEDATE TIMEandDATE;

//CONTROLFLAGS
//__________________6_____________________________5___________________4__________________3________2________1_____0_
//|7|MOTOR_STEP_UP_20|CHECK STEP UP MOTOR RPM BY 20|SHIFT DISPLAY RIGHT|SHIFT DISPLAY LEFT|BUTTON 3|BUTTON 2|LED 2|
#define CONTROLFLAGS_LED_2      0
#define CONTROLFLAGS_BUTTON_2   1
#define CONTROLFLAGS_BUTTON_3   2
#define SHIFT_DISPLAY_RIGHT     3
#define SHIFT_DISPLAY_LEFT      4
#define MOTOR_CHECK_STEP_UP_20  5
#define MOTOR_STEP_UP_20        6

#define SET(REG, N)             ((REG) |= ((1) << (N)))
#define CLEAR(REG, N)           (((REG) &= ~((1) << (N))))
#define CHECK_BIT(REG, N)       ((((REG) >> (N)) & (1)))
#define NUM_ELEMENTS( a )       ( sizeof( a ) / sizeof( a[ 0 ] ) )
#define DEGCtoDEGF(x)           ((((x)*(9))/(5))+(32))
#define DEGCtoDEGK(x)           ((x)+(273.15))
#define VREFP                   (3.3f)
#define VREFN                   (0.0f)
#define RPM_FACTOR              (1.18f)

#define PB4_PRESSED             (4)
#define PB5_PRESSED             (5)
#define PB6_PRESSED             (6)
#define NO_BUTTON_PRESSED       (0)

#define LED4                    (7)
#define LED5                    (6)
#define LED6                    (5)

#define SOFTWARE_RESET          (7)
#define POWER_ON_RESET          (4)
#define EXTERNAL_RESET          (3)
#define WATCHDOG_TIMER_RESET    (5)

#define MAX_RPM                 (133.0f)
#define MIN_RPM                 (0.0f)

#define PORTB_PIN4              (1 << 4)

#define CONTRAST_PRCNT          (17)

#define FREQ_80MHz
//#define FREQ_16MHz

//----------------------------------------------------------------------------
// DISPLAY BUFFERS
//----------------------------------------------------------------------------


#endif // GLOBAL_H_

//----------------------------------------------------------------------------
// END GLOBAL.H
//----------------------------------------------------------------------------
