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

/*
 * Ajustar el número de teclas del control remoto, en el archivo:
 * 
 * UB_C328_lib/IR_Nek.h
 *
 * Este valor puede ser 17 o 21.
 *
 * También debería revisar este archivo para conocer todos los códigos del
 * control remoto.
 */

#include <LiquidCrystal.h>
#include <IR_Nek.h>

#define IR_PIN 17
#define IR_ADDRESS 0x00

LiquidCrystal lcd( 10, 9, 4, 5, 6, 7 );

IR_Nek ir;

void setup() 
{
	lcd.begin(16, 2);

	lcd.setCursor(0, 0);
	lcd.print("    UB-C328");
	lcd.setCursor(0, 1);
	lcd.print("  fjrg76.com");

	IR_Nek_init( &ir, IR_PIN, IR_ADDRESS );

	PcInt::attachInterrupt( IR_PIN, eint_isr, FALLING );

//	Serial.begin( 115200 );
}

void eint_isr()
{
	IR_Nek_decode_isr( &ir );
}

ISR(TIMER1_OVF_vect)
{
	TCCR1B = 0;                                    
}


void loop() 
{
	uint8_t command_byte;

	if( IR_Nek_ready( &ir ) and IR_Nek_read( &ir, &command_byte ) ){

		lcd.clear();
		lcd.print( "CMD: 0x" );
		lcd.print( command_byte, HEX );

		lcd.setCursor( 0, 1 );
		lcd.print( "KEY: " );
		switch( command_byte ){
			case KEY_MODE:
				lcd.print( "MENU" );
				break;

			case KEY_REW:
				lcd.print( "BACK" );
				break;

			case KEY_FORW:
				lcd.print( "ENTER" );
				break;

			case KEY_PLUS:
				lcd.print( "UP" );
				break;

			case KEY_MINUS:
				lcd.print( "DOWN" );
				break;

			default:
				lcd.print( "OTHER" );
				break;
		}

		delay( 1000 );

		lcd.clear();
		lcd.print("    UB-C328");
		lcd.setCursor(0, 1);
		lcd.print("  fjrg76.com");
	}
}


