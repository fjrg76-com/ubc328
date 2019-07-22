
#include "UBC328.hpp"

/**
 * @brief Genera un beep con una duración determinada.
 *
 * @param newState Esta aquí por compatibilidad, pero no se usa.
 * @param duration Duración del beep en milisegundos.
 */
void UBC328::beep( uint8_t newState, size_t duration )
{
	beep_duration = duration;
	digitalWrite( 8, HIGH );
}

/**
 * @brief Controla la duración del beep. ¡Debe ser llamada cada milisegundo!
 */
void UBC328::beep_counter()
{
	if( beep_duration > 0 ){
		--beep_duration;
		if( beep_duration == 0 ){
			digitalWrite( 8, LOW );
		}
	}
}

