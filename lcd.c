//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : LCD.C
// FILE VERSION : 1.0
// PROGRAMMER   : Gursahib Singh
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2019-NOV-01, Gursahib Singh
// - Initial release
//
//----------------------------------------------------------------------------
// MODULE DESCRIPTION
//----------------------------------------------------------------------------
//
// Provides a low-level bit-banged driver for the Lumex LCM S01602DTR/M
// 16x2 Character LCD Module.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "lcd.h"
#include "hwdefs.h"

//----------------------------------------------------------------------------
// FUNCTION : LCD_WriteNibble( uint8_t uiData )
// PURPOSE  : Writes one nibble (bits 7-4) to the LCD module
//----------------------------------------------------------------------------

void LCD_WriteNibble( uint8_t uiData )
{
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_E << 2 ) ) = LCD_E;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DIR ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( LCD_BUS << 2 ) ) = uiData;

    // A delay is required here...
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );

    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_E << 2 ) ) = ~LCD_E;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DIR ) &= ~LCD_BUS;

    // A delay is required here...
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_ReadNibble( uint8_t *puiData )
// PURPOSE  : Reads and merges one nibble (bits 3-0) from the LCD module
//----------------------------------------------------------------------------

void LCD_ReadNibble( uint8_t *puiData )
{
    uint8_t uiNibble;

    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_E << 2 ) ) = LCD_E;

    // A delay is required here...
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );


    uiNibble = HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( LCD_BUS << 2 ) );
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_E << 2 ) ) = ~LCD_E;

    // A delay is required here...
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );

    // Merge to lower nibble
    *puiData = ( *puiData & 0xF0 ) | ( ( uiNibble >> 4 ) & 0x0F );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_IRWrite( uint8_t uiData )
// PURPOSE  : Writes one byte to the instruction register of the LCD module
//----------------------------------------------------------------------------

void LCD_IRWrite( uint8_t uiData )
{
    LCD_WaitForReady();

    // RS = 0, R/W = 0
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( ( LCD_RS | LCD_RW ) << 2 ) ) = 0;
    LCD_WriteNibble( uiData );
    LCD_WriteNibble( uiData << 4 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_IRRead( uint8_t *puiData )
// PURPOSE  : Reads one byte from the instruction register of the LCD module
//----------------------------------------------------------------------------

void LCD_IRRead( uint8_t *puiData )
{
    // RS = 0, R/W = 1
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( ( LCD_RS | LCD_RW ) << 2 ) ) = LCD_RW;
    LCD_ReadNibble( puiData );
    *puiData <<= 4;
    LCD_ReadNibble( puiData );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_DRWrite( uint8_t uiData )
// PURPOSE  : Writes one byte to the data register of the LCD module
//----------------------------------------------------------------------------

void LCD_DRWrite( uint8_t uiData )
{
    LCD_WaitForReady();

    // RS = 1, R/W = 0
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( ( LCD_RS | LCD_RW ) << 2 ) ) = LCD_RS;
    LCD_WriteNibble( uiData );
    LCD_WriteNibble( uiData << 4 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_DRRead( uint8_t *puiData )
// PURPOSE  : Reads one byte from the data register of the LCD module
//----------------------------------------------------------------------------

void LCD_DRRead( uint8_t *puiData )
{
    LCD_WaitForReady();

    // RS = 1, R/W = 1
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( ( LCD_RS | LCD_RW ) << 2 ) ) = LCD_RS | LCD_RW;
    LCD_ReadNibble( puiData );
    *puiData <<= 4;
    LCD_ReadNibble( puiData );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_WaitForReady( void )
// PURPOSE  : Waits (indefinitely) until the LCD module is ready
//----------------------------------------------------------------------------

void LCD_WaitForReady( void )
{
    uint8_t uiIR;
    do
    {
      LCD_IRRead( &uiIR );
    }
    while( uiIR & LCD_IC_STATUS_BUSY );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_Init( void )
// PURPOSE  : Initializes the LCD module
//----------------------------------------------------------------------------

void LCD_Init( void )
{
    // GPIO Port A and E Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCGPIO ) |= 0x11;

    // Configure Data Bus (initially input)
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN ) |= LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DIR ) &= ~LCD_BUS;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( LCD_BUS << 2 ) ) = 0;

    // Configure Control Signals
    HWREG( GPIO_PORTE_BASE + GPIO_O_DEN ) |= LCD_CONTROL;
    HWREG( GPIO_PORTE_BASE + GPIO_O_DIR ) |= LCD_CONTROL;
    HWREG( GPIO_PORTA_BASE + GPIO_O_DATA + ( LCD_CONTROL << 2 ) ) = 0;

    // Turn power off to the LCD module
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_PWR << 2 ) ) = 0;

    //Delay prior to power-up
    uint64_t x;
    for(x = 0; x < 1000; x++)
    {
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    }
    // Power-up the LCD module
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( LCD_PWR << 2 ) ) = LCD_PWR;

    LCD_WaitForReady();

    // Set to 4-bit mode
    HWREG( GPIO_PORTE_BASE + GPIO_O_DATA + ( ( LCD_RS | LCD_RW ) << 2 ) ) = 0;

    LCD_WriteNibble( LCD_IC_FUNCTION );

    // Configure LCD Module
    LCD_IRWrite( LCD_IC_FUNCTION | LCD_IC_FUNCTION_2LINE | LCD_IC_FUNCTION_ON );
    LCD_IRWrite( LCD_IC_DISPLAY | LCD_IC_DISPLAY_ON );
    LCD_IRWrite( LCD_IC_CLEAR );
    LCD_IRWrite( LCD_IC_ENTRYMODE | LCD_IC_ENTRYMODE_INC );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : LCD_SendMessage( uint8_t* sMessage )
// PURPOSE  : Send a message using the LCD module
//----------------------------------------------------------------------------

void LCD_SendMessage( uint8_t* sMessage )
{
    // Send entire message at the current cursor position until
    // the NULL character is reached

    while( *sMessage )
    {
        LCD_DRWrite( *sMessage++ );
    }

    return;
}

//----------------------------------------------------------------------------
// END OF LCD.C
//----------------------------------------------------------------------------
