/*Copyright (C) 
 * 2019 - gmail dot com
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

#include "Keypad.hpp"


// convierte un valor del ADC en la tecla correspondiente.
Key Keypad::decode()
{
	uint16_t key = analogRead( this->channel );

	if( key < 50 ) return Key::Enter;
	if( key < 200 ) return Key::Down;
	if( key < 500 ) return Key::Up;
	if( key < 700 ) return Key::Back;
	if( key < 900 ) return Key::Menu;
	else return Key::None;
}

/**
 * @brief Devuelve el código de la tecla presionada. Primero debe verificarse
 * que una tecla fue presionada utilizando a la función \see hit().
 *
 * @return El código de la tecla presionada \see Key
 */
Key Keypad::get()
{
	auto tmp = this->pressed;
	
	this->pressed = Key::None;
	// acknowledge

	return tmp;
}


/**
 * @brief Es la máquina de estados que determina si una tecla fue presionada.
 *
 * ¡¡¡ Debe ser llamada cada 1 ms !!!
 */
void Keypad::state_machine()
{
	switch( this->state ){
	case 0:
		this->key = decode();
		if( this->key != Key::None ){
			this->ticks = 80;
			this->state = 1;
		}
		break;

	case 1:
		--this->ticks;
		if( this->ticks == 0 ){
			auto new_key = decode();
			if( new_key == this->key ){
				this->state = 2;
			} else {
				this->pressed = Key::None;
				this->state = 0;
			}
		}
		break;

	case 2:
		/*
		 * TODO: poner el tiempo a 1seg
		 */

		this->ticks = 10;
		this->state = 3;
		break;

	case 3:
		/*
		 * TODO:
		 * ¿levantó el dedo? Sí, reestablecer
		 * ¿pasó 1 seg? Sí, enviar la tecla, poner el timer en 100ms, e ir a 4
		 */
		--this->ticks;
		if( this->ticks == 0 ){
			this->state = 4;
		}
		break;

	case 4:
	{
		auto new_key = decode();
		if( new_key != this->key ){
			this->state = 0;
			this->pressed = this->key;
		}
		break;
	}

	default:
		this->pressed = Key::None;
		this->state = 0;
		break;
	}
}


