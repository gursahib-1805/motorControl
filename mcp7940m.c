//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MCP7940M.C
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
#include "mcp7940m.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

extern uint32_t g_uiSYSFLAGS;

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Init( void )
// PURPOSE  : Initializes the MCP7940M RTC
//----------------------------------------------------------------------------

void MCP7940M_Init( TIMEDATE *pTIMEandDATE )
{
    uint8_t uiData = 0;

    uiData = ( 0b10000000 ) | ( pTIMEandDATE -> SECONDS ); // Seconds
    MCP7940M_Write( REG_SECONDS, &uiData, 1 );
    uiData = 0;

    uiData = pTIMEandDATE -> MINUTES; // Minute
    MCP7940M_Write( REG_MINUTES, &uiData, 1 );
    uiData = 0;

    uiData = TIMEandDATE.AMPM ? ( 0b01100000 ) | ( pTIMEandDATE -> HOUR ) : ( 0b01000000 ) | ( pTIMEandDATE -> HOUR );
    //uiData = ( 0b01000000 ) | ( pTIMEandDATE -> HOUR ); // Hour
    MCP7940M_Write( REG_HOUR, &uiData, 1 );
    uiData = 0;

    uiData = pTIMEandDATE -> WEEKDAY; // Weekday
    MCP7940M_Write( REG_WEEKDAY, &uiData, 1 );
    uiData = 0;

    uiData = pTIMEandDATE -> DATE; // Day
    MCP7940M_Write( REG_DATE, &uiData, 1 );
    uiData = 0;

    uiData = pTIMEandDATE -> MONTH; // Month
    MCP7940M_Write( REG_MONTH, &uiData, 1 );
    uiData = 0;

    uiData = pTIMEandDATE -> YEAR_LOW; // Year
    MCP7940M_Write( REG_YEAR, &uiData, 1 );
    uiData = 0;

    uiData = 0x43; // Control
    MCP7940M_Write( REG_CONTROL, &uiData, 1 );

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Sequencer( uint8_t uiCommand, uint8_t* puiData, uint8_t uiNumBytes )
// PURPOSE  : Sequences access to the MCP7940M RTC
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Read( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum )
// PURPOSE  : Reads one register from the MCP7940M RTC
//----------------------------------------------------------------------------

void MCP7940M_Read( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum )
{
    asm (" cpsid i");
    
    uint8_t uiMCS;
    uint8_t uiRegsRead = 0;

    if( uiNum )
    {
        // Write slave address and R/W = 1
        HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 );

        // Write register address to the transmit register
        HWREG( I2C0_BASE + I2C_O_MDR ) = uiReg;

        // Initiate I2C transaction
        uiMCS = I2C_MCS_RUN | I2C_MCS_START;
        HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

        // Wait until the controller is no longer busy
        while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

        do
        {
            // Write slave address and R/W = 1
            HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 ) | (0x01);

            // Initiate I2C transaction
            uiMCS = I2C_MCS_RUN | I2C_MCS_START;
            HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

            // Wait until the controller is no longer busy
            while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

            // Read register data
            *puiData = HWREG( I2C0_BASE + I2C_O_MDR );
            puiData++;

            // Continue with the I2C transaction
            uiMCS = I2C_MCS_RUN | ( ( uiNum - uiRegsRead ) == 1 ? I2C_MCS_STOP : 0 );
            HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

            // Wait until the controller is no longer busy
            while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

            // Another register written
            uiRegsRead++;

        } while( uiNum - uiRegsRead );
    }
    HWREG( I2C0_BASE + I2C_O_MDR ) = 0;

    asm (" cpsie i");
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : MCP7940M_Write( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
// PURPOSE  : Writes one register to the MCP7940M RTC
//----------------------------------------------------------------------------

void MCP7940M_Write( uint8_t uiRegAddr, uint8_t* puiData, uint8_t uiNumRegs )
{
    asm(" cpsid i");
    uint8_t uiMCS;
    uint8_t uiRegsWritten = 0;

    if( uiNumRegs )
    {
        // Write slave address and R/W = 0
        HWREG( I2C0_BASE + I2C_O_MSA ) = ( MCP7940M_SA << 1 );

        // Write register address to the transmit register
        HWREG( I2C0_BASE + I2C_O_MDR ) = uiRegAddr;

        // Initiate I2C transaction
        uiMCS = I2C_MCS_RUN | I2C_MCS_START;
        HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

        // Wait until the controller is no longer busy
        while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

        do
        {
            // Write register data
            HWREG( I2C0_BASE + I2C_O_MDR ) = *puiData++;

            // Continue with the I2C transaction
            uiMCS = I2C_MCS_RUN | ( ( uiNumRegs - uiRegsWritten ) == 1 ? I2C_MCS_STOP : 0 );
            HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

            // Wait until the controller is no longer busy
            while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

            // Another register written
            uiRegsWritten++;

        } while( uiNumRegs - uiRegsWritten );
    }
    HWREG( I2C0_BASE + I2C_O_MDR ) = 0;
    asm(" cpsie i");
    return;
}

void MAX518_Write(uint8_t* puiData, uint8_t uiContrast)
{
    asm(" cpsid i");
    uint8_t uiMCS;
    uint8_t MAX518_Command = 0;

    // Write slave address and R/W = 0
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( MAX518_SA << 1 ) | (0x00);

    //*****Send Command*****//
    //*********************//
    // Write register data
    HWREG( I2C0_BASE + I2C_O_MDR ) = MAX518_Command;

    // Initiate I2C transaction
    //Sending Command
    uiMCS = I2C_MCS_RUN | I2C_MCS_START;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

    //*****DAC0 Output*****//
    //*********************//
    // Write register data
    HWREG( I2C0_BASE + I2C_O_MDR ) = ( uint8_t )( ( (uiContrast * 255) / 100 ) );//*puiData;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

    //*****Send Command*****//
    //*********************//
    // Write register data
    HWREG( I2C0_BASE + I2C_O_MDR ) = MAX518_Command | 0x01;

    // Initiate I2C transaction
    //Sending Command
    uiMCS = I2C_MCS_RUN;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

    //*****DAC1 Output*****//
    //*********************//
    // Write register data
    HWREG( I2C0_BASE + I2C_O_MDR ) = *puiData;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

    HWREG( I2C0_BASE + I2C_O_MDR ) = 0;

    asm(" cpsie i");

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Read( uint8_t uiSA, uint8_t* puiData )
// PURPOSE  : Reads one byte from the PCF8574A
//----------------------------------------------------------------------------
uint8_t PCF8574A_Read( uint8_t uiSA )
{
    asm(" cpsid i");

    uint8_t uiMCS;
    uint8_t data = 0;

    // Write slave address and R/W = 1
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 ) | 0x01;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );


    // Read data from receive register
    data = HWREG( I2C0_BASE + I2C_O_MDR );

    HWREG( I2C0_BASE + I2C_O_MDR ) = 0;

    asm(" cpsie i");

    return ( data );
}

