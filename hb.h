//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : HB.H
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
// INCLUSION LOCK
//----------------------------------------------------------------------------

#ifndef HB_H_
#define HB_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void HB_Sequencer( bool bReset );

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define HB_DELAY_HB_ON   250  // LED on-time for heartbeat pulse
#define HB_DELAY_HB_OFF  750  // LED off-time for heartbeat pulse

#define HB_DELAY_PAUSE1  1980 // LED Pause1 time for heart-beat pulse
#define HB_DELAY_PAUSE2  2375 // LED Pause2 time for heart-beat pulse
#define HB_DELAY_FP_ON   125  // LED FP_ON time for heart-beat pulse
#define HB_DELAY_FP_OFF  375  // LED FP_OFF time for heart-beat pulse

// Heartbeat States
enum HB_STATE
{
    HB_STATE_PAUSE1 = 0,
    HB_STATE_FP_ON,
    HB_STATE_FP_OFF,
    HB_STATE_HB_OFF,
    HB_STATE_HB_ON,
    HB_STATE_PAUSE2


};

#endif // HB_H_

//----------------------------------------------------------------------------
// END HB.H
//----------------------------------------------------------------------------
