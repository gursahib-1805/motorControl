//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : UART.C
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
// Configures and Reloads the Watchdog Timer 0
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "hwdefs.h"

bool Watchdog_Timer_Init( uint32_t *pLoad_Value )
{
    HWREG( SYSCTL_RCGCWD ) |= (1 << 0);                             // Set bit 0 to enable clock for WDT0
    asm(" nop");
    HWREG( WDT0_BASE + WDT_O_WDTLOAD ) = *pLoad_Value;              // Load WDTLOAD with load value
    asm(" nop");
    HWREG( WDT0_BASE + WDT_O_WDTCTL ) |= ( ( 1 << 1) | (1 << 0) );  // Set bit RESEN to reset system of second count-down to 0
    asm(" nop");
    HWREG( NVIC_EN0 ) = ( 1 << 18);                                 //Enable WDT0 interrupt in NVIC
    HWREG( WDT0_BASE + WDT_O_WDTLOCK ) = ( uint32_t )1805;          // Write random value in WDTLOCK to lock registers
    //Wait for at least 2 clock cycles to complete
    uint8_t i;
    for( i = 0; i < 5; i++)
        asm(" nop");

    // Check if locked successfully
    // Return 'true' if locked successfully
    // Return 'false' if locking was unsuccessful
    return HWREG( WDT0_BASE + WDT_O_WDTLOCK );
}

bool Watchdog_Timer_Reload( uint32_t *pLoad_Value )
{
    HWREG( WDT0_BASE + WDT_O_WDTLOCK ) = ( uint32_t )0x1ACCE551;    // Unlock WDT registers
    HWREG( WDT0_BASE + WDT_O_WDTLOAD ) = *pLoad_Value;              // Write the reload value
    HWREG( WDT0_BASE + WDT_O_WDTLOCK ) = ( uint32_t )1805;          // Lock again by writing a random value

    //Wait for at least 2 cycles to complete
    asm(" nop");
    asm(" nop");
    asm(" nop");

    // Check if locked successfully
    // Return 'true' if locked successfully
    // Return 'false' if locking was unsuccessful
    return HWREG( WDT0_BASE + WDT_O_WDTLOCK );
}

//----------------------------------------------------------------------------
// END UART.C
//----------------------------------------------------------------------------
