/*
 * Copyright 2012 Adam Cooper, based heavily on the work of Elco Jacobs.
 * See http://www.elcojacobs.com/easy-to-use-atmel-studio-project-for-arduino-and-programming-the-arduino-from-python/
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ShiftPWM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

/* This is the main routine and library loader to allow arduino sketches and the arduino core libraries to be easily used from
 * within Atmel Studio 6. This version is for "standard" arduinos such as the Uno.
 * In general use, leave main.cpp unchanged except for including necessary core libraries and use sketch.cpp for your arduino sketch
 *
 * You also need to modify Atmel Studio 6 project properties to point it to the right directories for the arduino libraries.
 * To compile this project on your computer, take the following steps:
 * 1. Go the the project properties and change the following options:
 *		In AVR/GNU C++ Compiler --> Directories.
 *			Change the two directories to point to your local Arduino installation. For the second directory, choose the Arduino variant that you are using.
 *			Also add any Arduino libraries that you are using. My directories are:
 *			D:\arduino-1.0.1\hardware\arduino\cores\arduino
 *			D:\arduino-1.0.1\hardware\arduino\variants\standard (for Uno) or ..\leonardo
 *			D:\arduino-1.0.1\libraries\{library-name} <<< add 0 or more, substituting {library-name} accordingly
 *		If you are using an 8 MHz Arduino, also change F_CPU in AVR/GNU C++ Compiler --> Symbols
 * 
 * That is all that is needed! No hassle with makefiles and compiling libraries. You can find all the files that are automatically included under 'Dependencies'.
 *
 *Once all of this has been done, you can File>Export Template to create a project template in which all of these settings are preserved.
 */

#define ARDUINO_MAIN

// Disable some warnings for the Arduino files
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wuninitialized"

#include <Arduino.h>
extern "C"{
	#include <pins_arduino.h>
}


/*
* Libraries
*/

// Standard Arduino source files for serial:
#include <HardwareSerial.cpp>

// Other source files, depends on your program which you need
#include <Print.cpp>
#include <New.cpp>
#include <wiring.c>
#include <wiring_digital.c>
#include <wiring_analog.c> //analog read/write functions
#include <WString.cpp>
#include <WMath.cpp>
#include <Stream.cpp>

// Unused source files:
//#include <WInterrupts.c>
//#include <wiring_pulse.c>
//#include <wiring_shift.c>
//#include <IPAddress.cpp>

//#include <Tone.cpp>

// Restore original warnings configuration
#pragma GCC diagnostic pop

int main(void)
{
	init();

	setup();
	
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
	return 0;
}
