# MCP342x ADC library

Arduino library to support Microchip MCP342x analogue to digital
converters. The devices utilise the I2C bus. For the low level I2C
protocol this library depends on the Arduino Wire library.

## Supported devices

*   MCP3422: 2 channel, 12, 14, 16, or 18 bit
*   MCP3423: 2 channel, 12, 14, 16, or 18 bit
*   MCP3424: 4 channel, 12, 14, 16, or 18 bit
*   MCP3426: 2 channel, 12, 14, or 16 bit
*   MCP3427: 2 channel, 12, 14, or 16 bit
*   MCP3428: 4 channel, 12, 14, or 16 bit

The MCP3422 and MCP3426 use I2C address 0x68, all other devices can be
configured to use any address in the range 0x68 - 0x6F (inclusive).

## License
Released under the GNU Lesser General Public License, version 2.1. See
license.txt for details.

## Examples

### ListDevices 
List all devices which respond to a request for one byte. Useful to
debug the I2C bus.

### ConvertAndRead
Demonstrate the use of the `convertAndRead()` function.

### ConvertAndReadNoDelay
Demonstrate the use of the `convert()` and `read()` functions to
implement non-blocking analogue to digital conversion. Example makes
use of the built-in LED to demonstrate how a slow 18-bit conversion
need not interfere with other actions.

### GeneralCallConversion 
Demonstrate the general call conversion command which can be used to
instruct several analogue to digital converters to begin conversion
simultanteously.


