/**
 * Arduino tiny and cross-device compatible timer library.
 *
 * Timers used by microcontroller
 *	Atmel AVR:	Timer2 (3rd timer)
 *  STM32:		Timer3 (3rd timer)
 *  SAM (Due):  TC3 (Timer1, channel 0)
 *  ESP8266:	OS Timer, one slof of seven available (Software timer provided by Arduino because ESP8266 has only two hardware timers and one is needed by it normal operation)
 *  SAMD21:		Timer2 (3rd Timer. Used because there're 3 a 5 timer versions). See http://ww1.microchip.com/downloads/en/DeviceDoc/40001882A.pdf
 *
 * @copyright Naguissa
 * @author Naguissa
 * @email naguissa@foroelectro.net
 * @version 1.0.0
 * @created 2018-01-27
 */
#ifndef _uTimerLib_
	#define _uTimerLib_

	#include "Arduino.h"

	#ifdef ARDUINO_ARCH_ESP8266
		#include <Ticker.h>  //Ticker Library
	#endif
	// Operation modes
	#define UTIMERLIB_TYPE_OFF 0
	#define UTIMERLIB_TYPE_TIMEOUT 1
	#define UTIMERLIB_TYPE_INTERVAL 2

	#ifdef _VARIANT_ARDUINO_STM32_
		#include "HardwareTimer.h"
		extern HardwareTimer Timer3;
	#endif

	class uTimerLib {
		public:
			uTimerLib();
			void setInterval_us(void (*) (), unsigned long int);
			void setInterval_s(void (*) (), unsigned long int);
			int setTimeout_us(void (*) (), unsigned long int);
			int setTimeout_s(void (*) (), unsigned long int);
			void clearTimer();
			void _interrupt();

			#ifdef _VARIANT_ARDUINO_STM32_
				static void interrupt();
			#endif

			#ifdef ARDUINO_ARCH_ESP8266
				#pragma message "ESP8266 can only reach a ms resolution so any ms interrupt will be rounded to that"
				static void interrupt();
			#endif

		private:
			static uTimerLib *_instance;

			unsigned long int _overflows = 0;
			unsigned long int __overflows = 0;
			#ifdef ARDUINO_ARCH_AVR
				unsigned char _remaining = 0;
				unsigned char __remaining = 0;
			#else
				unsigned long int _remaining = 0;
				unsigned long int __remaining = 0;
			#endif
			void (*_cb)() = NULL;
			unsigned char _type = UTIMERLIB_TYPE_OFF;

			void _loadRemaining();

			void _attachInterrupt_us(unsigned long int);
			void _attachInterrupt_s(unsigned long int);

			#ifdef _VARIANT_ARDUINO_STM32_
				bool _toInit = true;
			#endif

			#ifdef ARDUINO_ARCH_ESP8266
				Ticker _ticker;
			#endif

	};

	extern uTimerLib TimerLib;

#endif

