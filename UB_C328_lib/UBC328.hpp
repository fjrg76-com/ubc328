/*Copyright (C) 
 * 2019 - fjrg76 dot com
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#ifndef  UBC328_INC
#define  UBC328_INC

#include <stdlib.h>
#include <Arduino.h>
#include <LiquidCrystal.h>


enum
{
	I1 = 15,
	I2 = 16,
	I3 = 17,
	I4 = 18,
	I5 = 19,
};

enum
{
	D1 = 0,
	D2 = 1,
	D3 = 3,
	D4 = 8,
	D5 = 11,
	D6 = 12,
	D7 = 13,
};




class UBC328
{
public:
	UBC328() : lcd( 10, 9, 4, 5, 6, 7 )
	{
		lcd.begin( 16, 2 );

		pinMode( 13, OUTPUT );
		pinMode( 2, OUTPUT );
		pinMode( 8, OUTPUT );
		pinMode( 3, OUTPUT );
	}

	void d13( uint8_t newState ) { digitalWrite( 13, newState ); }

	void relay( uint8_t newState ) { digitalWrite( 2, newState ); }

	void bl_lcd( uint8_t newState ) { digitalWrite( 3, newState ); }

	void beep( uint8_t newState ) { digitalWrite( 8, newState ); }

	void beep( uint8_t newState, size_t duration );

	void beep_counter();

	LiquidCrystal lcd;

private:
	volatile size_t beep_duration;
};



#endif   /* ----- #ifndef UBC328_INC  ----- */
