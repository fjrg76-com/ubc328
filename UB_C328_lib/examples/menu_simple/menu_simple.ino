/*Copyright (C)
   2019 - fjrg76 dot com
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/*
   Programa de ejemplo para el uso de un menú en la tarjeta UB-C328
   Programó: fjrg76 dot com
   (http://fjrg76.com/ub-c328/)
*/

#include <stdlib.h>
#include <LiquidCrystal.h>

#include "UBC328.hpp"
#include "Keypad.hpp"
#include "Menu.hpp"

/**
   @brief Ejemplo de parámetros.
*/
struct Params
{
  Params( int temp_max, int temp_min, int hum_max, int hum_min ) :
    temp_max {temp_max}, temp_min {temp_min}, hum_max {hum_max}, hum_min {hum_min}
  {
    // nada
  }

  int temp_max;
  int temp_min;
  int hum_max;
  int hum_min;
};

Params params( 90, 60, 80, 30 );
// conjunto de parámetros de prueba


using pfun1 = void (*)(Params&);
//typedef void (*pfun1)(Params&);
// funciones que no devuelven nada y reciben como parámetro una referencia a un
// objeto Params


UBC328 ubc328;
// control de la tarjeta

Keypad keypad( A0 );
// control del keypad




/**
   @brief Modifica el valor de temp_max.

   @param p Referencia a un objeto Params.
*/
void set_temp_max( Params& p )
{
  int shadow{ p.temp_max };

  ubc328.lcd.clear();
  ubc328.lcd.print( "TEMP_MAX: " );
  ubc328.lcd.print( shadow );

  while ( 1 )
  {
    delay( 1 );
    keypad.state_machine();
    if ( keypad.hit() ) {
      ubc328.beep( HIGH ); delay( 10 ); ubc328.beep( LOW );

      auto key = keypad.get();
      switch ( key ) {
        case Key::Back: return;
        case Key::Menu: return;
        case Key::Up: ++shadow; break;
        case Key::Down: --shadow; break;
        case Key::Enter:
          p.temp_max = shadow;
          ubc328.beep( HIGH );
          delay( 100 );
          ubc328.beep( LOW );
          break;
      }

      ubc328.lcd.clear();
      ubc328.lcd.print( "TEMP_MAX: " );
      ubc328.lcd.print( shadow );
    }
  }
}

void set_temp_min( Params& p )
{

  int shadow{ p.temp_min };

  ubc328.lcd.clear();
  ubc328.lcd.print( "TEMP_MIN: " );
  ubc328.lcd.print( shadow );

  while ( 1 )
  {
    delay( 1 );
    keypad.state_machine();
    if ( keypad.hit() ) {
      ubc328.beep( HIGH ); delay( 10 ); ubc328.beep( LOW );

      auto key = keypad.get();
      switch ( key ) {
        case Key::Back: return;
        case Key::Menu: return;
        case Key::Up: ++shadow; break;
        case Key::Down: --shadow; break;
        case Key::Enter:
          p.temp_min = shadow;
          ubc328.beep( HIGH );
          delay( 100 );
          ubc328.beep( LOW );
          break;
      }

      ubc328.lcd.clear();
      ubc328.lcd.print( "TEMP_MIN: " );
      ubc328.lcd.print( shadow );
    }
  }
}

void set_hum_max( Params& p )
{
  int shadow{ p.hum_max };

  ubc328.lcd.clear();
  ubc328.lcd.print( "HUM_MAX: " );
  ubc328.lcd.print( shadow );

  while ( 1 )
  {
    delay( 1 );
    keypad.state_machine();
    if ( keypad.hit() ) {
      ubc328.beep( HIGH ); delay( 10 ); ubc328.beep( LOW );

      auto key = keypad.get();
      switch ( key ) {
        case Key::Back: return;
        case Key::Menu: return;
        case Key::Up: ++shadow; break;
        case Key::Down: --shadow; break;
        case Key::Enter:
          p.hum_max = shadow;
          ubc328.beep( HIGH );
          delay( 100 );
          ubc328.beep( LOW );
          break;
      }

      ubc328.lcd.clear();
      ubc328.lcd.print( "HUM_MAX: " );
      ubc328.lcd.print( shadow );
    }
  }
}

