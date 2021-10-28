/*==================[inclusions]=============================================*/
#include "main.h"
#include "../gen/ej7.h"
#include "sapi.h"
#include "TimerTicks.h"
  // <= sAPI header

/* Include statechart header file. Be sure you run the statechart C code
 * generation tool!
 */


/*==================[macros and definitions]=================================*/

#define TICKRATE_1MS	(1)				/* 1000 ticks per second */
#define TICKRATE_MS		(TICKRATE_1MS)	/* 1000 ticks per second */


/*==================[internal data declaration]==============================*/

volatile bool SysTick_Time_Flag = false;

/*! This is a state machine */
static Ej7 statechart;

/*! This is a timed state machine that requires timer services */
#define NOF_TIMERS (sizeof(Ej7TimeEvents)/sizeof(sc_boolean))

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
 *  - buttons_setTimer and
 *  - buttons_unsetTimer
 *  are defined.
 *
 *  This state machine makes use of operations declared in the state machines
 *  interface or internal scopes. Thus the function prototypes:
 *  - buttonsIface_opLED
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
void ej7_opLED( Ej7* handle, sc_integer LED_NUMBER, sc_boolean status){
	gpioWrite((LEDR + LED_NUMBER), status);
}

void ej7_opHorno(Ej7* handle, sc_integer modoCoccion){
	if (modoCoccion == 0){
		gpioWrite(LEDR, EJ7_EJ7IFACE_LED_ON);
		gpioWrite(LEDG, EJ7_EJ7IFACE_LED_OFF);/* LED_HORNO, CHEQUEAR NOMBRE LED_OFF */
	}
	else{
		gpioWrite(LEDR, EJ7_EJ7IFACE_LED_OFF);
		gpioWrite(LEDG, EJ7_EJ7IFACE_LED_ON); /* CHEQUEAR NOMBRE LED_ON */
	}
}
/*! * This is a timed state machine that requires timer services
 */

/*! This function has to set up timers for the time events that are required
 *  by the state machine.
 */
/*! This function will be called for each time event that is relevant for a
 *  state when a state will be entered.
 *  \param evid An unique identifier of the event.
 *  \time_ms The time in milli seconds
 *  \periodic Indicates the the time event must be raised periodically until
 *   the timer is unset
 */
void ej7_set_timer( Ej7* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic )
{
	SetNewTimerTick(ticks, NOF_TIMERS, evid, time_ms, periodic);
}

/*! This function has to unset timers for the time events that are required
 *  by the state machine.
 */
/*! This function will be called for each time event that is relevant for a
 *  state when a state will be left.
 *  \param evid An unique identifier of the event.
 */
void ej7_unset_timer( Ej7* handle, const sc_eventid evid )
{
	UnsetTimerTick( ticks, NOF_TIMERS, evid );
}


/**
 * @brief	Hook on Handle interrupt from SysTick timer
 * @return	Nothing
 */
void myTickHook( void *ptr ){

	/* The sysTick Interrupt Handler only set a Flag */
	SysTick_Time_Flag = true;
}


/*! This function scan all EDU-CIAA-NXP buttons (TEC1, TEC2, TEC3 and TEC4),
 *  and return ID of pressed button (TEC1 or TEC2 or TEC3 or TEC4)
 *  or false if no button was pressed.
 */
uint32_t Buttons_GetStatus_(void) {
	uint8_t ret = false;
	uint32_t idx;

	for (idx = 0; idx < 4; ++idx) {
		if (gpioRead( TEC1 + idx ) == 0)
			ret |= 1 << idx;
	}
	return ret;
}


/**
 * @brief	main routine for statechart example: EDU-CIAA-NXP - Buttons
 * @return	Function should not exit.
 */
int main(void)
{
	uint32_t i;

	uint32_t BUTTON_Status;

	/* Generic Initialization */
	boardConfig();

	/* Init Ticks counter => TICKRATE_MS */
	tickConfig( TICKRATE_MS );

	/* Add Tick Hook */
	tickCallbackSet( myTickHook, (void*)NULL );

	/* Init Timer Ticks */
	InitTimerTicks( ticks, NOF_TIMERS );

	/* Statechart Initialization */
	ej7_init( &statechart );
	ej7_enter( &statechart );

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
					ej7_raise_time_event( &statechart, ticks[i].evid );

					/* Then Mark as Attached -> Ticks.evid => OK */
					MarkAsAttEvent( ticks, NOF_TIMERS, ticks[i].evid );
				}
			}

			/* Then Get status of buttons */

			BUTTON_Status = Buttons_GetStatus_();

			if (BUTTON_Status == 1)
				ej7_raise_evModoC(&statechart);
			else if (BUTTON_Status == 2)
				ej7_raise_evPuertaAbierta(&statechart);
			else if (BUTTON_Status == 4)
				ej7_raise_evPuertaCerrada(&statechart);
			else if (BUTTON_Status == 8)
				ej7_raise_evComenzarTerminar(&statechart);
			else if (BUTTON_Status == 0){
				ej7_raise_evTECXoff(&statechart);

			}
		}
	}
}


/*==================[end of file]============================================*/
