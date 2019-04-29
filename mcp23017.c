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

////////////////////////////////////////////////////////////////////////////////
/// \file mcp23017.c
/// \brief library used to manage mcp23017 micro
/// \author ox223252
/// \date 2018-04
/// \copyright GPLv2
/// \version 0.1
/// \warning NONE
/// \bug NONE
////////////////////////////////////////////////////////////////////////////////

#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <errno.h>

#include "mcp23017.h"

enum
{
	B0_IODIRA = 0x00,
	B0_IODIRB = 0x01,
	B0_IPOLA = 0x02,
	B0_IPOLB = 0x03,
	B0_GPINTENA = 0x04,
	B0_GPINTENB = 0x05,
	B0_DEFVALA = 0x06,
	B0_DEFVALB = 0x07,
	B0_INTCONA = 0x08,
	B0_INTCONB = 0x09,
	B0_IOCON = 0x0A,
	B0_IOCON_ = 0x0B,
	B0_GPPUA = 0x0C,
	B0_GPPUB = 0x0D,
	B0_INTFA = 0x0E,
	B0_INTFB = 0x0F,
	B0_INTCAPA = 0x10,
	B0_INTCAPB = 0x11,
	B0_GPIOA = 0x12,
	B0_GPIOB = 0x13,
	B0_OLATA = 0x14,
	B0_OLATB = 0x15
}
registersBank_0;

enum
{
	REGA = 0x00,
	REGB = 0x10,
	IODIR = 0x00,
	IPOL = 0x01,
	GPINTEN = 0x02,
	DEFVAL = 0x03,
	INTCON = 0x04,
	IOCON = 0x05,
	GPPU = 0x06,
	INTF = 0x07,
	INTCAP = 0x08,
	GPIO = 0x09,
	OLAT = 0x0a
}
registersBank_1;

static inline uint8_t getPort ( const char port )
{
	if ( port == 'A' )
	{
		return ( REGA );
	}
	else if ( port == 'B' )
	{
			return ( REGB );
	}
	return ( 0xff );
}

static int configPort ( const int mcp23017 )
{
	static uint8_t bank = 0; 
	uint8_t buf[ 2 ];

	if ( bank == 0 )
	{
		buf[ 0 ] = B0_IOCON;
	}
	else
	{
		buf [ 0 ] = IOCON;
	}

	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	buf[ 1 ] |= 0x80; // pass to bank 1
	buf[ 1 ] &= ~0x40; // int A for port A & int B for port B
	buf[ 1 ] |= 0x20; // sequencial mode not enabled 
	buf[ 1 ] |= 0x02; // int is an active high pin

	return ( 0 );
}

int gpioSetDir ( const int mcp23017, const char port, const uint8_t id, const mcp23017GpioMode mode )
{ // IODIR
	uint8_t dir[ 2 ];
	uint8_t lat[ 2 ];
	dir[ 0 ] = getPort ( port );

	if ( ( dir[ 0 ] == 0xff ) ||
		( id > 7 ) )
	{
		errno = EINVAL;
		return ( __LINE__ );
	}

	dir[ 0 ] |= IODIR;

	write ( mcp23017, dir, 1 );
	read ( mcp23017, &dir[ 1 ], 1 );

	if ( !mode )
	{ // mode output
		dir[ 1 ] &= ~( 1 << id );
	}
	else
	{ // mode input
		dir[ 1 ] |= 1 << id;
	}

	if ( write ( mcp23017, dir, 2 ) != 2 )
	{
		return ( __LINE__ );
	}
	
	return ( 0 );
}

int gpioInputSetPol ( const int mcp23017, const char port , const uint8_t id, const uint8_t mode )
{ // IPOL
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( ( buf[ 0 ] == 0xff ) ||
		( id > 7 ) )
	{
		errno = EINVAL;
		return ( __LINE__ );
	}

	buf[ 0 ] |= IPOL;
	
	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	if ( !mode )
	{ // normal mode
		buf[ 1 ] &= ~( 1 << id );
	}
	else
	{ // inverted mode
		buf[ 1 ] |= 1 << id;
	}
	return ( write ( mcp23017, buf, 2 ) != 2 );
}

