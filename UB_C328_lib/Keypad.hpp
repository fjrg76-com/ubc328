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

#ifndef  KEYPAD_INC
#define  KEYPAD_INC

#include <stdlib.h>
#include <Arduino.h>
#include "UBC328.hpp"


enum class Key : uint8_t { None, Enter, Down, Up, Back, Menu, };

class Keypad
{
public:
	explicit Keypad( uint8_t channel ) : channel{ channel }
	{

	}

	/**
	 * @brief Indica si una tecla fue presionada.
	 *
	 * @return true si una tecla fue presionada; false en caso contrario.
	 */
	bool hit(){ return pressed != Key::None; }

	Key get();

	void state_machine();
	// must be called every 1ms !!!

private:
	Key decode();

	uint8_t channel;
	uint16_t ticks;
	Key key;
	Key pressed;
	uint8_t state;
};


#endif   /* ----- #ifndef KEYPAD_INC  ----- */
