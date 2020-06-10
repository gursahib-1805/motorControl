 //----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MAIN.C
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
// Generates the heart-beat sequencing for the system. The HB_Sequencer()
// function must be called once every millisecond.
//
//----------------------------------------------------------------------------

//Calculating the calibration factor for RPM
//120 ---- 101 ---- 1.18
//100 ---- 84.6 --- 1.18
//80 ----- 67.5 --- 1.18

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "global.h"
#include "systick.h"
#include "led.h"
#include "hb.h"
#include "uart.h"
#include "lcd.h"
#include "reset.h"
#include "button.h"
#include "display.h"
#include "contact.h"
#include "hwdefs.h"
#include "adc.h"
#include "osc.h"
#include "i2c.h"
#include "mcp7940m.h"
#include "motor.h"
#include "qei.h"
#include "watchdog.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

// System Flags
uint32_t g_uiSYSFLAGS;

//Control Flags
uint8_t g_uiCONTROLFLAGS;

//Number of Sequencer Pulses
uint8_t Sequencer_Pulses;

//Screen Number
int8_t uiSCRN = CSCRN_EL507;

//Temperature to be sent on UART
float extTemp_uartSend = 0;
float intTemp_uartSend = 0;
float RPM_Read_uartSend = 0;

//Data to be sent on Pin 8 of DAC
uint8_t U7_8 = 0;
float fAIN4 = 0;
float R17_Voltage_for_RPM = 0;
float Motor_Speed = 0;
float fAIN5 = 0;
float fIntTemp = 0;
uint32_t motorCounter = 0;
float Motor_RPM = 0.0f;
uint8_t last_buttonPressed = 0;
uint64_t buttonCounter = 0;
float RPM_Read = 0.0f;
float RPM_Given = 0.0f;
float error = 0.0f;
float adj = 0.0f;
uint32_t WDT_Load_Value = 80000000;

uint8_t PCF8574_Status;

uint8_t  g_aData[ 97 ];
char g_TimeBuffer[ 80 ];
char g_DateBuffer[ 80 ];
char Motor_RPM_Dis[ 80 ];

float fDutyCycle    = 0.0f;
bool  bDirection    = 0;
bool  bAcceleration = 1;

//----------------------------------------------------------------------------
// FUNCTION : Initialize( void )
// PURPOSE  : Initialize embedded system
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------
void Serial_Terminal_Header( void );
void Initialize( void );
void UpdateADCValues( void );
uint8_t ReadIOExpanderStatus( void );
void PB4_Interrupt_Init( void );
void GPIO_IntHandler();

