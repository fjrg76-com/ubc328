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


#ifndef  MENU_INC
#define  MENU_INC

#include <stdlib.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "UBC328.hpp"
#include "Keypad.hpp"


#ifndef MIN
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#endif



#define LCD_NUMBER_LINES 2 
// número de líneas del LCD


// no queremos meternos con matrices de cadenas
typedef struct MenuEntry
{
	char text[16];
	// la longitud máxima del texto es de 15 debido el carácter extra de
	// señalización
} MenuEntry;


// queremos devolver la tecla y la posición del cursor
typedef struct Pair_key_value
{
	Key key;
	uint8_t value;
} Pair_key_value;


/**
 * @brief Implementa un menú dentro de un LCD.
 */
class Menu
{
public:
	Menu( MenuEntry* _entries, uint8_t _num_entries );
	Pair_key_value Run();

private:
	void show();

	uint8_t offset;
	// indica la opción del menú a partir de la cual se imprimirán el resto de
	// opciones.

	uint8_t cursor;
	// indica la opción que se ejecutará si se oprime Enter.

	MenuEntry* entries;
	// es el arreglo de entradas de texto

	uint8_t num_entries;
	// indica el número de elementos en el arreglo de entradas.

	uint8_t min; 
	// guarda el valor que sea el mínimo entre el número de líneas del display y
	// el número de opciones
};


#endif   /* ----- #ifndef MENU_INC  ----- */
