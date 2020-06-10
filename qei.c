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
// Reads the RPM of the MOTOR
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include "hwdefs.h"


void QEI_Init( void )
{
    //Configure the Quadrature Encoder Interface peripheral
    HWREG( SYSCTL_SRQEI ) |= 0x00000001; //Reset the QEI peripheral
    HWREG( SYSCTL_SRQEI ) &= ~0x00000001; //Remove Reset

    HWREG( SYSCTL_RCGCQEI ) |= 0x00000001; //Enable Clock for QEI 0
    HWREG( SYSCTL_RCGCGPIO ) |= 0x00000008; //Enable Clock for PORTD

    //PD7 is a special pin (locked by default) - unlock PD7
    HWREG( GPIO_PORTD_BASE + GPIO_O_LOCK ) |= 0x4C4F434B;
    HWREG( GPIO_PORTD_BASE + GPIO_O_CR ) |= 0x80;

    // Configure PD6 and PD7 for QEI 0
    HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= 0xC0;
    HWREG(GPIO_PORTD_BASE + GPIO_O_DIR) &= ~0xC0;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) |= 0xC0;
    HWREG(GPIO_PORTD_BASE + GPIO_O_PCTL) &= 0x00FFFFFF;
    HWREG(GPIO_PORTD_BASE + GPIO_O_PCTL) |= 0x66000000;

    // Configure QEI0 (80 MHz System Clock)
    HWREG(QEI0_BASE + QEI_O_CTL) = 0x00000628;
    HWREG(QEI0_BASE + QEI_O_LOAD) = 80000000 - 1;
    HWREG(QEI0_BASE + QEI_O_CTL) |= 0x00000001;
    return;
}

float QEI_GetSpeed(void)
{
    return (((float)HWREG(QEI0_BASE + QEI_O_SPEED) / (float)(4.0f * 3.0f)));//*2.0f);
}