//----------------------------------------------------------------------------
// FUNCTION : main( void )
// PURPOSE  : Main program entry point - thread mode
//----------------------------------------------------------------------------
int main( void )
{
    //Identify the cause of Reset
    Sequencer_Pulses = Reset_Cause();

    // Initialize System
    Initialize();

    // Variables for timings
    uint32_t ui100msInterval = 100;
    uint8_t ui250msInterval = 250;
    uint32_t ui1sInterval = 1000;
    uint8_t buttonPressed = 0;

    // Send UART terminal header
    Serial_Terminal_Header();
   // fDutyCycle = 1.0;
    //MOTOR_SetDutyCycle( fDutyCycle, bDirection );
    while( 1 )
    {
        Watchdog_Timer_Reload( &WDT_Load_Value );
        // If there are no pending events, go into low-power mode (wait for interrupt)
        if( !g_uiSYSFLAGS )
            asm(" wfi");

        if(g_uiSYSFLAGS & (1 << SYSFLAGS_READ_PCF))
        {
            g_uiSYSFLAGS &= ~( 1 << SYSFLAGS_READ_PCF );
            buttonPressed = ReadIOExpanderStatus();
        }

        //----------------------------------------------------------------------------
        // Perform 1 ms Tasks
        //----------------------------------------------------------------------------
        if( CHECK_BIT(g_uiSYSFLAGS, SYSFLAGS_SYS_TICK) )
        {
            // Clear System Tick Event
            CLEAR(g_uiSYSFLAGS, SYSFLAGS_SYS_TICK);

            // Update LED2
            LED_2_Control();

            // Update Heart-beat Sequencer
            HB_Sequencer( 0 );
            // Read the status of IO Expander Buttons

            if(( (buttonPressed == 4) || (buttonPressed == 6) ) && (g_uiSYSFLAGS & (1 << SYSFLAGS_MOTOR_MODE)))
            {
                buttonCounter++;
                if(buttonCounter == 50)
                {
                    if( ((buttonPressed == 4) && ( (Motor_RPM + 1.0f) <= 120.0f)) )
                    {
                        Motor_RPM = ( Motor_RPM + 1.0f );
                    }

                    if( ((buttonPressed == 6) && ( (Motor_RPM - 1.0f) >= 0.0f)) )
                    {
                        Motor_RPM = ( Motor_RPM - 1.0f );
                    }

                }

                if((buttonCounter > 0) && ((buttonCounter % 1000) == 0))
                {
                    if( ((buttonPressed == 4) && ( (Motor_RPM + 20.0f) <= 120.0f)) )
                    {
                        Motor_RPM = ( Motor_RPM + 20.0f );
                    }
                    if( ((buttonPressed == 6) && ( (Motor_RPM - 20.0f) >= 0.0f)) )
                    {
                        Motor_RPM = ( Motor_RPM - 20.0f );
                    }
                }
            }

            if( !(g_uiSYSFLAGS & (1 << SYSFLAGS_MOTOR_MODE)) )
            {
                Motor_RPM = 120.0f * (R17_Voltage_for_RPM / (VREFP - VREFN));
            }

            if(buttonPressed == 0)
            {
                buttonCounter = 0;
            }

            //----------------------------------------------------------------------------
            // Perform 100 ms Tasks
            //----------------------------------------------------------------------------
            if( !--ui100msInterval )
            {
                MOTOR_SetDutyCycle( fDutyCycle, bDirection );
                RPM_Read = QEI_GetSpeed();
                RPM_Read = (RPM_Read / RPM_FACTOR);
                RPM_Read_uartSend = RPM_Read;
//                if( !--ui10msInterval )
                {
                    RPM_Given = Motor_RPM;
                    error = RPM_Given - RPM_Read;

                    adj = (( fabs(error) / 120.0f) * 0.10f);

                    if((error) > 0)
                    {
                        fDutyCycle += adj;
                    }

                    if((error) < 0)
                    {
                        fDutyCycle -= adj;
                    }

                    if(error == 0)
                    {

                    }
//                    ui10msInterval = 10;
                }
                ui100msInterval = 1000;
            }

            //----------------------------------------------------------------------------
            // Perform 250 ms Tasks
            //----------------------------------------------------------------------------
            if( !--ui250msInterval )
            {
                // Start a new set of ADC conversions on SS0
                ADC_SS0_Trigger();
                UpdateADCValues();

                if(uiSCRN == CSCRN_RPM)
                {
                    //if( ( RPM_Read < (RPM_Given + 0.5f) ) &&  ( RPM_Read > (RPM_Given - 0.5f) ) )
                        sprintf( Motor_RPM_Dis, "%4.2f", RPM_Read );

                    //Display RPM on the 1st Line
                    LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x00 + 7 ) );
                    LCD_SendMessage( ( uint8_t* )Motor_RPM_Dis );

                    // Display Blank Line
                    LCD_IRWrite( LCD_IC_DDRAMADDR | ( 0x40 ) );
                    LCD_SendMessage("                ");
                }
                ui250msInterval = 250;
            }

            //----------------------------------------------------------------------------
            // Perform 1 s Tasks
            //----------------------------------------------------------------------------
            if( !--ui1sInterval )
            {
                //Write to DAC
                U7_8 = (uint8_t)(((fAIN5/10)/5.0f)*256);
                MAX518_Write( &U7_8, CONTRAST_PRCNT );

                //Update Dynamic Display
                if(uiSCRN == CSCRN_TIME)
                {
                    getTime();
                    getDate();
                    sprintf( g_DateBuffer, "Date: %02X%02X-%02X-%02X", TIMEandDATE.YEAR_HIGH, TIMEandDATE.YEAR_LOW, TIMEandDATE.MONTH, TIMEandDATE.DATE );
                    sprintf( g_TimeBuffer, "Time:   %02X:%02X:%02X", TIMEandDATE.HOUR, TIMEandDATE.MINUTES, TIMEandDATE.SECONDS );

                    //Display Time on 1st Line
                    LCD_IRWrite( LCD_IC_DDRAMADDR | 0x00 );
                    LCD_SendMessage( ( uint8_t *) g_DateBuffer );
                    //Display Date on 2nd Line
                    LCD_IRWrite( LCD_IC_DDRAMADDR | 0x40 );
                    LCD_SendMessage( ( uint8_t *) g_TimeBuffer);
                }
                ui1sInterval = 1000;
            }
            Monitor_Buttons();
        }
    }
    return (0);
}

