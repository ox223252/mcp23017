#ifndef __MCP23017_H__
#define __MCP23017_H__
////////////////////////////////////////////////////////////////////////////////
/// \copiright ox223252, 2018
///
/// This program is free software: you can redistribute it and/or modify it
///     under the terms of the GNU General Public License published by the Free
///     Software Foundation, either version 2 of the License, or (at your
///     option) any later version.
///
/// This program is distributed in the hope that it will be useful, but WITHOUT
///     ANY WARRANTY; without even the implied of MERCHANTABILITY or FITNESS FOR
///     A PARTICULAR PURPOSE. See the GNU General Public License for more
///     details.
///
/// You should have received a copy of the GNU General Public License along with
///     this program. If not, see <http://www.gnu.org/licenses/>
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
/// \file pca9685.h
/// \brief library used to manage pca9685 micro
/// \author ox223252
/// \date 2018-03
/// \copyright GPLv2
/// \version 0.1
/// \warning NONE
/// \bug NONE
////////////////////////////////////////////////////////////////////////////////

#define iT(type) mcp23017_IT_##type

enum
{
	mcp23017_IT_off,
	mcp23017_IT_edges,
	mcp23017_IT_high,
	mcp23017_IT_low
}
mcp23017InterruptType;

////////////////////////////////////////////////////////////////////////////////
/// \fn int openMCP23017 ( const char busName[], const uint8_t address, 
///     int * const mcp23017 );
/// \param[ in ] busName: i2c bus name "/dev/i2c-0"
/// \param[ in ] address: chip address on i2c bus
/// \param[ out ] mcp23017: file descriptor pointer to access to device
/// \breif create access to mcp23017 chip
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int openMCP23017 ( const char busName[], const uint8_t address, 
	int * const mcp23017 );

////////////////////////////////////////////////////////////////////////////////
/// \fn int closeMCP23017 ( int const mcp23017 );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \breif close access to mcp23017 chip
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int closeMCP23017 ( const int mcp23017 );

////////////////////////////////////////////////////////////////////////////////
/// \fn int gpioSetDir ( const int mcp23017, const char port, const uint8_t id,
///     const uint8_t mode );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \param[ in ] id: GPIO pin id on port [ 0 ; 7 ]
/// \param[ in ] mode: 1 input / 0 output
/// \breif Set GPIO pin dir
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int gpioSetDir ( const int mcp23017, const char port, const uint8_t id, 
	const uint8_t mode );

////////////////////////////////////////////////////////////////////////////////
/// \fn int gpioSetPol ( const int mcp23017, const char port, const uint8_t id,
///     const uint8_t mode );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \param[ in ] id: GPIO pin id on port [ 0 ; 7 ]
/// \param[ in ] mode: 1 active low / 0 active high
/// \breif Set GPIO pin polarity
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int gpioSetPol ( const int mcp23017, const char port , const uint8_t id, 
	const uint8_t mode );

////////////////////////////////////////////////////////////////////////////////
/// \fn int gpioSetPol ( const int mcp23017, const char port, const uint8_t id,
///     const uint8_t mode );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \param[ in ] id: GPIO pin id on port [ 0 ; 7 ]
/// \param[ in ] mode: 1 active low / 0 active high
/// \breif Set GPIO pin polarity
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int gpioSetInt ( const int mcp23017, const char port, const uint8_t id, 
	const uint8_t mode );

////////////////////////////////////////////////////////////////////////////////
/// \fn int gpioSetPol ( const int mcp23017, const char port, const uint8_t id,
///     const uint8_t mode );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \param[ in ] id: GPIO pin id on port [ 0 ; 7 ]
/// \param[ in ] mode: 1 100k pull-up / 0 no pull-up
/// \breif Set GPIO pin internal pull up
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int gpioSetPullUp ( const int mcp23017, const char port, const uint8_t id,
	const uint8_t mode );

////////////////////////////////////////////////////////////////////////////////
/// \fn int getInteruptsStatus ( const int mcp23017, const char port );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \breif Get interrupt flags status
/// \return flags status or 0 if fail
////////////////////////////////////////////////////////////////////////////////
uint8_t getInteruptsStatus ( const int mcp23017, const char port );

////////////////////////////////////////////////////////////////////////////////
/// \fn int getInteruptsValue ( const int mcp23017, const char port );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \breif Get GPIO value when interrupt occured
/// \return value catched or 0 if failed
////////////////////////////////////////////////////////////////////////////////
uint8_t getInteruptsValue ( const int mcp23017, const char port );

////////////////////////////////////////////////////////////////////////////////
/// \fn int gpioSet ( const int mcp23017, const char port, const uint8_t id, 
///     const uint8_t status );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \param[ in ] id: GPIO pin id on port [ 0 ; 7 ]
/// \param[ in ] status: value to write to port[id]
/// \breif Set GPIO status for specified id
/// \return 0: OK
///         else see errno>
////////////////////////////////////////////////////////////////////////////////
int gpioSet ( const int mcp23017, const char port, const uint8_t id, 
	const uint8_t status );

////////////////////////////////////////////////////////////////////////////////
/// \fn uint8_t gpioSet ( const int mcp23017, const char port, 
///     const uint8_t id );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \param[ in ] id: GPIO pin id on port [ 0 ; 7 ]
/// \breif Get GPIO status for specified id
/// \return value read or 0xff if failed
////////////////////////////////////////////////////////////////////////////////
uint8_t gpioGet ( const int mcp23017, const char port, const uint8_t id );

////////////////////////////////////////////////////////////////////////////////
/// \fn int portSet ( const int mcp23017, const char port, 
///     const uint8_t status );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \param[ in ] status: value to write to port
/// \breif Set all GPIOs of the specified port
/// \return 0: OK
///         else see errno>
////////////////////////////////////////////////////////////////////////////////
int portSet ( const int mcp23017, const char port, const uint8_t status );

////////////////////////////////////////////////////////////////////////////////
/// \fn uint8_t gpioSet ( const int mcp23017, const char port );
/// \param[ in ] mcp23017: file descriptor to access to device
/// \param[ in ] port: GPIO port ( 'A' / 'B' )
/// \breif Get GPIO status for specified id
/// \return value read or 0 if failed
////////////////////////////////////////////////////////////////////////////////
uint8_t portGet ( const int mcp23017, const char port );

#endif
