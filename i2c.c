//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : I2C.C
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
// Inter-integrated circuit interface.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"
#include "hwdefs.h"
#include "i2c.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

extern uint32_t g_uiSYSFLAGS;

//----------------------------------------------------------------------------
// FUNCTION : I2C_IntHandler( void )
// PURPOSE  : Handles the I2C interrupt
//----------------------------------------------------------------------------

void I2C0_IntHandler( void )
{
    // Check if an I2C interrupt occurred
    if( HWREG( I2C0_BASE + I2C_O_MMIS ) & ( 1 << 0 ) )
    {
        // Acknowledge interrupt (clear)
        HWREG( I2C0_BASE + I2C_O_MICR ) = ( 1 << 0 );

        // Flag an I2C event
        g_uiSYSFLAGS |= ( 1 << SYSFLAGS_I2C_READY );

        // Halt on error
        if( HWREG( I2C0_BASE + I2C_O_MCS ) & ( I2C_MCS_CLKTO | I2C_MCS_ERROR ) )
        {
            while( 1 );
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : I2C_Init( void )
// PURPOSE  : Initializes the inter-integrated circuit interface
//----------------------------------------------------------------------------

void I2C_Init( void )
{
    // GPIO Port B Run Mode Clock Gating Control (Port B)
    HWREG( SYSCTL_RCGCGPIO ) |= 0x02;

    // Configure GPIO Port B (PB3/PB2 = I2C0SDA/I2C0SCL)
    HWREG( GPIO_PORTB_BASE + GPIO_O_AFSEL ) |= 0x0C;
    HWREG( GPIO_PORTB_BASE + GPIO_O_DEN   ) |= 0x0C;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL  ) &= 0xFFFF00FF;
    HWREG( GPIO_PORTB_BASE + GPIO_O_PCTL  ) |= 0x00003300;
    HWREG( GPIO_PORTB_BASE + GPIO_O_ODR   ) |= 0x08;

    // I2C 0 Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCI2C ) |= 0x01;

    // Assign the microcontroller as master
    HWREG( I2C0_BASE + I2C_O_MCR ) = 0x10;

    // Assign the bus frequency to 100 kHz:
    //
    // TPR = ( SYSCLK / ( 2 * ( 6 + 4 ) * SCL     ) ) - 1
    // TPR = ( 16 MHz / ( 2 * ( 6 + 4 ) * 100 kHz ) ) - 1
    // TPR = 7
    //
    HWREG( I2C0_BASE + I2C_O_MTPR ) = 39;

    // I2C0 Master Interrupt Mask
    // Bit 0: IM
    HWREG( I2C0_BASE + I2C_O_MIMR ) = ( 1 << 0 );

    // Enable Interrupts for I2C0
    //HWREG( NVIC_EN0 ) |= ( 1 << 8 );
    HWREG( NVIC_DIS0 ) = ( 1 << 8 );

    return;
}

//----------------------------------------------------------------------------
// END I2C.C
//----------------------------------------------------------------------------
