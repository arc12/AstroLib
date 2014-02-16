/*
* Copyright 2012 Adam Cooper, derived from the work of Elco Jacobs.
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

#include <Arduino.h>
#include <AstroLib.cpp>

// Function prototypes go here (telling the compiler these functions exist).
void doSiderialTime();
void doSunPos();
void doMoonPos();

/*
* Place a direct direct copy of sketch (*.ino or *.pde) beneath this comment block.... except that you:
* 1: might have to add some function prototypes to the top, because the Arduino environment does this automatically but Atmel Studio does not.
* 2: might have to change the #include to the .cpp rather than .h (check which one includes the other. Arduino IDE seems able to work it out!)
*/

void setup(){
	Serial.begin(9600);
	doMoonPos();
	
}

void doSunPos(){
	
}

void doMoonPos(){
	Serial.println("Date=2003-09-01 T 00:00 UTC");
	long Y=2003;
	long M=9;
	long D=1;
	long h=0;
	long m=0;
	Serial.println("Ecliptic Coordinates");
	float beta;
	float M_moon;
	float age;
	float lambda = AstroLib::calcMoon(Y,M,D,h,m, beta, M_moon, age);
	Serial.print("Longitude=");
	Serial.println(R2D*lambda,3);
	Serial.print("Latitude=");
	Serial.println(R2D*beta,3);
	Serial.print("Lunar age (degrees)=");
	Serial.println(R2D*age,3);
	
	Serial.println("----- Lunar longitude at 00:00 on the 1st of each month in 2025");
	for(M=1;M<=12;M++){
		Serial.println(R2D*AstroLib::calcMoon(2025, M, 1, 0, 0, beta, M_moon, age),4);
	}
	
	Serial.println("----- Lunar age at 00:30 on EACH DAY in FEB 2014");
	for(D=1;D<=12;D++){
		AstroLib::calcMoon(2014, 2, D, 0, 30, beta, M_moon, age);
		Serial.println(R2D*age,4);
	}
	
	Serial.println("----- Lunar altitude at for each hour on 2014-02-10 at Wormhill (lat=53.27, lng=-1.82)");
	for(h=0;h<=12;h++){
		lambda = AstroLib::calcMoon(2014, 2, 10, h, 0, beta, M_moon, age);
		float alpha;
		float delta;
		AstroLib::ecliptic2equatorial(lambda, beta, alpha, delta);
		float LST = AstroLib::calcSiderial(2014, 2, 10, h, 0, -1.82);
		float H = LST*H2R - alpha; //Hour angle in radians!
		float alt;
		float az;
		AstroLib::equatorial2horizontal(H,delta,D2R*53.27, alt, az);
		Serial.println(alt*R2D,3);
	}
	
}

void doSiderialTime(){
		Serial.println("Date=1980-04-22 T 14:37 UTC");
		long Y=1980;
		long M=4;
		long D=22;
		long h=14;
		long m=37;
		Serial.println("Julian Date");
		long microDay;
		Serial.print(AstroLib::calcJulianDate(Y,M,D,h, m,microDay));
		Serial.print(".");
		Serial.println(microDay);
		Serial.println("Greenwich Siderial Time");
		float ST = AstroLib::calcSiderial(Y, M, D, h, m, 0.0);
		Serial.println(ST,6);
		Serial.println("and at -1.8177 Longitude");
		ST = AstroLib::calcSiderial(Y, M, D, h, m, -1.8177);
		Serial.println(ST,6);
		
		Serial.println("----- GST at 00:00 on the 1st of each month in 2025");
		for(M=1;M<=12;M++){
			Serial.println(AstroLib::calcSiderial(2025, M, 1, 0, 0, 0.0),4);
		}
		
		Serial.println("----- GST at 00:30 on EACH DAY in FEB 2014");
		for(D=1;D<=12;D++){
			Serial.println(AstroLib::calcSiderial(2014, 2, D, 0, 30, 0.0),4);
		}
		
		Serial.println("----- GST at for each hour on 2014-02-10");
		for(h=0;h<=12;h++){
			Serial.println(AstroLib::calcSiderial(2014, 2, 10, h, 0, 0.0),4);
		}
}

void loop(){
	
}