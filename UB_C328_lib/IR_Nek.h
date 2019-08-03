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

#ifndef  IR_NEK_INC
#define  IR_NEK_INC

#include <Arduino.h>
#include <YetAnotherPcInt.h>

// Puede ser 21 o 17
#define IR_REMOTE_KEYS 21


#if IR_REMOTE_KEYS == 21

enum
{
	KEY_POWER = 0XA2,
	KEY_MODE = 0X62,
	KEY_MUTE = 0XE2,
	KEY_PLAY = 0X22,
	KEY_REW = 0X02,
	KEY_FORW = 0XC2,
	KEY_EQ = 0XE0,
	KEY_MINUS = 0XA8,
	KEY_PLUS = 0X90,
	KEY_0 = 0X68,
	KEY_SYM = 0X98,
	KEY_USD = 0XB0,
	KEY_1 = 0X30,
	KEY_2 = 0X18,
	KEY_3 = 0X7A,
	KEY_4 = 0X10,
	KEY_5 = 0X38,
	KEY_6 = 0X5A,
	KEY_7 = 0X42,
	KEY_8 = 0X4A,
	KEY_9 = 0X52,
};

#elif IR_REMOTE_KEYS == 17

enum
{
	KEY_UP = 0X62,
	KEY_LEFT = 0X22,
	KEY_OK = 0X02,
	KEY_RIGHT = 0XC2,
	KEY_DOWN = 0XA8,
	KEY_1 = 0X68,
	KEY_2 = 0X98,
	KEY_3 = 0XB0,
	KEY_4 = 0X30,
	KEY_5 = 0X18,
	KEY_6 = 0X7A,
	KEY_7 = 0X10,
	KEY_8 = 0X38,
	KEY_9 = 0X5A,
	KEY_STAR = 0X42,
	KEY_0 = 0X4A,
	KEY_HASH = 0X52,
};

#else

#error "DEBE DEFINIR UN TIPO DE CONTROL REMOTO!"

#endif  


typedef struct IR_Nek
{
	uint8_t pin;
	uint8_t address;
	bool done;
	bool error;
	uint32_t bits;
} IR_Nek;

void IR_Nek_init(  IR_Nek* self, uint8_t pin, uint8_t address );
uint8_t IR_Nek_get_pin( IR_Nek* self );
bool IR_Nek_ready( IR_Nek* self );
bool IR_Nek_read(  IR_Nek* self, uint8_t* data_val );
void IR_Nek_decode_isr( IR_Nek* self );

#endif   /* ----- #ifndef IR_NEK_INC  ----- */
