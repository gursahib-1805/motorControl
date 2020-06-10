//----------------------------------------------------------------------------
// COMPANY      : Confederation College
// FILE         : MOTOR.H
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

#ifndef MOTOR_H_
#define MOTOR_H_

//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>


//----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------------------------

void MOTOR_Init( void );
void MOTOR_SetDutyCycle( float fMotorSpeed, bool bMotorDir );
float incrementRPM( float *motorRPM, float incrementValue );
float decrementRPM( float *motorRPM, float decrementValue );

#endif // MOTOR_H_

//----------------------------------------------------------------------------
// END MOTOR.H
//----------------------------------------------------------------------------
