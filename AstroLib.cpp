/*
* AstroLib.cpp
*
* Created: 2/8/2014 9:58:56 PM
* Author: Adam
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

#include <avr/io.h>

#include "AstroLib.h"

long AstroLib::calcJulianDate(long Year, long Mon, long Date, long Hour, long Min, long& microDays){
	if(Mon<3){
		Mon+=12;
		Year-=1;
	}
	long A=Year/100;
	long B=2-A+A/4;
	long C=(long)(365.25 * float(Year));
	long D=(long)(30.6001*float(Mon+1L));
	long JD=(B+C+D+Date)+1720994L;
	microDays=500000L + 41667L*(long)Hour + 694L*(long)Min;
	if(microDays>1000000){//microDays will be >=1000000 after noon UT
		microDays-=1000000;
		JD++;
	}
	return JD;
}

float AstroLib::calcSiderial(long Year, long Month, long Date, long Hour, long Min, float longitude){
	//date at 0h. there will be a 0.5 too, but it is neglected as insignificant when calc variable T.
	
	long microDays;
	long JDi=AstroLib::calcJulianDate(Year, Month, Date, 0L, 0L, microDays);
	float S = (float)(JDi-2451545L);
	float T=S/36525.0;
	float T0=6.697374 + 2400.051336*T + 0.000025862*T*T;
	
	float decT=float(Hour)+((float)(Min))/60.0;
	float SiderialTime=T0 + decT + decT*0.002737909;
	while(SiderialTime<0.0) SiderialTime+=24.0;
	while(SiderialTime>24.0) SiderialTime-=24.0;
	
	//to local time
	SiderialTime+=(longitude/15.0);
	if(SiderialTime<0.0) SiderialTime+=24.0;
	if(SiderialTime>24.0) SiderialTime-=24.0;
	return SiderialTime;
}

void AstroLib::ecliptic2equatorial(float lambda, float beta, float& alpha, float& delta){
	//orbital obliquity (assumed constant). In rads
	float epsilon = 0.408930644;//23.43 degs
	float cosep = cos(epsilon);
	float sinep = sin(epsilon);
	float sinlam= sin(lambda);
	float y = sinlam*cosep - tan(beta)*sinep;
	float x = cos(lambda);
	alpha = atan2(y,x);
	delta = asin(sin(beta)*cosep + cos(beta)*sinep*sinlam);
	if(alpha<0.0)alpha+=TWOPI;
}

//convert equatorial (arguments in RADIAN measure) to altitude/azimuth for an observer at the given
//latitude (RADIAN).
//the result is in RADIAN inless scaleResult is true, in which case degrees are returned
void AstroLib::equatorial2horizontal(float hourAngle, float declination, float latitude, float& altitude, float& azimuth){
	float sinAlt = sin(declination)* sin(latitude) + cos(declination)*cos(latitude)*cos(hourAngle);
	altitude = asin(sinAlt);
	float y = -cos(declination)*cos(latitude)*sin(hourAngle);
	float x = sin(declination) - sin(latitude)*sinAlt;
	azimuth = atan2(y,x);
	if(azimuth<0.0)azimuth+=TWOPI;
}

float AstroLib::calcSun(long Year, long Month, long Date, float& M, float& Ec){
	const long epoch=2455196;//Julian date is 2455196.5 for epoch 2010.0. 0.5 is neglected here and later too
	// constants for the sun's apparent orbit AT epoch 2010.0
	const float epsilon_g=4.879193727;//279.557208 degs, ecliptic longitude
	const float omega_g=4.941244196;//283.112438 degs, ecliptic longitude of perigee
	const float ecc=0.016705;//eccentricity
	
	long microDays;//ignored, would be 500000 (i.e. 0.5 day)
	long daysSinceEpoch=calcJulianDate(Year, Month, Date, 0L, 0L, microDays)-epoch;//0.5 in both parts would cancel
	M=TWOPI*float(daysSinceEpoch)/365.2422+epsilon_g-omega_g;//Mean sun "anomaly"
	while(M<0.0)M+=TWOPI;
	while(M>TWOPI)M-=TWOPI;
	float E_c=M+2.0*ecc*sin(M);//equation of centre ("true anomaly")
	float lambda=E_c+omega_g;
	while(lambda<0.0)lambda+=TWOPI;
	while(lambda>TWOPI)lambda-=TWOPI;
	return lambda;
}

float AstroLib::calcMoon(long Year, long Month, long Date, long Hour, long Min, float& beta_moon, float& M_moon, float& age){
	// constants for the calcs at a given epoch, including the epoch.
	const long epoch=2455196;//Julian date is 2455196.5 for epoch 2010.0. 0.5 is missed off, hard-coded later
	//degree measure
	const float l_0=1.604469592;//91.929336 degs moon mean longitude
	const float P_0=2.271425175;//130.143076 degs moon mean longitude at perigee
	const float N_0=5.090820816;//291.682547 degs mean longitude of the node
	const float incl=0.089804102;//5.145396 inclination in degrees
	const float F1 = 0.229971504; //13.1763966 degrees
	const float F2= 0.001944368;//0.1114041 degrees
	const float F3= 0.00092422;// 0.0529539 degrees
	const float F4 =0.022233749;//1.2739 degrees
	const float F5 =0.003242822;//0.1858 degrees
	const float F6= 0.006457718;//0.37 degs
	const float F7=0.109756775;//6.2886 degs
	const float F8=0.003735005;//0.214 degs
	const float F9=0.011489502;//0.6583 degs
	//const float ecc=0.0549;//eccentricity
	
	//need sun position (geogentric eclipltic coords)
	float M_sun;
	float Ec_sun;
	float lambda_sun = calcSun(Year, Month, Date, M_sun, Ec_sun);
	//need mean moon
	long microDays;
	long daysSinceEpoch_int=calcJulianDate(Year, Month, Date, Hour, Min, microDays)-epoch;
	long daysSinceEpoch_micro=microDays-500000;
	if(daysSinceEpoch_micro<0){
		daysSinceEpoch_micro+=1000000;
		daysSinceEpoch_int--;
	}
	float l_moon=F1*float(daysSinceEpoch_int) + F1*float(daysSinceEpoch_micro)*1.0E-6 +l_0;//moon mean longitude
	M_moon=l_moon-F2*daysSinceEpoch_int-P_0;//moon mean anomaly. micro days neglected cos F2 v.small
	float N_moon=N_0-F3*daysSinceEpoch_int;//ascending node mean longitude. micro days neglected cos F3 v.small
	//now corrections to moon
	float E_v=F4*sin(2.0*(l_moon-lambda_sun)-M_moon);//evection
	float sinSun=sin(M_sun);
	float A_e=F5*sinSun;
	float A_3=F6*sinSun;
	M_moon+=E_v-A_e-A_3;//interim corrected anomaly...
	float E_c=F7*sin(M_moon);//equation of centre
	//more corrections using where we've corrected to so far
	float A_4=F8*sin(2.0*M_moon);
	l_moon+=E_v+E_c-A_e+A_4;
	float V=F9*sin(2.0*(l_moon-lambda_sun));
	l_moon+=V; //l_moon is the ecliptic longitude of the moon
	//calc the moons latitude relative to the ecliptic
	float N_moon_prime = N_moon - 0.002792527*sin(M_sun); //factor is 0.16 in degrees
	//trigfunctions work in rads
	beta_moon = asin(sin(incl)*sin(l_moon-N_moon_prime));
	
	//get in range
	while(l_moon<0.0)l_moon+=TWOPI;
	while(l_moon>TWOPI)l_moon-=TWOPI;
	
	//age in rads is just due the relative position of sun and moon
	//phase = (1-cos(age))/2
	age=l_moon - lambda_sun;
	//get in range
	while(age<0.0)age+=TWOPI;
	while(age>TWOPI)age-=TWOPI;
	
	return l_moon;
}