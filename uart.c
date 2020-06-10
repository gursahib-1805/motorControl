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
// Provides a communication link to another system.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdio.h>

#include "global.h"
#include "hwdefs.h"
#include "uart.h"
#include "queue.h"
#include "lcd.h"
#include "mcp7940m.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
extern uint8_t g_uiCONTROLFLAGS;
extern uint32_t g_uiSYSFLAGS;
extern float extTemp_uartSend;
extern float intTemp_uartSend;
extern float RPM_Read_uartSend;
extern float Motor_RPM;


QUEUE* g_pQueueTransmit;

//----------------------------------------------------------------------------
// FUNCTION : UART0_IntHandler( void )
// PURPOSE  : Handles the UART interrupts
//----------------------------------------------------------------------------

void UART0_IntHandler( void )
{
    uint32_t uiIntStatus = HWREG( UART0_BASE + UART_O_MIS );
    uint8_t uiData;


    // Check for transmit (FIFO empty) interrupt
    if( uiIntStatus & ( 1 << 5 ) )
    {
        // Clear interrupt
        HWREG( UART0_BASE + UART_O_ICR ) |= ( 1 << 5 );

        // Continue transmitting anything left in the queue
        UART_TransmitFromQueue();
    }

    // Check for receive interrupt
    if( uiIntStatus & ( 1 << 4 ) )
    {
        // Clear interrupt
        HWREG( UART0_BASE + UART_O_ICR ) |= ( 1 << 4 );

        // Read data and loop back
        uiData = HWREG( UART0_BASE + UART_O_DR );

        if( uiData == '?' )         // Required
        {
            UART_SendMessage( "\r\nAvailable Commands: \r" );
            UART_SendMessage( "\r\n" );
            UART_SendMessage( "\r\nC         - Display the date and time \r" );
            UART_SendMessage( "\r\nT         - Display the ambient and internal temperature \r" );
            UART_SendMessage( "\r\nR         - Display the speed of the output shaft in RPM \r" );
            UART_SendMessage( "\r\n0-9,F     - Sets the speed of the output shaft \r" );
            UART_SendMessage( "\r\nA         - Change the mode of control to automatic \r" );
            UART_SendMessage( "\r\nM         - Change the mode of csontrol to manual \r" );
            UART_SendMessage( "\r\nI         - Display system information \r" );
            UART_SendMessage( "\r\nL         - Toggles the state of LED3 \r" );
            UART_SendMessage( "\r\n" );
            UART_SendMessage( "\r\n<Ctrl>+R  - Resets the embedded system \r\n" );
        }

        else if(uiData == 'C')      // Required
        {
            getTime();
            getDate();
            char date[256];
            char time[256];
            sprintf( date, "\r\n%02X%02X-%02X-%02X", TIMEandDATE.YEAR_HIGH, TIMEandDATE.YEAR_LOW, TIMEandDATE.MONTH, TIMEandDATE.DATE );
            sprintf( time, "T%02X:%02X:%02X\r\n", TIMEandDATE.HOUR, TIMEandDATE.MINUTES, TIMEandDATE.SECONDS );

            UART_SendMessage((uint8_t*)date);
            UART_SendMessage((uint8_t*)time);
        }

        else if(uiData == 'T')      // Required
        {
            char extTemp[256];
            char intTemp[256];
            sprintf(extTemp,"\r\nAmbient Temperature: %2.0f%c%cC", extTemp_uartSend, 0xC2, 0xB0);
            sprintf(intTemp,"\r\nInternal Temperature: %2.0f%c%cC\r\n", intTemp_uartSend, 0xC2, 0xB0);
            UART_SendMessage((uint8_t*)extTemp);
            UART_SendMessage((uint8_t*)intTemp);
        }

        else if( uiData == 'R')     // Required
        {
            char RPM[256];
            sprintf(RPM,"\r\nSpeed: %4.1f RPM\r\n", RPM_Read_uartSend);
            UART_SendMessage((uint8_t*)RPM);
        }

        else if( uiData == 'M' )    // Required
        {
            g_uiSYSFLAGS |= ( 1 << SYSFLAGS_MOTOR_MODE );       //SET MANUAL MOTOR
            char ModeM[256];
            sprintf(ModeM,"\r\nMode: Manual\r\n");
            UART_SendMessage((uint8_t*)ModeM);
            Motor_RPM = 0.0f;
        }

        else if( uiData == 'A' )   // Required
        {
            g_uiSYSFLAGS &= ~( 1 << SYSFLAGS_MOTOR_MODE );      //SET AUTOMATIC MOTOR
            char ModeA[256];
            sprintf(ModeA,"\r\nMode: Automatic\r\n");
            UART_SendMessage((uint8_t*)ModeA);
        }

        else if ( (uiData == 'F') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )    // Required
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 100% of maximum (120.0 RPM)\r\n" );
            Motor_RPM = 120.0f;
        }

        else if(uiData == 'I')      // Required
        {
            getTime();
            getDate();
            char date[256];
            char time[256];
            sprintf( date, "\r\nDate: %02X%02X-%02X-%02X", TIMEandDATE.YEAR_HIGH, TIMEandDATE.YEAR_LOW, TIMEandDATE.MONTH, TIMEandDATE.DATE );
            sprintf( time, "\r\nTime: %02X:%02X:%02X\r\n", TIMEandDATE.HOUR, TIMEandDATE.MINUTES, TIMEandDATE.SECONDS );

            UART_SendMessage( "\r\nEL 507 Embedded Control Systems II\r" );
            UART_SendMessage( "\r\nEmbedded System V1.0\r" );
            UART_SendMessage( "\r\n" );
            UART_SendMessage((uint8_t*)date);
            UART_SendMessage((uint8_t*)time);
        }

        else if( uiData == 'L' )    // Required
        {
            HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x08 << 2 ) ) ^= 0x08;
            if ( HWREG(GPIO_PORTF_BASE + GPIO_O_DATA + ( 0x08 << 2 )) )
                UART_SendMessage( "\r\nLED3: ON\r\n" );
            else
                UART_SendMessage( "\r\nLED3: OFF\r\n" );
        }

        else if ( (uiData == '0') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 0% of maximum (0.0 RPM)\r\n" );
            Motor_RPM = 0.0f;
        }

        else if ( (uiData == '1') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 10% of maximum (12.0 RPM)\r\n" );
            Motor_RPM = 12.0f;
        }

        else if ( (uiData == '2') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 20% of maximum (24.0 RPM)\r\n" );
            Motor_RPM = 24.0f;
        }

        else if ( (uiData == '3') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 30% of maximum (36.0 RPM)\r\n" );
            Motor_RPM = 36.0f;
        }

        else if ( (uiData == '4') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 40% of maximum (48.0 RPM)\r\n" );
            Motor_RPM = 48.0f;
        }

        else if ( (uiData == '5') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 50% of maximum (60.0 RPM)\r\n" );
            Motor_RPM = 60.0f;
        }

        else if ( (uiData == '6') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 60% of maximum (72.0 RPM)\r\n" );
            Motor_RPM = 72.0f;
        }

        else if ( (uiData == '7') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 70% of maximum (84.0 RPM)\r\n" );
            Motor_RPM = 84.0f;
        }

        else if ( (uiData == '8') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 80% of maximum (96.0 RPM)\r\n" );
            Motor_RPM = 96.0f;
        }

        else if ( (uiData == '9') && (g_uiSYSFLAGS & ( 1 << SYSFLAGS_MOTOR_MODE )) )
        {
            UART_SendMessage( "\r\nOutput shaft speed set to 90% of maximum (108.0 RPM)\r\n" );
            Motor_RPM = 108.0f;
        }

        else if(uiData == 0x12)
        {
            asm(" nop");
            HWREG(NVIC_APINT) = (NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESREQ);
        }

        else
        {

            g_uiCONTROLFLAGS |= (1 << CONTROLFLAGS_LED_2);              //Set LED2 Flag
            QUEUE_Enqueue( g_pQueueTransmit, uiData );
            UART_TransmitFromQueue();
        }
    }

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : UART_TransmitFromQueue()
// PURPOSE  : Transmits a message from the global transmit queue
//----------------------------------------------------------------------------

