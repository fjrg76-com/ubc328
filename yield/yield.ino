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

// Este ejemplo muestra el uso básico de la función de Arduino, yield()

#include <stdlib.h>
// para uint32_t

constexpr uint32_t TICKS_PER_SECOND = 142857;
// 1000ms / 7us = 142857.1


bool state{ false };
uint32_t ticks{ TICKS_PER_SECOND };


void yield( void )
{

	--ticks;
	if( ticks == 0 ){
		ticks = TICKS_PER_SECOND;

		state = state == false ? true : false;
		digitalWrite( 13, state );
	}
}


void setup() 
{
	pinMode( 13, OUTPUT );
}

void loop() 
{
	delay( 1 );
	// if the built-in led blinks, then the system is alive!
}
