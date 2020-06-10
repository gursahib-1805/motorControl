//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : QUEUE.C
// FILE VERSION : 1.0
// PROGRAMMER   : Gursahib Singh
//----------------------------------------------------------------------------
// REVISION HISTORY
//----------------------------------------------------------------------------
//
// 1.0, 2019-NOV-01, Gursahib Singh
//   - Initial release
//
#include "queue.h"
#include <stdlib.h>

//----------------------------------------------------------------------------
// EXTERNAL REFERENCES
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// FUNCTION : QUEUE _Create( uint32_t bfrsize )
// PURPOSE : Create a queue
//----------------------------------------------------------------------------


QUEUE* QUEUE_Create ( uint32_t bfrsize )

{

    uint32_t bytes;
    QUEUE *q;

    bytes = ( sizeof ( QUEUE ) - 1 ) + bfrsize;
    q = ( QUEUE *)malloc( bytes );

    if ( q != NULL )
    {
        q->nq = q->dq = q->count = 0;
        q->size = bfrsize;
    }

    return q;
}

//----------------------------------------------------------------------------
// FUNCTION : QUEUE _Enqueue( QUEUE *q, uint8_t data )
// PURPOSE : Inserts one byte into the queue
//----------------------------------------------------------------------------

bool QUEUE_Enqueue( QUEUE *q, uint8_t data )
{
  bool full;

  asm( " cpsid i" );
  full = q->count == q->size;
  if( !full )
  {
      q->bfr[ q->nq ] = data;
      if( ++q->nq == q->size )
      {
          q->nq = 0;
      }
      q->count++;
  }
  asm( " cpsie i");

  return !full;
}

//----------------------------------------------------------------------------
// FUNCTION : QUEUE _Dequeue( QUEUE *q, uint8_t *ptr2data )
// PURPOSE : Removes one byte into the queue
//----------------------------------------------------------------------------

bool QUEUE_Dequeue( QUEUE *q, uint8_t *ptr2data )
{
  bool empty;

  asm( " cpsid i" );
  empty = q->count == 0;
  if( !empty )
  {
      *ptr2data = q->bfr[ q->dq ];
      if( ++q->dq == q->size )
      {
          q->dq = 0;
      }
      q->count--;
  }
  asm( " cpsie i");

  return !empty;
}
  //----------------------------------------------------------------------------
  // END QUEUE.C
  //----------------------------------------------------------------------------
