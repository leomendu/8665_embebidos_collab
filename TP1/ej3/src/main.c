/*==================[inclusions]=============================================*/

#include "../inc/main.h"

#include "../gen/ej3.h"
#include "../inc/TimerTicks.h"
#include "sapi.h"       // <= sAPI header


/*==================[macros and definitions]=================================*/

#define TICKRATE_1MS	(1)				/* 1000 ticks per second */
#define TICKRATE_MS		(TICKRATE_1MS)	/* 1000 ticks per second */


/*==================[internal data declaration]==============================*/

volatile bool SysTick_Time_Flag = false; // @suppress("Type cannot be resolved")

/*! State machine definition */
static Ej3 statechart;

/*! This is a timed state machine that requires timer services */
#define NOF_TIMERS (sizeof(Ej3TimeEvents)/sizeof(sc_boolean))

TimerTicks ticks[NOF_TIMERS];


/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*! \file This header defines prototypes for all functions that are required
 *  by the state machine implementation.
 *
 *  This is a state machine uses time events which require access to a timing
 *  service. Thus the function prototypes:
 *  - blink_setTimer and
 *  - blink_unsetTimer
 *  are defined.
 *
 *  This state machine makes use of operations declared in the state machines
 *  interface or internal scopes. Thus the function prototypes:
 *  - blinkIface_opLED
 *  are defined.
 *
 *  These functions will be called during a 'run to completion step' (runCycle)
 *  of the statechart.
 *  There are some constraints that have to be considered for the
 *  implementation of these functions:
 *  - never call the statechart API functions from within these functions.
 *  - make sure that the execution time is as short as possible.
 */

/** state machine user-defined external function (action)
 *
 * @param handle state machine instance
 * @param LEDNumber number of LED
 * @param onoff state machine operation parameter
 */
void ej3_opSetForma( Ej3* handle, sc_integer Number)
{
	gpioWrite((LEDR+Number),bool_true); // @suppress("Symbol is not resolved")
}

void ej3_opSetMag( Ej3* handle, sc_integer Number)
{
	if(Number == EJ3_EJ3IFACE_V)
		gpioWrite(LED1,bool_false);
	else
		gpioWrite(LED1,bool_true);
}

void ej3_opChangeMag( Ej3* handle, sc_integer MAG, sc_integer CHG)
{
	if(CHG == EJ3_EJ3IFACE_INC && gpioRead(LED2) == 0)
		gpioWrite(LED2, bool_true);
	else if(CHG == EJ3_EJ3IFACE_INC && gpioRead(LED2) == 1)
		gpioWrite(LED2, bool_false);
	else if(CHG == EJ3_EJ3IFACE_DEC && gpioRead(LED3) == 0)
		gpioWrite(LED3, bool_true);
	else if(CHG == EJ3_EJ3IFACE_DEC && gpioRead(LED3) == 1)
		gpioWrite(LED3, bool_false);
}

/* Timer functions
*/
void ej3_set_timer( Ej3* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic )
{
	SetNewTimerTick(ticks, NOF_TIMERS, evid, time_ms, periodic);
}

void ej3_unset_timer( Ej3* handle, const sc_eventid evid )
{
	UnsetTimerTick( ticks, NOF_TIMERS, evid );
}

/* Hook on Handle interrupt from SysTick timer
*/
void myTickHook( void *ptr ){

	/* The sysTick Interrupt Handler only sets a Flag */
	SysTick_Time_Flag = true;
}

/*  This function scans all EDU-CIAA-NXP buttons (TEC1, TEC2, TEC3 and TEC4),
 *  and return ID of pressed button (0001 for TEC1, 0010 for TEC2, 0101 for TEC3 and TEC1
 *  at the same time, 0000 if no button is pressed.
*/
uint32_t Buttons_GetStatus_(void) { // @suppress("Type cannot be resolved")
	uint8_t ret = false; // @suppress("Type cannot be resolved")
	uint32_t idx; // @suppress("Type cannot be resolved")

	for (idx = 0; idx < 4; ++idx) {
		if (gpioRead( TEC1 + idx ) == 0) // @suppress("Symbol is not resolved")
			ret |= 1 << idx;
	}
	return ret;
}



int main(void)
{
	uint32_t i; // @suppress("Type cannot be resolved")

	uint32_t BUTTON_Status = 0; // @suppress("Type cannot be resolved")

	uint32_t BUTTON_Status_old; // @suppress("Type cannot be resolved")

	/* Generic initialization */
	boardConfig();

	/* Init Ticks counter => TICKRATE_MS */
	tickConfig( TICKRATE_MS );

	/* Add Tick Hook */
	tickCallbackSet( myTickHook, (void*)NULL );

	/* Init Timer Ticks */
	InitTimerTicks( ticks, NOF_TIMERS );

	/* Statechart Initialization */
	ej3_init( &statechart );
	ej3_enter( &statechart );

	/* LED state is toggled in the main program */
	while (1) {
		/* The uC sleeps waiting for an interruption */
		__WFI();

		/* When a interrupt wakes to the uC, the main program validates it,
		 * checking the waited Flag */
		if (SysTick_Time_Flag == true) {

			/* Then reset its Flag */
			SysTick_Time_Flag = false;

			/* Then Update all Timer Ticks */
			UpdateTimers( ticks, NOF_TIMERS );

			/* Then Scan all Timer Ticks */
			for (i = 0; i < NOF_TIMERS; i++) {

				/* Then if there are pending events */
				if (IsPendEvent( ticks, NOF_TIMERS, ticks[i].evid ) == true) {

					/* Then Raise an Event -> Ticks.evid => OK */
					ej3_raise_time_event( &statechart, ticks[i].evid );

					/* Then Mark as Attached -> Ticks.evid => OK */
					MarkAsAttEvent( ticks, NOF_TIMERS, ticks[i].evid );
				}
			}

			/* Then Get status of buttons */
			BUTTON_Status_old = BUTTON_Status;
			BUTTON_Status = Buttons_GetStatus_();

			/* Then if there is a pressed button, raise respective event */
			if (BUTTON_Status == 1 && BUTTON_Status_old == 0)
				ej3_raise_evForma(&statechart);
			else if (BUTTON_Status == 2 && BUTTON_Status_old == 0)
				ej3_raise_evCambiarMag(&statechart);
			else if (BUTTON_Status == 4 && BUTTON_Status_old == 0)
				ej3_raise_evDown(&statechart);
			else if (BUTTON_Status == 8  && BUTTON_Status_old == 0)
				ej3_raise_evUp(&statechart);
		}
	}
}


/** @} doxygen end group definition */

/*==================[end of file]============================================*/