//----------------------------------------------------------------------------
// FUNCTION : PCF8574A_Write( uint8_t uiSA, uint8_t* puiData )
// PURPOSE  : Writes one byte to the PCF8574A
//----------------------------------------------------------------------------
void PCF8574A_Write( uint8_t uiSA, uint8_t* puiData )
{
    asm(" cpsid i");
    uint8_t uiMCS;

    // Write slave address and R/W = 0
    HWREG( I2C0_BASE + I2C_O_MSA ) = ( uiSA << 1 ) | (0x00);

    // Write data to transmit register
    HWREG( I2C0_BASE + I2C_O_MDR ) = *puiData;

    // Initiate I2C transaction
    uiMCS = I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    HWREG( I2C0_BASE + I2C_O_MCS ) = uiMCS;

    // Wait until the controller is no longer busy
    while( HWREG( I2C0_BASE + I2C_O_MCS ) & I2C_MCS_BUSY );

    HWREG( I2C0_BASE + I2C_O_MDR ) = 0;

    asm(" cpsie i");

    return;
}

void getTime()
{
    //Read Seconds
    MCP7940M_Read(REG_SECONDS, &TIMEandDATE.SECONDS, 1 );
    TIMEandDATE.SECONDS &= 0b01111111;

    //Read Minutes
    MCP7940M_Read(REG_MINUTES, &TIMEandDATE.MINUTES, 1 );
    TIMEandDATE.MINUTES &= 0b01111111;


    //Read Hour
    MCP7940M_Read(REG_HOUR, &TIMEandDATE.HOUR, 1 );
    TIMEandDATE.HOUR &= 0b00011111;
}

void getDate()
{
    //Read Weekday
    MCP7940M_Read(REG_WEEKDAY, &TIMEandDATE.WEEKDAY, 1 );
    TIMEandDATE.WEEKDAY &= 0b00000111;

    //Read Date
    MCP7940M_Read(REG_DATE, &TIMEandDATE.DATE, 1 );
    TIMEandDATE.DATE &= 0b00111111;

    //Read Month
    MCP7940M_Read(REG_MONTH, &TIMEandDATE.MONTH, 1 );
    TIMEandDATE.MONTH &= 0b00011111;

    //Read Year
    MCP7940M_Read(REG_YEAR, &TIMEandDATE.YEAR_LOW, 1 );
    TIMEandDATE.YEAR_LOW &= 0b11111111;
}

void setTime_Date(uint8_t sec, uint8_t min, uint8_t hour, _Bool AMPM,uint8_t weekday, uint8_t date, uint8_t month, uint16_t year)
{
    TIMEandDATE.SECONDS = sec;
    TIMEandDATE.MINUTES = min;
    TIMEandDATE.HOUR = hour;
    TIMEandDATE.WEEKDAY = weekday;
    TIMEandDATE.DATE = date;
    TIMEandDATE.MONTH = month;
    TIMEandDATE.AMPM = AMPM;

    //| ONES | TENS | HUNDREDS | THOUSANDS |
    uint8_t year_digits[4];
    int i;
    for(i = 0; i < 4; i++)
    {
        year_digits[ i ] = year % 10;
        year = year / 10;
    }

    TIMEandDATE.YEAR_LOW = ( year_digits[0] & 0x0F );
    TIMEandDATE.YEAR_LOW |= ( ( year_digits[1] << 4 ) & 0xF0 );

    TIMEandDATE.YEAR_HIGH = ( year_digits[2] & 0x0F );
    TIMEandDATE.YEAR_HIGH |= ( ( year_digits[3] << 4 ) & 0xF0 );

    MCP7940M_Init(&TIMEandDATE);

    return;
}

//----------------------------------------------------------------------------
// END MCP7940M.C
//----------------------------------------------------------------------------
