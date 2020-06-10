//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : BUTTON.C
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
// Checks the status of the buttons.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "global.h"
#include "hwdefs.h"
#include "display.h"
#include "contact.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
extern uint8_t g_uiCONTROLFLAGS;

CONTACT g_PD0;
CONTACT g_PD1;



//----------------------------------------------------------------------------
// FUNCTION : Buttons_Init( void )
// PURPOSE  : Initializes the pins to which buttons are connected
//----------------------------------------------------------------------------

void Buttons_Init( void )
{
    //PORTD INITIALIZATION
    HWREG( SYSCTL_RCGCGPIO ) |= 0x08;                               // GPIO Port D Run Mode Clock Gating Control
    HWREG( GPIO_PORTD_BASE + GPIO_O_DEN ) |= ( 0x01 | 0x02 );       // Configure PD0, PD1 as a digital signals
    HWREG( GPIO_PORTD_BASE + GPIO_O_DIR ) &= ~( 0x01 | 0x02 );      // Configure PD0, PD1 as an inputs
    CONTACT_Init(&g_PD0, 50);
    CONTACT_Init(&g_PD1, 50);
}

//----------------------------------------------------------------------------
// FUNCTION : Button_2_Check( void )
// PURPOSE  : Checks the status of the buttons connected to PD0 and PD1
//----------------------------------------------------------------------------
void Monitor_Buttons()
{
    //----------------------------------------------------------------------------
    // Check Button (PB2), Connected To PD0
    //----------------------------------------------------------------------------
    if(CONTACT_Sample(&g_PD0, HWREG(GPIO_PORTD_BASE + GPIO_O_DATA + (0x01 << 2))))
    {
        if(g_PD0.bEventState == 0)
        {
            SET(g_uiCONTROLFLAGS, CONTROLFLAGS_BUTTON_2); //Button 2 Pressed
            Update_Display (0);
        }
    }

    else
        CLEAR(g_uiCONTROLFLAGS, CONTROLFLAGS_BUTTON_2);  //Button 2 not pressed

    //----------------------------------------------------------------------------
    // Check Button (PB3), Connected To PD1
    //----------------------------------------------------------------------------
    if(CONTACT_Sample(&g_PD1, HWREG(GPIO_PORTD_BASE + GPIO_O_DATA + (0x02 << 2))))
    {
        if(g_PD1.bEventState == 0)
        {
            SET(g_uiCONTROLFLAGS, CONTROLFLAGS_BUTTON_3);   //Button 3 Pressed
            Update_Display (0);
        }
    }

    else
        CLEAR(g_uiCONTROLFLAGS, CONTROLFLAGS_BUTTON_3);  //Button 3 not pressed
}

//----------------------------------------------------------------------------
// END RESET.C
//----------------------------------------------------------------------------
