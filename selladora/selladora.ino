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

#include <stdlib.h>
#include <LiquidCrystal.h>


class Time
{
public:
	Time( uint8_t _hrs = 0, uint8_t _min = 0, uint8_t _sec = 0 ) : 
		hrs{ _hrs }, min{ _min }, sec{ sec }
	{
		// nothing
	}

	/**
	 * @brief Increments the clock one second each time it's called
	 *
	 * THIS FUNCTION MUST BE CALLED ONCE PER SECOND!
	 *
	 * @post  The time is incremented in one second.
	 */
	void tick()
	{
		++sec;
		if( sec > 59 ){
			sec = 0;
			++min;
			if( min > 59 ){
				min = 0;
				++hrs;
				if( hrs > 23 ){
					hrs = 0;
				}
			}
		}
	}

	uint8_t hours() { return hrs; }
	uint8_t minutes(){ return min; }
	uint8_t seconds(){ return sec; }

private:
	uint8_t hrs;
	uint8_t min;
	uint8_t sec;
};




const uint8_t KEY_ANALOG_PIN = 0;

enum 
{
	Enter,
	Down,
	Up,
	Back,
	Menu,
	None,
};

enum
{
	On,
	Off,
};


const uint8_t LED_ONBOARD =    13;
const uint8_t RELE_ONBOARD =   2;
const uint8_t BUZZER_ONBOARD = 8;
const uint8_t BL_LCD_ONBOARD = 3;

class UBC328
{
public:
	UBC328() : lcd( 10, 9, 4, 5, 6, 7 )
	{
		lcd.begin( 16, 2 );

		pinMode( LED_ONBOARD, OUTPUT );
		pinMode( RELE_ONBOARD, OUTPUT );
		pinMode( BUZZER_ONBOARD, OUTPUT );
		pinMode( BL_LCD_ONBOARD, OUTPUT );
	}

	void beep( uint8_t newState ) { digitalWrite( BUZZER_ONBOARD, newState ); }

	void d13( uint8_t newState ) { digitalWrite( LED_ONBOARD, newState ); }

	void relay( uint8_t newState ) { digitalWrite( RELE_ONBOARD, newState ); }

	void bl_lcd( uint8_t newState ) { digitalWrite( BL_LCD_ONBOARD, newState ); }

	bool kb_hit() { return kb_pushed != None; }

	uint8_t kb_get() 
	{ 
		uint8_t tmp = kb_pushed;
		kb_pushed = None;
		return tmp;
	}

	void kb_state_machine();

	LiquidCrystal lcd;

private:
	uint8_t kb_ticks{ 0 };
	uint8_t kb_state{ 0 };
	uint8_t kb_key{ None };
	uint8_t kb_pushed{ None };

	uint8_t kb_read()
	{
		uint16_t key = analogRead( 0 );

		if( key < 50 ) return Enter;
		if( key < 200 ) return Down;
		if( key < 500 ) return Up;
		if( key < 700 ) return Back;
		if( key < 900 ) return Menu;
		else return None;
	}

};

// MUST BE CALLED EVERY 1ms !!!
void UBC328::kb_state_machine()
{
switch( kb_state ){
	case 0:
		kb_key = kb_read();
		if( kb_key != None ){
			kb_ticks = 200;
			kb_state = 1;
		}
		break;

	case 1:
		--kb_ticks;
		if( kb_ticks == 0 ){
			uint8_t new_key = kb_read();
			if( new_key == kb_key ){
				kb_state = 2;
			} else {
				kb_pushed = None;
				kb_state = 0;
			}
		}
		break;

	case 2:
		kb_ticks = 10;
		kb_state = 3;
		beep( HIGH );
		break;

	case 3:
		--kb_ticks;
		if( kb_ticks == 0 ){
			kb_state = 4;
			beep( LOW );
		}
		break;

	case 4:
		{
			uint8_t new_key = kb_read();
			if( new_key != kb_key ){
				kb_state = 0;
				kb_pushed = kb_key;
			}
			break;
		}

	default:
		kb_pushed = None;
		kb_state = 0;
		break;
}
}

UBC328 ubc328;
Time time( 12 );



size_t TICKS_PER_MILLISECOND = 143;
// the yield() function is called every 7us; there are 143 function
// calls in 1ms. The delay() function must be called as often as possible!

uint32_t ticks = TICKS_PER_MILLISECOND;

void yield()
{
	--ticks;
	if( ticks == 0 ){
		ticks = TICKS_PER_MILLISECOND;

		{
			static size_t one_thousend_ticks = 1000;

			--one_thousend_ticks;
			if( one_thousend_ticks == 0 ){
				one_thousend_ticks = 1000;
				time.tick();
			}
		}

		{
			ubc328.kb_state_machine();
		}

	}
}


