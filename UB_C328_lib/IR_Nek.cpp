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


#include "IR_Nek.h"

//
// esta callback será llamada cada vez que se presente un flanco de bajada
//
void IR_Nek_decode_isr( IR_Nek* self )
{
	static uint8_t state = 0;
	static uint8_t bit_counter = 0;
	uint16_t period = 0;

	switch( state ){

		case 0:
			self->error = false;

			TCNT1 = 0;
			TCCR1B = 2;
			state = 1;

			break;

		case 1:
			period = TCNT1 >> 1;
			TCNT1 = 0;

			if( 13000 < period and period < 14000 ){
				state = 2;
				self->done = false;
				bit_counter = 32;
				self->bits = 0L;
			} else{
				state = 0;
				self->error = true;
				TCCR1B = 0;
			}
			break;

		case 2:
		{

			period = TCNT1 >> 1;
			TCNT1 = 0;

			uint8_t bit_val;
			if( 1000 < period and period < 1300 ){
				bit_val = 0;
			} else if( 2100 < period and period < 2400 ){
				bit_val = 1;
			} else{
				state = 0;
				self->error = true;
				TCCR1B = 0;
			}

			self->bits |= bit_val ? 0x00000001 : 0x00000000;
			if( bit_counter > 1 ) self->bits <<= 1;

			--bit_counter;
			if( bit_counter == 0 ){
				TCCR1B = 0;
				state = 0;
				self->error = false;
				self->done = true;
			}

			break;
		}

		default:
			TCCR1B = 0;
			state = 0;
			self->done = false;
			self->error = true;
			break;

	} // switch

}

/**
 * @brief Inicializa al módulo IR.
 *
 * @param self Referencia a un objeto tipo IR_Nek.
 * @param pin Pin de Arduino asociado al receptor IR.
 * @param address La dirección del control remoto.
 */
void IR_Nek_init(  IR_Nek* self, uint8_t pin, uint8_t address )
{
	self->pin = pin;
	self->address = address;

	self->bits = 0;
	self->error = false;
	self->done = false;

	TCCR1A = 0;
	TCCR1B = 0;                                    
	TCNT1  = 0;                                    
	TIMSK1 = 1;                                    

	pinMode( self->pin, INPUT );
}


/**
 * @brief Devuelve el pin asociado al receptor IR.
 *
 * @param self Referencia a un objeto tipo IR_Nek.
 *
 * @return El pin asociado al receptor IR.
 */
uint8_t IR_Nek_get_pin( IR_Nek* self )
{
	return self->pin;
}

/**
 * @brief Indica si se completó la recepción de un comando del control IR.
 *
 * @param self Referencia a un objeto tipo IR_Nek.
 *
 * @return true si está listo un comando para ser leído; false en caso
 * contrario.
 */
bool IR_Nek_ready( IR_Nek* self )
{
	return self->done;
}


/**
 * @brief Devuelve el código de la tecla presionada en el control remoto.
 *
 * @param address La dirección asignada al control remoto.
 * @param data_val El código de la tecla presionada. Este valor tiene sentido
 *        si, y sólo si, el valor devuelto es true.
 *
 * @return true si la decodificación fue correcta. false en cualquier otro caso. \n
 *         La decodificación puede ser falsa en los siguientes escenarios: \n
 *         a) si la combinación de los bytes directos e invertidos es incorrecta,
 *         b) la dirección decodificada no coincide con la recibida en la
 *            lista de argumentos.
 */
bool IR_Nek_read( IR_Nek* self, uint8_t* data_val )
{
	uint8_t data[4] = {0};

	// address
	data[ 0 ] = ( self->bits & 0xff000000 ) >> 24;
	data[ 1 ] = ( self->bits & 0x00ff0000 ) >> 16;

	// command
	data[ 2 ] = ( self->bits & 0x0000ff00 ) >> 8;
	data[ 3 ] = ( self->bits & 0x000000ff );

	bool no_error = false;

	if( ( data[0] ^ data[1] ) == 0xff and (data[2] ^ data[3] ) == 0xff and data[ 0 ] == self->address ){
		*data_val = data[2];
		no_error = true;
	}

	self->done = false;
	// acknowledge

	return no_error;
}
