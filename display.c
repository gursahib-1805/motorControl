//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : DISPLAY.C
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
// Updates the display (LCD Screen)
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "global.h"
#include "hwdefs.h"
#include "display.h"
#include "lcd.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
extern uint8_t g_uiCONTROLFLAGS;
extern int8_t uiSCRN;
//----------------------------------------------------------------------------
// FUNCTION : Buttons_Init( void )
// PURPOSE  : Initializes the pins to which buttons are connected
//----------------------------------------------------------------------------

void Reset_Display( void )
{
    LCD_IRWrite(LCD_IC_CLEAR);

    //Delay to Clear the LCD
    uint64_t i;
    for(i = 0; i < 100000; i++)
    {
        asm(" nop");
    }

    Update_Display(true);
}

void Update_Display ( bool bDis_Reset )
{
    if(bDis_Reset)
        uiSCRN = CSCRN_EL507;

    if(g_uiCONTROLFLAGS & (1 << CONTROLFLAGS_BUTTON_2))
    {
        uiSCRN++;
        if(uiSCRN > CSCRN_RPM)
            uiSCRN = CSCRN_EL507;
    }

    if(g_uiCONTROLFLAGS & (1 << CONTROLFLAGS_BUTTON_3))
    {
        uiSCRN--;
        if(uiSCRN < CSCRN_EL507)
            uiSCRN = CSCRN_RPM;
    }

    switch(uiSCRN)
    {

    case CSCRN_EL507:
        LCD_IRWrite(LCD_IC_DDRAMADDR | 0x00);
        LCD_SendMessage("EL 507 - LAB 10  ");
        LCD_IRWrite(LCD_IC_DDRAMADDR | 0x40);
        LCD_SendMessage("Embedded Systems");
        break;

    case CSCRN_TIME:
        LCD_IRWrite(LCD_IC_DDRAMADDR | 0x00);
        LCD_SendMessage("Date: YYYY-MM-DD");
        LCD_IRWrite(LCD_IC_DDRAMADDR | 0x40);
        LCD_SendMessage("Time:   HH:MM:SS");
        break;

    case CSCRN_RPM:
        LCD_IRWrite(LCD_IC_DDRAMADDR | 0x00);
        LCD_SendMessage("SPEED: ###.# RPM");
        LCD_IRWrite(LCD_IC_DDRAMADDR | 0x40);
        LCD_SendMessage("                ");
        break;

    }


}

/*void Update_Dynamic_Display( uint8_t SCRN_Select )
{
    if(CHECK_BIT(SCRN_Select, 0))
    {
        //Display the ADC Value on 1st Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x00 + 8 ) );
        LCD_SendMessage( ( uint8_t* )voltageAIN4.Digital );

        //Display the Analog Voltage from Potentiometer on 2nd Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x40 + 8 ) );
        LCD_SendMessage( ( uint8_t* )voltageAIN4.Analog );

        return;
    }

    else if(CHECK_BIT(SCRN_Select, 1))
    {
        return;
    }

    else if(CHECK_BIT(SCRN_Select, 2))
    {
        return;
    }

    else if(CHECK_BIT(SCRN_Select, 3))
    {
        return;
    }

    else if(CHECK_BIT(SCRN_Select, 4))
    {
        //Display the ADC Value on 1st Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x00 + 8 ) );
        LCD_SendMessage( ( uint8_t* )hex_intTemperature );

        //Display Analog Value on 2nd Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x40 + 8 ) );
        LCD_SendMessage( ( uint8_t* )intTemperature_C.Analog );

        return;
    }

    else if(CHECK_BIT(SCRN_Select, 5))
    {
        //Display the ADC Value on 1st Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x00 + 8 ) );
        LCD_SendMessage( ( uint8_t* )hex_intTemperature );

        //Display Analog Value on 2nd Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x40 + 8 ) );
        LCD_SendMessage( ( uint8_t* )intTemperature_F );

        return;
    }

    else if(CHECK_BIT(SCRN_Select, 6))
    {
        //Display the ADC Value on 1st Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x00 + 8 ) );
        LCD_SendMessage( ( uint8_t* )hex_intTemperature );

        //Display Analog Value on 2nd Line
        LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x40 + 8 ) );
        LCD_SendMessage( ( uint8_t* )intTemperature_K );

        return;
    }
}*/

//----------------------------------------------------------------------------
// END DISPLAY.C
//----------------------------------------------------------------------------
