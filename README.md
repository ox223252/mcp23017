# mcp23017
mcp23017 basic driver

## Not fully tested, work in progress


```C
#inlcude <stdint.h>
#include "lib/mcp23017/mcp23017.h"

int main ( void )
{
	int fd = 0;
	uint8_t pinID = 0;

	openMCP23017 ( "/dev/i2c-1", 0x20, &fd );
	gpioSetDir ( fd, 'A', pinID, mcp23017_OUTPUT );
	
	uint8_t status = gpioGet ( fd, 'A', pinID );
	gpioSet ( fd, 'A', pinID, ~status );

	return( 0 );
}
```