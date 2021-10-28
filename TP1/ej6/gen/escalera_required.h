/** Generated by YAKINDU Statechart Tools code generator. */

#ifndef ESCALERA_REQUIRED_H_
#define ESCALERA_REQUIRED_H_

#include "../../../src/sc_types.h"
#include "escalera.h"

#ifdef __cplusplus
extern "C"
{
#endif 

/*! \file This header defines prototypes for all functions that are required by the state machine implementation.

This is a state machine uses time events which require access to a timing service. Thus the function prototypes:
	- escalera_set_timer and
	- escalera_unset_timer
are defined.

This state machine makes use of operations declared in the state machines interface or internal scopes. Thus the function prototypes:
	- escalera_opVel
	- escalera_opToggleLED
are defined.

These functions will be called during a 'run to completion step' (runCycle) of the statechart. 
There are some constraints that have to be considered for the implementation of these functions:
	- never call the statechart API functions from within these functions.
	- make sure that the execution time is as short as possible.
 
*/
extern void escalera_opVel( Escalera* handle, const sc_integer velocity);
extern void escalera_opToggleLED( Escalera* handle, const sc_integer n_led, const sc_boolean state_led);



/*!
 * This is a timed state machine that requires timer services
 */ 

/*! This function has to set up timers for the time events that are required by the state machine. */
/*! 
	This function will be called for each time event that is relevant for a state when a state will be entered.
	\param evid An unique identifier of the event.
	\time_ms The time in milliseconds
	\periodic Indicates the the time event must be raised periodically until the timer is unset 
*/
extern void escalera_set_timer(Escalera* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic);

/*! This function has to unset timers for the time events that are required by the state machine. */
/*! 
	This function will be called for each time event that is relevant for a state when a state will be left.
	\param evid An unique identifier of the event.
*/
extern void escalera_unset_timer(Escalera* handle, const sc_eventid evid);



#ifdef __cplusplus
}
#endif 

#endif /* ESCALERA_REQUIRED_H_ */