void UART_TransmitFromQueue( void )
{
    uint8_t uiData;

    // If there are no transmissions occurring (TXFE = 1), then start one
    if( HWREG( UART0_BASE + UART_O_FR ) & ( 1 << 7 ) )
    {
        // Remove byte from queue and transmit
        if( QUEUE_Dequeue( g_pQueueTransmit, &uiData ) )
        {
            HWREG( UART0_BASE + UART_O_DR ) = uiData;
        }
    }
}

//----------------------------------------------------------------------------
// FUNCTION : UART_SendMessage( uint8_t* sMessage )
// PURPOSE  : Send a message using the UART
//----------------------------------------------------------------------------

void UART_SendMessage( uint8_t* sMessage )
{
    // Enqueue entire message using the UART transmit queue
    do
    {
        QUEUE_Enqueue( g_pQueueTransmit, *sMessage );
    } while( *++sMessage );

    // Attempt to transmit immediately
    UART_TransmitFromQueue();
}

//----------------------------------------------------------------------------
// FUNCTION : UART_Init( void )
// PURPOSE  : Initialize the UART interface
//----------------------------------------------------------------------------

void UART_Init( void )
{
    // UART 0 Run Mode Clock Gating Control
    HWREG( SYSCTL_RCGCUART ) |= 0x01;

    // GPIO Port A Run Mode Clock Gating Control (Port A)
    HWREG( SYSCTL_RCGCGPIO ) |= 0x01;

    // GPIO Port A Digital Enable (UART U0Rx/U0Tx = PA0/PA1)
    HWREG( GPIO_PORTA_BASE + GPIO_O_DEN ) |= 0x03;

    // GPIO Port A Alternate Function Select (UART U0Rx/U0Tx = PA0/PA1)
    HWREG( GPIO_PORTA_BASE + GPIO_O_AFSEL ) |= 0x03;

    // GPIO Port A Port Control (assign as U0Rx and U0Tx)
    HWREG( GPIO_PORTA_BASE + GPIO_O_PCTL ) = ( 1 << 0 ) | ( 1 << 4 );

    // Initialize control (default)
    HWREG( UART0_BASE + UART_O_CTL ) = 0;

    // Compute the fractional baud rate divisor
    uint32_t uiDivisor = ( ( ( UART_CLOCK * 8 ) / UART_BAUDRATE ) + 1 ) / 2;

    // Set the baud rate
    HWREG( UART0_BASE + UART_O_IBRD ) = uiDivisor / 64;
    HWREG( UART0_BASE + UART_O_FBRD ) = ( uiDivisor % 64 ) + 0.5;

    // Set parity, data length, and number of stop bits
    HWREG( UART0_BASE + UART_O_LCRH ) = ( 0x03 << 5 );

    // Clear the flags register
    HWREG( UART0_BASE + UART_O_FR ) = 0;

    // Enable the UART
    // Bit 9: RXE
    // Bit 8: TXE
    // Bit 0: UARTEN
    HWREG( UART0_BASE + UART_O_CTL ) = ( ( 1 << 9 ) | ( 1 << 8 ) | ( 1 << 0 ) );

    // UART Interrupt Mask
    // Bit 5: TXIM
    // Bit 4: RXIM
    HWREG( UART0_BASE + UART_O_IM ) = ( ( 1 << 5 ) | ( 1 << 4 ) );

    // Enable Interrupts for UART0
    HWREG( NVIC_EN0 ) = ( 1 << 5 );

    // Create a transmit queue
    g_pQueueTransmit = QUEUE_Create( 1024 );

    return;
}

//----------------------------------------------------------------------------
// END UART.C
//----------------------------------------------------------------------------
