//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : SYSTICK.C
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

#include "global.h"
#include "hwdefs.h"
#include "systick.h"
#include "led.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

extern uint32_t g_uiSYSFLAGS;

//----------------------------------------------------------------------------
// FUNCTION : SYSTICK_IntHandler( void )
// PURPOSE  : Handles the system tick interrupt
//----------------------------------------------------------------------------

void SYSTICK_IntHandler( void )
{
    // Check if a system tick timeout occurred
    if( HWREG( NVIC_ST_CTRL ) & ( 1 << 16 ) )
    {
        // Flag a system tick event
        us_1_Tick();
        SET(g_uiSYSFLAGS, SYSFLAGS_SYS_TICK);

    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : SYSTICK_Init( void )
// PURPOSE  : Initialize the system tick
//----------------------------------------------------------------------------

void SYSTICK_Init( void )
{
    // Configure STRELOAD value
    HWREG( NVIC_ST_RELOAD ) = ( 4000 - 1 );

    // Clear the STCURRENT register
    HWREG( NVIC_ST_CURRENT ) = 0;

    // Enable the System Timer (SysTick) by configuring STCTRL
    HWREG( NVIC_ST_CTRL ) = 0x03;

    return;
}

//----------------------------------------------------------------------------
// END SYSTICK.C
//----------------------------------------------------------------------------