//----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : void Serial_Terminal_Header ( void )
// PURPOSE  : Sends initial terminal commands
//----------------------------------------------------------------------------
void Serial_Terminal_Header( void )
{
    // Send Initial Messages on UART...!!
    UART_SendMessage( "\r\n---------------------------------------\r" );
    UART_SendMessage( "\r\nFirmware   : EL 507 - LAB 10\r" );
    UART_SendMessage( "\r\nSystem     : Speed Control\r" );
    UART_SendMessage( "\r\nCompany    : Confederation College\r" );
    UART_SendMessage( "\r\nPlatform   : TM4C123GH6PM\r" );
    UART_SendMessage( "\r\nDate       : 2019-12-10\r" );
    UART_SendMessage( "\r\nProgrammer : Gursahib Singh\r" );
    UART_SendMessage( "\r\n---------------------------------------\r" );

    switch(Sequencer_Pulses)
    {
    case EXTERNAL_RESET:
        UART_SendMessage( " \r\nSYSTEM RESTART - HARDWARE RESET\r\n" );
        break;
    case POWER_ON_RESET:
        UART_SendMessage( "\r\nSYSTEM RESTART - POWER FAIL\r\n" );
        break;
    case WATCHDOG_TIMER_RESET:
        UART_SendMessage( "\r\nSYSTEM RESTART - WATCHDOG TIMER\r\n" );
        break;
    case SOFTWARE_RESET:
        UART_SendMessage( "\r\nSYSTEM RESTART - SOFTWARE RESET\r\n" );
        break;
    default:
        UART_SendMessage( "\r\nSYSTEM RESTART - UNKNOWN\r\n" );
        break;
    }
}


//----------------------------------------------------------------------------
// FUNCTION : void Initialize ( void )
// PURPOSE  : Initializes required peripherals
//----------------------------------------------------------------------------
void Initialize( void )
{
    //Reset the Control Flags
    g_uiSYSFLAGS = 0;
    g_uiCONTROLFLAGS = 0;

    // Delay before initializing the LCD
    uint64_t i;
    for(i = 0; i < 200000; i++)
    {
        asm(" nop");
    }

    LCD_Init(); //....................................Initialize LCD
    Reset_Display(); //...............................Reset Diaplay
    SYSTICK_Init(); //................................Initialize Systick Timer
    LED_Init(); //....................................Initialize LEDs
    Buttons_Init(); //................................Initialize Buttons

    QEI_Init(); //....................................Initialize Quadrature Encoder

    MOTOR_Init(); //..................................Initialize Motor

    //Manual Mode by Default
    g_uiSYSFLAGS |= (1 << SYSFLAGS_MOTOR_MODE);

    PB4_Interrupt_Init(); //..........................Initialize Motor
    UART_Init(); //...................................Initialize UART

    #ifdef FREQ_80MHz
    OSC_Init();
    #endif

    I2C_Init(); //....................................Initialize I2C
    MAX518_Write( &U7_8, CONTRAST_PRCNT ); //.........Write Contrast

    //Set Date and Time
    setTime_Date( 0x00, 0x45, 0x11, PM, TUESDAY, 0x18, 0x08, 1991 );

    // Read Time
    getTime();

    // Read Date
    getDate();

    //Reset the Sequencer
    HB_Sequencer( 1 );

    //Initialize ADC
    ADC_Init();

    //Initalize Watchdog Timer 0
    if(!Watchdog_Timer_Init( &WDT_Load_Value ))
        while(1);
    asm(" nop");
    return;
}

