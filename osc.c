//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : OSC.C
// FILE VERSION : 1.0
// PROGRAMMER   : Gursahib Singh
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, YYYY-MM-DD, Programmer Name
//   - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Configures the main oscillator.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"
#include "hwdefs.h"
#include "osc.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : OSC_Init( void )
// PURPOSE  : Initialize the main oscillator
//----------------------------------------------------------------------------

void OSC_Init( void )
{
    uint32_t uiRCC, uiRCC2;

    uiRCC2 = HWREG( SYSCTL_RCC2 );      // Read current value of the RCC2 register // Read the register value
    uiRCC2 |=  ( uint32_t )( 1 << 31 ); // Set bit 31 (USERCC2)....................// RCC2 register is used
    uiRCC2 &= ~( 1 << 13 );             // Clear bit 13 (PWRDN2)...................// PLL operates normal, not in power-down
    uiRCC2 &= ~( 0x07 << 4 );           // Clear bits 6:4 (OSCSRC2)                // Select MOSC (Main Oscillator) as the Clock Source
    HWREG( SYSCTL_RCC2 ) = uiRCC2;      // Update the RCC2 register                // Load the values back

    uiRCC = HWREG( SYSCTL_RCC );        // Read current value of the RCC register  // Read the register values
    uiRCC &= ~( 1 << 13 );              // Clear bit 13 (PWRDN)....................// PLL operates normally, not in power-down
    uiRCC &= ~( 0x1F << 6 );            // Clear bits 10:6 (XTAL)..................// Clear bits 6 - 10
    uiRCC |=  ( 0x15 << 6 );            // Configure for a 16 MHz crystal..........// Enter 0x15 for 16 MHz crystal
    uiRCC &= ~( 0x03 << 4 );            // Clear bits 5:4 (OSCSRC).................// Select MOSC (Main Oscillator) as the Clock Source
    HWREG( SYSCTL_RCC ) = uiRCC;        // Update the RCC register.................// Load the values back

    while( !( HWREG( SYSCTL_PLLSTAT ) & 1 ) ); //..................................// Wait until the PLL locks

    uiRCC &= ~( 0x0F << 23 );           // Clear bits 26:23 (SYSDIV)
    uiRCC |=  ( 2 << 23 );              // Update SYSDIV
    uiRCC |=  ( 1 << 22 );              // Set bit 22 (USESYSDIV)
    uiRCC &= ~( 1 << 11 );              // Clear bit 11 (BYPASS)
    uiRCC &= ~( 1 << 0 );               // Clear bit 0 (MOSCDIS)
    HWREG( SYSCTL_RCC ) = uiRCC;        // Update the RCC register

    uiRCC2 |=  ( 1 << 30 );             // Set bit 30 (DIV400)
    uiRCC2 &= ~( 0x7F << 22 );          // Clear bits 28:22 (SYSDIV2:SYSDIV2LSB)
    uiRCC2 |=  ( 4 << 22 );             // Update SYSDIV2
    uiRCC2 &= ~( 1 << 11 );             // Clear bit 11 (BYPASS2)
    HWREG( SYSCTL_RCC2 ) = uiRCC2;      // Update the RCC2 register

    return;
}

//----------------------------------------------------------------------------
// END OSC.C
//----------------------------------------------------------------------------