int gpioSetInterrupts ( const int mcp23017, const char port, const uint8_t id, const mcp23017InterruptType mode )
{ // GPINTEN + DEFVAL + INTCON
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( ( buf[ 0 ] == 0xff ) ||
		( id > 7 ) )
	{
		errno = EINVAL;
		return ( __LINE__ );
	}

	switch ( mode )
	{
		case mcp23017_IT_off:
		{ // it disabled
			buf[ 0 ] |= GPINTEN;

			write ( mcp23017, buf, 1 );
			read ( mcp23017, &buf[ 1 ], 1 );

			buf [ 1 ] &= ~( 1 << id );
			return ( write ( mcp23017, buf, 2 ) != 2 );
		}
		case mcp23017_IT_edges:
		{ // it enabled on edges
			buf[ 0 ] |= INTCON;
			
			write ( mcp23017, buf, 1 );
			read ( mcp23017, &buf[ 1 ], 1 );

			buf [ 1 ] &= ~( 1 << id );
			if ( write ( mcp23017, buf, 2 ) != 2 )
			{
				return ( __LINE__ );
			}
			buf[ 0 ] &= ~INTCON;
			buf[ 0 ] |= GPINTEN;

			write ( mcp23017, buf, 1 );
			read ( mcp23017, &buf[ 1 ], 1 );

			buf [ 1 ] |= 1 << id;
			return ( write ( mcp23017, buf, 2 ) != 2 );
		}
		case mcp23017_IT_high:
		case mcp23017_IT_low:
		{ // it enabled on HIGH/LOW state
			buf[ 0 ] |= DEFVAL;

			write ( mcp23017, buf, 1 );
			read ( mcp23017, &buf[ 1 ], 1 );

			if ( mode == mcp23017_IT_high )
			{
				buf [ 1 ] &= ~(1 << id);
			}
			else
			{
				buf [ 1 ] |= 1 << id;
			}

			if ( write ( mcp23017, buf, 2 ) != 2 )
			{
				return ( __LINE__ );
			}

			buf[ 0 ] &= ~DEFVAL;
			buf[ 0 ] |= INTCON;

			write ( mcp23017, buf, 1 );
			read ( mcp23017, &buf[ 1 ], 1 );

			buf [ 1 ] |= 1 << id;
			if ( write ( mcp23017, buf, 2 ) != 2 )
			{
				return ( __LINE__ );
			}
			buf[ 0 ] &= ~INTCON;
			buf[ 0 ] |= GPINTEN;

			write ( mcp23017, buf, 1 );
			read ( mcp23017, &buf[ 1 ], 1 );

			return ( write ( mcp23017, buf, 2 ) != 2 );
		}
	}
	errno = EINVAL;
	return ( __LINE__ );
}

int gpioSetPullUp ( const int mcp23017, const char port, const uint8_t id, const uint8_t mode )
{ // GPPU
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( ( buf[ 0 ] == 0xff ) ||
		( id > 7 ) )
	{
		errno = EINVAL;
		return ( __LINE__ );
	}

	buf[ 0 ] |= GPPU;
	
	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	if ( !mode )
	{ // pull-up desabled
		buf[ 1 ] &= ~( 1 << id );
	}
	else
	{ // pull-up enabled
		buf[ 1 ] |= 1 << id;
	}
	return ( write ( mcp23017, buf, 2 ) != 2 );
}

uint8_t getInterruptsStatus ( const int mcp23017, const char port )
{ // INTF
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( buf[ 0 ] == 0xff )
	{
		errno = EINVAL;
		return ( 0 );
	}

	buf[ 0 ] |= INTF;
	
	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	return ( buf[ 1 ] );
}

uint8_t getInterruptsValue ( const int mcp23017, const char port )
{ // INTCAP
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( buf[ 0 ] == 0xff )
	{
		errno = EINVAL;
		return ( 0 );
	}
	buf[ 0 ] |= INTCAP;
	
	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	return ( buf[ 1 ] );
}

int gpioSet ( const int mcp23017, const char port, const uint8_t id, const uint8_t status )
{ // OLAT
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( ( buf[ 0 ] == 0xff ) ||
		( id > 7 ) )
	{
		errno = EINVAL;
		return ( __LINE__ );
	}

	buf[ 0 ] |= OLAT;
	
	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	if ( !status )
	{ // set to LOW state
		buf[ 1 ] &= ~( 1 << id );
	}
	else
	{ // set to HIGH state
		buf[ 1 ] |= 1 << id;
	}
	return ( write ( mcp23017, buf, 2 ) != 2 );
}

uint8_t gpioGet ( const int mcp23017, const char port, const uint8_t id )
{ // GPIO
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( ( buf[ 0 ] == 0xff ) ||
		( id > 7 ) )
	{
		errno = EINVAL;
		return ( 0xff );
	}

	buf[ 0 ] |= GPIO;
	
	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	return ( ( buf[ 1 ] & ( 1 << id ) ) != 0 );
}

int portSet ( const int mcp23017, const char port, const uint8_t status )
{ // OLAT
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( buf[ 0 ] == 0xff )
	{
		errno = EINVAL;
		return ( __LINE__ );
	}

	buf[ 0 ] |= OLAT;
	buf[ 1 ] = status;

	return ( write ( mcp23017, buf, 2 ) != 2 );
}

uint8_t portGet ( const int mcp23017, const char port )
{ // GPIO
	uint8_t buf[ 2 ];
	buf[ 0 ] = getPort ( port );

	if ( buf[ 0 ] == 0xff )
	{
		errno = EINVAL;
		return ( 0 );
	}

	buf[ 0 ] |= GPIO;
	
	write ( mcp23017, buf, 1 );
	read ( mcp23017, &buf[ 1 ], 1 );

	return ( buf[ 1 ] );
}

int openMCP23017 ( const char busName[], const uint8_t address, int * const mcp23017 )
{
	*mcp23017 = open ( busName, O_RDWR );
	if ( *mcp23017 < 0 )
	{
		return ( __LINE__ );
	}

	if ( ioctl ( *mcp23017, I2C_SLAVE, address ) < 0 )
	{
		return ( __LINE__ );
	}

	if ( configPort ( *mcp23017 ) )
	{
		return ( __LINE__ );
	}

	return ( 0 );
}

int closeMCP23017 ( const int mcp23017 )
{
	return ( close ( mcp23017 ) );
}