//----------------------------------------------------------------------------
// FUNCTION : void UpdateADCValues ( void )
// PURPOSE  : Updates and Stores the Digital Values
//----------------------------------------------------------------------------
void UpdateADCValues( void )
{
    //Array for ADC Values
    uint16_t aValues[ 3 ];

    // Conversion complete - update information
    if( g_uiSYSFLAGS & ( 1 << SYSFLAGS_ADC_SS0 ) )
    {
        g_uiSYSFLAGS &= ~( 1 << SYSFLAGS_ADC_SS0 ); //....................................... Clear system ADC SS0 event
        if( ADC_SS0_Read( aValues, NUM_ELEMENTS( aValues ) ) == NUM_ELEMENTS( aValues ) )
        {
            // Format AIN4 reading
            fAIN4 = aValues[ 0 ];
            fAIN4 = R17_Voltage_for_RPM = ( fAIN4 / 4096.0f ) * 3.3f;

            // Format AIN5 reading
            fAIN5 = aValues[ 1 ];
            fAIN5 = extTemp_uartSend = (((( fAIN5 / 4096.0f ) * 3.3f)/6.66)*100);

            // Format internal temperature reading
            /*
             * See specifications on page 1390 of the TM4C123GH6PM datasheet
             *  VTSENS =   1.633 V @ 25�C
             * STSENS = -13.3 mV/�C
             * ETSENE = +/-5�C
             */
             fIntTemp = aValues[ 2 ] - 200;
             fIntTemp = intTemp_uartSend = 147.5f - ( 75.0f * 3.3f * ( fIntTemp / 4096.0f ) );

            // Flush out remaining readings from the FIFO (if any)
            while( ADC_SS0_Read( aValues, 1 ) );
        }
    }
}

//----------------------------------------------------------------------------
// FUNCTION : ReadIOExpanderStatus ( void )
// PURPOSE  : Returns the status of the IO Expander Pins (only buttons)
//----------------------------------------------------------------------------
uint8_t ReadIOExpanderStatus( void )
{
    uint8_t buttonPressed1 = 0;
    uint8_t LEDs;
    uint8_t buttons;

    // Read the Status of IO Expander
    buttons = PCF8574A_Read(PCF8574_SA);

    if(!(buttons & ( 1 << 0 )))
    {
        LEDs = 0xFF;
        LEDs &= ~(1 << LED4);
        buttonPressed1 = PB4_PRESSED;
    }

    else if(!(buttons & ( 1 << 1 )))
    {
        LEDs = 0xFF;
        LEDs &= ~(1 << LED5);
        buttonPressed1 = PB5_PRESSED;
    }

    else if(!(buttons & ( 1 << 2 )))
    {
        LEDs = 0xFF;
        LEDs &= ~(1 << LED6);
        buttonPressed1 = PB6_PRESSED;
    }

    else
    {
        LEDs = 0xFF;
        buttonPressed1 = NO_BUTTON_PRESSED;
    }
    PCF8574A_Write(PCF8574_SA, &LEDs);
    return (buttonPressed1);
}

//----------------------------------------------------------------------------
// FUNCTION : PB4_Interrupt_Init ( void )
// PURPOSE  : Initialize Interrupt for PORTB Pin 4
//----------------------------------------------------------------------------
void PB4_Interrupt_Init( void )
{
    HWREG( SYSCTL_RCGCGPIO ) |= 0x02;                               // GPIO Port B Run Mode Clock Gating Control
    HWREG( GPIO_PORTB_BASE + GPIO_O_DEN ) |= PORTB_PIN4;            // Configure PB4 as digital signal
    HWREG( GPIO_PORTB_BASE + GPIO_O_DIR ) &= ~PORTB_PIN4;           // Configure PB4 as input
    HWREG( GPIO_PORTB_BASE + GPIO_O_IS ) &= ~PORTB_PIN4;            // Detect interrupt on edge
    HWREG( GPIO_PORTB_BASE + GPIO_O_IEV ) &= ~PORTB_PIN4;           // Detect interrupt on falling edge
    HWREG( GPIO_PORTB_BASE + GPIO_O_IM ) |= PORTB_PIN4;             // Enable the interrupt
    HWREG( NVIC_EN0 ) = ( 1 << 1 );                                 // Enable GPIO Interrupt in NVIC
}

//----------------------------------------------------------------------------
// FUNCTION : GPIO_IntHandler ( void )
// PURPOSE  : Handles the external interrupt on PB4
//----------------------------------------------------------------------------

void GPIO_IntHandler( void )
{
    HWREG( GPIO_PORTB_BASE + GPIO_O_ICR ) |= ( 1 << 4 );    // Clear the interrupt
    g_uiSYSFLAGS |= (1 << SYSFLAGS_READ_PCF);               // Set flag for PCF7940 Read
    asm(" nop");
}


//----------------------------------------------------------------------------
// END MAIN.C
//----------------------------------------------------------------------------
