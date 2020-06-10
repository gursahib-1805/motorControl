//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MCP7940M.H
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
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef MCP7940M_H_
#define MCP7940M_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

#include "i2c.h"

//----------------------------------------------------------------------------
// MACROS
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// STRUCTURES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void    MCP7940M_Init( TIMEDATE *pTIMEDATE );
void    MCP7940M_Read( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum );
void    MCP7940M_Write( uint8_t uiReg, uint8_t* puiData, uint8_t uiNum );

void MAX518_Write(uint8_t* puiData, uint8_t uiContrast);

void getTime();
void getDate();
void setTime_Date(uint8_t sec,
                  uint8_t min,
                  uint8_t hour,
                  _Bool AMPM,
                  uint8_t weekday,
                  uint8_t date,
                  uint8_t month,
                  uint16_t year);

void PCF8574A_Write( uint8_t uiSA, uint8_t* puiData );
uint8_t PCF8574A_Read( uint8_t uiSA );

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define MCP7940M_SA 0x6F
#define MAX518_SA   0x2C
#define PCF8574_SA  0x38

enum MCP7940M_REGISTERS
{
    REG_SECONDS = 0,
    REG_MINUTES,
    REG_HOUR,
    REG_WEEKDAY,
    REG_DATE,
    REG_MONTH,
    REG_YEAR,
    REG_CONTROL
};

enum MCP7940M_COMMANDS
{
    MCP7940M_CMD_RESET = 0,
    MCP7940M_CMD_WRITE,
    MCP7940M_CMD_READ,
    MCP7940M_CMD_PROCESS
};

enum MCP7940M_STATES
{
    MCP7940M_STATE_IDLE = 0,
    MCP7940M_STATE_START,
    MCP7940M_STATE_START_WFR,
    MCP7940M_STATE_RESTART,
    MCP7940M_STATE_RESTART_WFR,
    MCP7940M_STATE_DATA_WRITE,
    MCP7940M_STATE_DATA_WRITE_WFR,
    MCP7940M_STATE_DATA_READ,
    MCP7940M_STATE_DATA_READ_WFR
};

#endif // MCP7940M_H_

//----------------------------------------------------------------------------
// END MCP7940M.H
//----------------------------------------------------------------------------
