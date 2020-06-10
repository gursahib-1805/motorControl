//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : RESET.C
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
#include "reset.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : Reset_Cause
// PURPOSE  : Identifies the cause of Reset
//----------------------------------------------------------------------------

uint8_t Reset_Cause( void )
{
    //Read RESC register
    uint32_t rescReg = HWREG(SYSCTL_RESC);

    //Clear the bits
    HWREG(SYSCTL_RESC) &= ~((1 << 0) | (1 << 1) | (1 << 4));

    //Return 3 if bit 0 is set (External Reset)
    if( (rescReg & 0x01) )
        return ( EXTERNAL_RESET );

    //Return 4 if bit 1 is set (Power-On Reset)
    else if( (rescReg & 0x02) )
        return ( POWER_ON_RESET );

    //Return 7 if bit 4 is set (Software Reset)
    else if( (rescReg & 0x10) )
        return ( SOFTWARE_RESET );

    else if( (rescReg & 0x08) )
        return ( WATCHDOG_TIMER_RESET );

    //Set Sequencer Pulses to 0 if something else causes reset
    else
        return (0);
}

//----------------------------------------------------------------------------
// END RESET.C
//----------------------------------------------------------------------------
