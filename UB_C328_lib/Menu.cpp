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

#include "Menu.hpp"


extern UBC328 ubc328;
extern Keypad keypad;


/**
 * @brief Constructor
 *
 * @param _entries Textos para cada elemento del menú
 * @param _num_entries Número de entradas del menú
 */
Menu::Menu( MenuEntry* _entries, uint8_t _num_entries ) :
	offset{0}, cursor{0}, entries{_entries}, num_entries{_num_entries}
{
	this->min = MIN( LCD_NUMBER_LINES, this->num_entries );
}

/**
 * @brief Ejecuta al menú.
 *
 * @return El par <<tecla, valor>>. 'valor' tiene sentido cuando 'tecla' es
 *         Key::Enter.
 */
Pair_key_value Menu::Run()
{
	Pair_key_value a_pair;

	show();

	while( 1 )
	{
		delay( 1 );

		keypad.state_machine();
		// state_machine debe ser llamada cada 1ms

		if( keypad.hit() ){
			auto option = keypad.get();
			ubc328.beep( HIGH ); delay( 10 ); ubc328.beep( LOW );

			switch( option ) {
			case Key::Up:
				if( this->cursor > 0 ) --this->cursor;
				break;

			case Key::Down:
				if( this->cursor < (this->num_entries - 1) ) ++this->cursor;
				break;

			case Key::Enter: 
				a_pair.key = Key::Enter;
				a_pair.value = this->cursor;

//				this->cursor = 0;
				// eliminar esta línea si se desea volver a la última opción
				// seleccionada cuando se vuelva a presionar 'Menú'.

				return a_pair;

			case Key::Back:
				this->cursor = 0;
				a_pair.key = Key::Back;
				a_pair.value = 0;
				return a_pair;

			case Key::Menu:
				this->cursor = 0;
				a_pair.key = Key::Menu;
				a_pair.value = 0;
				return a_pair;

			default: break;
			}

			show();
		}
	}
}

void Menu::show()
{
	this->offset = LCD_NUMBER_LINES > this->cursor ? 0 : this->cursor - (LCD_NUMBER_LINES-1);

	ubc328.lcd.clear();

	for( int i = 0; i < this->min; ++i ) {
		ubc328.lcd.setCursor( 0, i );

		ubc328.lcd.write( (this->offset+i) == this->cursor ? 0 : ' ' );
		// por eso las cadenas deben ser de 15 carácteres y no de 16

		ubc328.lcd.println( this->entries[ this->offset+i ].text );
	}
}