void setup() 
{
	ubc328.d13( true );

	ubc328.lcd.begin( 16, 2 );
	// the ubc328 object forwards the calls to the LCD; so, for the case of it,
	// you can use the same functions of that the LCD, but prefixing them with
	// the object's name.

	ubc328.bl_lcd( HIGH );
	ubc328.lcd.print( "    UB-C328A" );
	ubc328.lcd.setCursor( 0, 1 );
	ubc328.lcd.print( "   fjrg76.com" );

	ubc328.beep( HIGH );
	// for the rest of the API, the call must be as usual.

	pinMode( 16, INPUT_PULLUP );
	// I2 -> switch de activación



	delay( 100 );
	ubc328.beep( LOW );
	ubc328.d13( false );
	delay( 900 );
}

double mcp9700( uint8_t analog_channel )
{
	uint16_t temp_raw = 0;
	for( auto i = 8; i > 0; --i ){ temp_raw += analogRead( analog_channel ); }
	uint16_t temp_x = temp_raw >> 3;
	return (500.0 * temp_x / 1024.0) - 50.0;
}


class Params
{
public:
	Params( uint8_t temp_max, uint8_t temp_min, uint8_t temp_work, uint8_t time_MOD ) :
		temp_max{ temp_max }, temp_min{ temp_min }, temp_work{ temp_work }, time_MOD{ time_MOD },
		time{ 0 }, active{ false }
	{
		// nada
	}

	bool is_active(){ return active; }
	void start() { this->active = true; }
	void stop() { this->active = false; }


	uint8_t temp_max;
	uint8_t temp_min;
	uint8_t temp_work;
	uint8_t time;
	uint8_t time_MOD;

private:
	bool active;;
};

void selladora( class Params& p)
{
	static uint8_t selladora_sm = 0;
	switch( selladora_sm ){
		case 0:
			if( digitalRead( 16 ) == LOW ){
				selladora_sm = 1;
			} else{
				selladora_sm = 0;
			}
			break;

		case 1:
		{
			auto temp_in_centi = mcp9700( A1 );
			ubc328.lcd.setCursor( 0, 0 );
			ubc328.lcd.print( "TEMP: " );
			ubc328.lcd.print( temp_in_centi );


			if( temp_in_centi >= p.temp_work ){

				ubc328.d13( HIGH );
				p.time = p.time_MOD;
				p.start();
				selladora_sm = 2;
			}

			if( digitalRead( 16 ) == HIGH ){
				ubc328.lcd.print( "OPEN " );

				ubc328.relay( LOW );
				ubc328.d13( LOW );
				selladora_sm = 0;
				p.stop();
			}
			break;
		}

		case 2:
			ubc328.lcd.setCursor( 0, 0 );
			ubc328.lcd.print( "TEMP: " );
			ubc328.lcd.print( mcp9700( A1 ) );
			ubc328.lcd.setCursor( 0, 1 );
			ubc328.lcd.print( "TIME: " );
			ubc328.lcd.print( p.time );

			if( p.time == 0 or digitalRead( 16 ) == HIGH ){
				ubc328.relay( LOW );
				ubc328.d13( LOW );
				selladora_sm = 0;
				p.stop();
			}
			break;

		default:
			ubc328.lcd.clear();
			ubc328.lcd.setCursor( 0, 0 );
			ubc328.lcd.print( "    ERROR: " );

			//selladora_sm = 0;
			ubc328.relay( LOW );
			ubc328.d13( LOW );
			break;
	}
}


void loop() 
{
	static Params params( 90, 50, 79, 5 );


	delay( 1 );
	// we need this call to trigger the call to the yield() callback.
	// This method isn't very accurate. I prefer a true interrupt handled
	// system tick.

	selladora( params );
	// proceso principal



	static uint8_t last_seconds = time.seconds();
	if( time.seconds() != last_seconds ){
		last_seconds = time.seconds();

		if( params.is_active() and params.time > 0 ){
			--params.time;
		} else{
			auto temp_in_centi = mcp9700( A1 );
			ubc328.lcd.clear();
			ubc328.lcd.setCursor( 0, 0 );
			ubc328.lcd.print( "Temp: " );
			ubc328.lcd.print( temp_in_centi );

			ubc328.lcd.setCursor( 0, 1 );
			ubc328.lcd.print( "Time: " );
			ubc328.lcd.print( params.time_MOD );
		}

	}

	if( ubc328.kb_hit() ) {

		switch( ubc328.kb_get() ) {
			case Enter:
				break;

			case Down:
				--params.time_MOD;
				if( params.time_MOD < 1 ) params.time_MOD = 1;
				ubc328.lcd.setCursor( 0, 1 );
				ubc328.lcd.print( "Time: " );
				ubc328.lcd.print( params.time_MOD );
				break;

			case Up:
				++params.time_MOD;
				if( params.time_MOD > 30 ) params.time_MOD = 30;
				ubc328.lcd.setCursor( 0, 1 );
				ubc328.lcd.print( "Time: " );
				ubc328.lcd.print( params.time_MOD );
				break;
				
			case Back:
				break;

			case Menu:  
				break;

			default: 
				break;
		}
	}

}

