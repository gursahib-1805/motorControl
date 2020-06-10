//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : HB.C
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
// Generates the heartbeat sequencing for the system. The HB_Sequencer()
// function must be called once every millisecond.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "hb.h"
#include "led.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------
extern uint8_t Sequencer_Pulses;

//----------------------------------------------------------------------------
// FUNCTION : HB_Sequencer( bool bReset )
// PURPOSE  : Sequences the heartbeat LED
//----------------------------------------------------------------------------

void HB_Sequencer( bool bReset )
{
    // State tracking
    static uint8_t uiHBState = HB_STATE_PAUSE1;

    //Variable for FP
    static uint8_t uiFPCount = 0;

    // Timer for LED duration and pauses
    static uint16_t uiTimer = 0;

    // Reset all internal variables to known values
    if( bReset )
    {
        uiHBState = HB_STATE_PAUSE1;
        uiTimer = 0;
    }

    // State sequencer
    switch( uiHBState )
    {

    case HB_STATE_PAUSE1:
        if( uiTimer == 0 )
        {
            LED_Control( 0 );
        }
        if( ++uiTimer >= HB_DELAY_PAUSE1 )
        {
            uiHBState = HB_STATE_FP_ON;
            uiTimer = 0;
        }
        break;

    case HB_STATE_FP_ON:
        if( uiTimer == 0 )
        {
            LED_Control( 1 );
        }
        if( ++uiTimer >= HB_DELAY_FP_ON )
        {
            if(++uiFPCount >= Sequencer_Pulses)
                uiHBState = HB_STATE_PAUSE2;
            else
                uiHBState = HB_STATE_FP_OFF;
            uiTimer = 0;
        }
        break;

    case HB_STATE_FP_OFF:
            if( uiTimer == 0 )
            {
                LED_Control( 0 );
            }

            if( ++uiTimer >= HB_DELAY_FP_OFF )
            {
                uiHBState = HB_STATE_FP_ON;
                uiTimer = 0;
            }
            break;

    case HB_STATE_PAUSE2:
                if( uiTimer == 0 )
                {
                    LED_Control( 0 );
                }
                if( ++uiTimer >= HB_DELAY_PAUSE2 )
                {
                    uiHBState = HB_STATE_HB_ON;
                    uiTimer = 0;
                }
                break;

    case HB_STATE_HB_ON:
                    if( uiTimer == 0 )
                    {
                        LED_Control( 1 );
                    }
                    if( ++uiTimer >= HB_DELAY_HB_ON )
                    {
                        uiHBState = HB_STATE_HB_OFF;
                        uiTimer = 0;
                    }
                    break;

    case HB_STATE_HB_OFF:
                        if( uiTimer == 0 )
                        {
                            LED_Control( 0 );
                        }
                        if( ++uiTimer >= HB_DELAY_HB_OFF )
                        {
                            uiHBState = HB_STATE_HB_ON;
                            uiTimer = 0;
                        }
                        break;

    default:
        uiHBState = HB_STATE_HB_ON;
        LED_Control( 0 );
        break;

    }

    return;
}

//----------------------------------------------------------------------------
// END HB.C
//----------------------------------------------------------------------------
