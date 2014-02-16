/*
* AstroLib.h
*
* Created: 2/8/2014 9:58:56 PM
*  Author: Adam
*/
/*
* ***Made available using the The MIT License (MIT)***
* Copyright (c) 2014, Adam
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the �Software�), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. */

#ifndef ASTROLIB_H
#define ASTROLIB_H

#include <Arduino.h>

/*! Doxygen documentation about class AstroLib
@brief ... brief desc of AstroLib. */
class AstroLib{
	public:
	/** @name Constructors */
	//!@{
	/*! Basic constructor. */
	//AstroLib();
	//!@}
	
	#define R2D 57.29578//rads to degrees, multiply rad measure by R2D
	#define R2H 3.819719//rads to decimal hours
	#define D2R 0.01745329 //degrees to rads
	#define H2R 0.2617994 //decimal hours to rads
	#define TWOPI 6.283185 //2*PI
	
	//arguments are year,month,date,hour minute
	//returns the Julian Date as a decimal of days since mean noon, Jan 1st 4713BC
	//Julian Dates change at midday (i.e. the truncated integer changes)
	//only valid for Gregorian calendar dates i.e since 1582-10-15
	// ** independent of epoch
	//returns the truncated integer no of days
	//microDays returns 1000000*the decimal part
	static long calcJulianDate(long Year,long Mon, long Date,long Hour,long Min, long& microDays);
	
	// arguments are date-time array from getFormDT(), longitude (pass 0.0 for Greenwich ST)
	// returns the decimal siderial time accuracy is "within a few minutes" (some testing indicates <1min is common but ~2mins is possible)
	static float calcSiderial(long Year, long Month, long Date, long Hour, long Min, float longitude);
	
	//convert ecliptic longitude (lambda) and lat (beta) to equatorial.
	// Arguments and results  in RADIAN
	static void ecliptic2equatorial(float lambda, float beta, float& alpha, float& delta);
	
	//convert equatorial (arguments in RADIAN measure) to altitude/azimuth for an observer at the given
	//latitude (RADIAN).
	//the result is in RADIAN inless scaleResult is true, in which case degrees are returned
	static void  equatorial2horizontal(float hourAngle, float declination, float latitude, float& altitude, float& azimuth);

	// MOON 00000000000000000000000000000000000000
	//calculate the sun's geocentric ecliptic longitude (RADIAN), mean anomaly and equation of centre
	//returns the ecliptic longitude with mean anomaly and equation of centre as ref arguments
	//Practical Astronomy with your Calculator. PRACTICAL ASTRONOMY WITH EXCEL section 46
	//********* converted to radian measure 2014-02-15
	static float calcSun(long Year, long Month, long Date, float& M, float& Ec);
	// calculate the position of the moon for a given date-time (RADIAN angles)
	//returns the lunar ecliptic longitude with ecliptic latitude (beta), mean anomaly (M), and lunar age as ref arguments
	//Practical Astronomy with your Calculator. PRACTICAL ASTRONOMY WITH EXCEL section 64
	static float calcMoon(long Year, long Month, long Date, long Hour, long Min, float& beta_moon, float& M_moon, float& age);

		private:
		//private member variables
		//private methods
	};



	#endif //include guard