/*==================[inclusions]=============================================*/

#include "../inc/main.h"

#include "../gen/ej5.h"
#include "../inc/TimerTicks.h"
#include "sapi.h"       // <= sAPI header


/*==================[macros and definitions]=================================*/

#define TICKRATE_1MS	(1)				/* 1000 ticks per second */
#define TICKRATE_MS		(TICKRATE_1MS)	/* 1000 ticks per second */


/*==================[internal data declaration]==============================*/

volatile bool SysTick_Time_Flag = false; // @suppress("Type cannot be resolved")

/*! State machine definition */
static Ej5 statechart;

/*! This is a timed state machine that requires timer services */
#define NOF_TIMERS (sizeof(Ej5TimeEvents)/sizeof(sc_boolean))

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
void ej5_opMotor( Ej5* handle, sc_boolean Action, sc_boolean state)
{
	if(Action == EJ5_EJ5IFACE_OPEN && state == EJ5_EJ5IFACE_ON) {
		gpioWrite((LED1),EJ5_EJ5IFACE_ON); // @suppress("Symbol is not resolved")
		gpioWrite((LED2),EJ5_EJ5IFACE_OFF); // @suppress("Symbol is not resolved")
		gpioWrite((LED3),EJ5_EJ5IFACE_OFF); // @suppress("Symbol is not resolved")
	}
	else if(Action == EJ5_EJ5IFACE_CLOSE && state == EJ5_EJ5IFACE_ON) {
		gpioWrite((LED1),EJ5_EJ5IFACE_OFF); // @suppress("Symbol is not resolved")
		gpioWrite((LED2),EJ5_EJ5IFACE_ON); // @suppress("Symbol is not resolved")
		gpioWrite((LED3),EJ5_EJ5IFACE_OFF); // @suppress("Symbol is not resolved")
	}
	else if(state == EJ5_EJ5IFACE_OFF) {
		gpioWrite((LED1),EJ5_EJ5IFACE_OFF); // @suppress("Symbol is not resolved")
		gpioWrite((LED2),EJ5_EJ5IFACE_OFF); // @suppress("Symbol is not resolved")
		gpioWrite((LED3),EJ5_EJ5IFACE_ON); // @suppress("Symbol is not resolved")
	}

}

void ej5_opToggleLED( Ej5* handle, sc_integer n_LED)
{
	if(gpioRead(LEDR+n_LED) == 0) // @suppress("Symbol is not resolved")
		gpioWrite(LEDR+n_LED, EJ5_EJ5IFACE_ON); // @suppress("Symbol is not resolved")
	else
		gpioWrite(LEDR+n_LED, EJ5_EJ5IFACE_OFF); // @suppress("Symbol is not resolved")
}

void ej5_opLED( Ej5* handle, sc_integer n_LED, sc_boolean state){
	gpioWrite(LEDR+n_LED, state); // @suppress("Symbol is not resolved")
}

/* Timer functions
*/
void ej5_set_timer( Ej5* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic )
{
	SetNewTimerTick(ticks, NOF_TIMERS, evid, time_ms, periodic);
}

void ej5_unset_timer( Ej5* handle, const sc_eventid evid )
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
	ej5_init( &statechart );
	ej5_enter( &statechart );

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
					ej5_raise_time_event( &statechart, ticks[i].evid );

					/* Then Mark as Attached -> Ticks.evid => OK */
					MarkAsAttEvent( ticks, NOF_TIMERS, ticks[i].evid );
				}
			}

			/* Then Get status of buttons, usage of ..._old variable to prevent from pressing more
			 * than one time */
			BUTTON_Status_old = BUTTON_Status;
			BUTTON_Status = Buttons_GetStatus_();

			/* Then if there is a pressed button, raise respective event */
			if (BUTTON_Status == 1 && BUTTON_Status_old == 0)
				ej5_raise_evRemotoAbrir(&statechart);
			else if (BUTTON_Status == 2 && BUTTON_Status_old == 0)
				ej5_raise_evRemotoCerrar(&statechart);
			else if (BUTTON_Status == 4 && BUTTON_Status_old == 0)
				ej5_raise_evFinCarrera(&statechart);
			else if (BUTTON_Status == 8 && BUTTON_Status_old == 0)
				ej5_raise_evEmergencia(&statechart);
		}
	}
}


/** @} doxygen end group definition */

/*==================[end of file]============================================*/
