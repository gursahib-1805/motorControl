//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : CONTACT.C
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
// Provides contact debounce interfacing.
//
//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include "contact.h"

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FUNCTION : CONTACT_Init( CONTACT *pContact, uint32_t uiWindow )
// PURPOSE  : Initializes the contact structure
//----------------------------------------------------------------------------

void CONTACT_Init( CONTACT *pContact, uint32_t uiWindow )
{
    pContact->bCurrentState  = 0;
    pContact->bPreviousState = 0;

    pContact->uiCount  = 0;
    pContact->uiWindow = uiWindow;

    pContact->bEvent      = 0;
    pContact->bEventState = 0;

    pContact->bReserved = 0;

    return;
}

//----------------------------------------------------------------------------
// FUNCTION : CONTACT_Sample( CONTACT *pContact, bool bContactState )
// PURPOSE  : Samples the state of a contact and returns the event status
//----------------------------------------------------------------------------

bool CONTACT_Sample( CONTACT *pContact, bool bContactState )
{
	pContact->bCurrentState = bContactState ? 1 : 0;

    if( pContact->bCurrentState ^ pContact->bPreviousState )
    {
        // Change detected from previous sample
    	pContact->uiCount = pContact->uiWindow;
    }
    else
    {
        // No change from previous sample
    	if( pContact->uiCount )
    	{
    		pContact->uiCount--;
    	}
    }

    // Prepare for next sample
    pContact->bPreviousState = pContact->bCurrentState;

    // Indicate event and event state
	pContact->bEvent = pContact->uiCount == 1 ? 1 : 0;
	pContact->bEventState = pContact->uiCount == 1 ?
			                pContact->bCurrentState :
							pContact->bEventState;

    return pContact->bEvent;
}

//----------------------------------------------------------------------------
// END CONTACT.C
//----------------------------------------------------------------------------
