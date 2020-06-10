//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : ADC.C
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
// Controls the internal analog to digital converter.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "global.h"
#include "hwdefs.h"
#include "adc.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

extern uint32_t g_uiSYSFLAGS;

//----------------------------------------------------------------------------
// FUNCTION : ADC_SS0_IntHandler( void )
// PURPOSE  : Handles the ADC SS0 interrupt
//----------------------------------------------------------------------------

void ADC_SS0_IntHandler( void )
{
    uint32_t uiIntStatus = HWREG( ADC0_BASE + ADC_O_ISC );

    // Check for SS0 interrupt
    if( uiIntStatus & ( 1 << 0 ) )
    {
        // Clear interrupt
        HWREG( ADC0_BASE + ADC_O_ISC ) = ( 1 << 0 );

        // Flag ADC SS0 conversion complete
        g_uiSYSFLAGS |= ( 1 << SYSFLAGS_ADC_SS0 );

    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : ADC_Init( void )
// PURPOSE  : Initialize the ADC
//----------------------------------------------------------------------------

void ADC_Init( void )
{
    // From table 13-1 (page 801 of the TM4C123GH6PM datasheet)
    // AIN4 is assigned to pin PD3 (pin 64)

    // Enable the run-mode clock for ADC0
    HWREG( SYSCTL_RCGCADC ) |= 0x01;

    // Enable the run-mode clock for GPIO Port D
    HWREG( SYSCTL_RCGCGPIO ) |= 0x08;

    // Configure pin 64 (PD3) to be an analog input
    HWREG( GPIO_PORTD_BASE + GPIO_O_AFSEL ) |= 0x08;
    HWREG( GPIO_PORTD_BASE + GPIO_O_DEN   ) &= ~0x08;

    // Configure pin 64 (PD2) to be an analog input
    HWREG( GPIO_PORTD_BASE + GPIO_O_AFSEL ) |= 0x04;
    HWREG( GPIO_PORTD_BASE + GPIO_O_DEN   ) &= ~0x04;

    // Configure PD3 as analog (AIN4 as per table 23-5 on page 1351 of the TM4C123GH6PM datasheet)
    HWREG( GPIO_PORTD_BASE + GPIO_O_AMSEL ) |= 0x08;

    // Configure PD2 as analog (AIN5 as per table 23-5 on page 1351 of the TM4C123GH6PM datasheet)
    HWREG( GPIO_PORTD_BASE + GPIO_O_AMSEL ) |= 0x04;

    // Disable SS0
    HWREG( ADC0_BASE + ADC_O_ACTSS ) &= ~( 1 << 0 );

    // Ensure trigger for SS0 is the processor
    HWREG( ADC0_BASE + ADC_O_EMUX ) &= 0x0000FFF0;

    // Assign priority for SS0 as highest
    HWREG( ADC0_BASE + ADC_O_SSPRI ) &= 0x00003330;

    // Assign AIN4 as the first (zeroth) sample for SS0
    HWREG( ADC0_BASE + ADC_O_SSMUX0 ) &= 0xFFFFFFF0;
    HWREG( ADC0_BASE + ADC_O_SSMUX0 ) |= 0x00000004;

    // Assign AIN5 as the first (1st) sample for SS0
    HWREG( ADC0_BASE + ADC_O_SSMUX0 ) &= 0xFFFFFF0F;
    HWREG( ADC0_BASE + ADC_O_SSMUX0 ) |= 0x00000050;

    // Configure the first sample for SS0:
    // Bit : Description
    // ---------------------------------------
    //  0  : 0 = Single-ended sample
    //  1  : 0 = Final sample in the sequence
    //  2  : 0 = No interrupt
    //  3  : 0 = Analog input

    // Configure the second sample for SS0:
    // Bit : Description
    // ---------------------------------------
    //  4  : 0 = Single-ended sample
    //  5  : 1 = Final sample in the sequence
    //  6  : 1 = Generate interrupt
    //  7  : 1 = Internal temperature sensor

    HWREG( ADC0_BASE + ADC_O_SSCTL0 ) &= 0xFFFFF000;
    HWREG( ADC0_BASE + ADC_O_SSCTL0 ) |= 0x00000E00;

    // Enable hardware averaging
    HWREG( ADC0_BASE + ADC_O_SAC ) = 6;

    // Enable SS0
    HWREG( ADC0_BASE + ADC_O_ACTSS ) |= ( 1 << 0 );

    // ADC Interrupt Mask
    // Bit 0: SS0 Interrupt Mask
    HWREG( ADC0_BASE + ADC_O_ISC ) |= ( 1 << 0 );
    HWREG( ADC0_BASE + ADC_O_IM  ) |= ( 1 << 0 );

    // Enable Interrupts for ADC0 SS0 (Table 2-9 on page 104 of TM4C123GH6PM datasheet)
    HWREG( NVIC_EN0 ) = ( 1 << 14 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : ADC_SS0_Trigger( void )
// PURPOSE  : Trigger a new set of conversions for SS0
//----------------------------------------------------------------------------

void ADC_SS0_Trigger( void )
{
    HWREG( ADC0_BASE + ADC_O_PSSI ) |= ( 1 << 0 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : ADC_SS0_Read( uint16_t* pValues, uint8_t uiNumValues )
// PURPOSE  : Reads data from the ADC FIFO
//----------------------------------------------------------------------------

uint8_t ADC_SS0_Read( uint16_t* pValues, uint8_t uiNumValues )
{
    uint8_t uiValuesRead = 0;

    while( !( HWREG( ADC0_BASE + ADC_O_SSFSTAT0 ) & ( 1 << 8 ) ) && uiNumValues )
    {
        pValues[ uiValuesRead++ ] = ( uint16_t )( HWREG( ADC0_BASE + ADC_O_SSFIFO0 ) & 0x00000FFF );
        uiNumValues--;
    }

    return uiValuesRead;
}

//----------------------------------------------------------------------------
// END ADC.C
//----------------------------------------------------------------------------