void set_hum_min( Params& p )
{

  int shadow{ p.hum_min };

  ubc328.lcd.clear();
  ubc328.lcd.print( "HUM_MIN: " );
  ubc328.lcd.print( shadow );

  while ( 1 )
  {
    delay( 1 );
    keypad.state_machine();
    if ( keypad.hit() ) {
      ubc328.beep( HIGH ); delay( 10 ); ubc328.beep( LOW );

      auto key = keypad.get();
      switch ( key ) {
        case Key::Back: return;
        case Key::Menu: return;
        case Key::Up: ++shadow; break;
        case Key::Down: --shadow; break;
        case Key::Enter:
          p.hum_min = shadow;
          ubc328.beep( HIGH );
          delay( 100 );
          ubc328.beep( LOW );
          break;
      }

      ubc328.lcd.clear();
      ubc328.lcd.print( "HUM_MIN: " );
      ubc328.lcd.print( shadow );
    }
  }
}

void setup()
{
  uint8_t symbol_0[8] =
  {
    B01000,
    B01100,
    B01110,
    B01111,
    B01110,
    B01100,
    B01000,
    B00000,
  };

  ubc328.lcd.createChar( 0, symbol_0 );

  ubc328.bl_lcd( HIGH );
  // enciende al led de backlight del LCD

  pinMode( I2, INPUT_PULLUP );
  // establece en la entrada I2 al switch (no se usa en este ejemplo)

  //Serial.begin( 115200 );
  // descomentar si se desea utilizar a la UART

  ubc328.lcd.clear();
  ubc328.lcd.print( "    UB-C328" );
  ubc328.lcd.setCursor( 0, 1 );
  ubc328.lcd.print( "   fjrg76.com" );

  ubc328.beep( HIGH );
  delay( 50 );
  ubc328.beep( LOW );
  delay( 1950 );


  char cad[31];
  // de usarse un arreglo para lograr la composición de un mensaje complejo,
  // uno se debe asegurar de hacerlo lo suficientemente grande, aunque en el
  // LCD no se alcancen a ver todos los símbolos. De otra manera, el programa
  // se corrompe.

  ubc328.lcd.clear();
  ubc328.lcd.print( "    UB-C328" );
  ubc328.lcd.setCursor( 0, 1 );
  sprintf( cad, "T(%02d,%02d)H(%02d,%02d)", params.temp_max, params.temp_min, params.hum_max, params.hum_min );
  ubc328.lcd.print( cad );
}

void loop()
{
  static size_t ticks_in_one_second{ 1000 };
  
  static MenuEntry text[4] = { "TEMP_MAX       ", "TEMP_MIN       ", "HUM_MAX        ", "HUM_MIN        " };
  static Menu menu( text, 4 );
  static pfun1 menu_set[4] = { set_temp_max, set_temp_min, set_hum_max, set_hum_min };
  static char cad[31];
  
  delay( 1 );

  keypad.state_machine();
  // se debe leer cada milisegundo en las funciones que vayan a utilizar
  // al keypad. La razón es evitar los conflictos entre las lecturas del
  // ADC (lecturas síncronas vs asíncronas).

  if ( keypad.hit() ) {
    auto key = keypad.get();

    ubc328.beep( HIGH ); delay( 10 ); ubc328.beep( LOW );

    if ( key == Key::Menu ) {
      Pair_key_value option;

      do {
        option = menu.Run();

        if ( option.key == Key::Enter ) { menu_set[ option.value ]( params ); }

      } while ( option.key != Key::Back );

      ubc328.lcd.clear();
      ubc328.lcd.print( "    UB-C328" );
      ubc328.lcd.setCursor( 0, 1 );
      sprintf( cad, "T(%02d,%02d)H(%02d,%02d)", params.temp_max, params.temp_min, params.hum_max, params.hum_min );
      ubc328.lcd.print( cad );
    }
  }

  --ticks_in_one_second;
  if ( ticks_in_one_second == 0 ) {
    ticks_in_one_second = 1000;

    // --- Poner aquí código que se deba ejecutar cada 1 seg ---

  }
}

