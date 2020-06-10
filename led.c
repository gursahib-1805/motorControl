//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LED.C
// FILE VERSION : 1.0
// PROGRAMMER   : Gursahib Singh
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2019-NOV-01, Gursahib Singh
//   - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Controls the system LED.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "led.h"
#include "hwdefs.h"
#include "global.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
extern uint8_t g_uiCONTROLFLAGS;
//----------------------------------------------------------------------------
// FUNCTION : LED_Init( void )
// PURPOSE  : Initializes the system LED interface
//----------------------------------------------------------------------------

void LED_Init( void )
{
    //PORTF INITIALIZATION
    HWREG( SYSCTL_RCGCGPIO ) |= 0x20;                                // GPIO Port F Run Mode Clock Gating Control
    HWREG( GPIO_PORTF_BASE + GPIO_O_DEN ) |= ( 0x02 | 0x04 | 0x08 ); // Configure PF1 as a digital signal
    HWREG( GPIO_PORTF_BASE + GPIO_O_DIR ) |= ( 0x02 | 0x04 | 0x08 ); // Configure PF1 as an output

    //PORTA INITIALIZATION
    HWREG( SYSCTL_RCGCGPIO ) |= 0x01;                                // GPIO Port F Run Mode Clock Gating Control
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN ) |= 0x04;                   // Configure PF1 as a digital signal
    HWREG( GPIO_PORTA_BASE + GPIO_O_DIR ) |= 0x04;                   // Configure PF1 as an output

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LED_Control( bool bState )
// PURPOSE  : Controls the system LED
//----------------------------------------------------------------------------

void LED_Control( bool bState )
{
    if( bState )
    {
        // Turn on LED
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x02 << 2 ) ) = 0x02;
    }
    else
    {
        // Turn off LED
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x02 << 2 ) ) = 0;
    }

    return;
}

void LED_2_Control( void )
{
    static uint32_t LED_2_Counter = 0;
    if(g_uiCONTROLFLAGS & (1 << CONTROLFLAGS_LED_2))                   //LED2 Flag
    {
        LED_2_Counter = 0;                                             //Reset LED Counter
        g_uiCONTROLFLAGS &= ~(1 << CONTROLFLAGS_LED_2);                //Reset LED2 flag
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = 0x04; //Turn LED2 ON
    }

    if(++LED_2_Counter >= 500)
    {
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x04 << 2 ) ) = 0;     //Turn LED OFF
        g_uiCONTROLFLAGS &= ~(1 << CONTROLFLAGS_LED_2);                 //Reset LED2 Flag
        LED_2_Counter = 0;                                              //Reset lED Counter
    }
    return;

}

#ifdef FREQ_16MHz
void us_1_Tick( void )
{
    HWREG(GPIO_PORTA_BASE + GPIO_O_DATA + (0x04 << 2)) = 0x04;          //Turn LED ON
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    HWREG(GPIO_PORTA_BASE + GPIO_O_DATA + (0x04 << 2)) = 0x00;          //Turn LED OFF
}
#endif

#ifdef FREQ_80MHz
//1 us tick for 80 MHz
void us_1_Tick( void )
{
    HWREG(GPIO_PORTA_BASE + GPIO_O_DATA + (0x04 << 2)) = 0x04;          //Turn LED ON
    //uint32_t i = 0;
    //for(i=0; i<5; i++)
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");
    asm(" nop");asm(" nop");asm(" nop");asm(" nop");asm(" nop");

    HWREG(GPIO_PORTA_BASE + GPIO_O_DATA + (0x04 << 2)) = 0x00;          //Turn LED OFF
}
#endif

//----------------------------------------------------------------------------
// END LED.C
//----------------------------------------------------------------------------
