#include "Wire.h"
#include "MCP342x.h"

#include "Arduino.h"

uint8_t MCP342x::generalCallReset(void)
{
  Wire.beginTransmission(0x00);
  Wire.write(0x06);
  return Wire.endTransmission();
}

uint8_t MCP342x::generalCallLatch(void)
{
  Wire.beginTransmission(0x00);
  Wire.write(0x04);
  return Wire.endTransmission();
}

uint8_t MCP342x::generalCallConversion(void)
{
  Wire.beginTransmission(0x00);
  Wire.write(0x08);
  return Wire.endTransmission();
}

MCP342x::MCP342x(uint8_t add) : address(add), config(0)
{
  ;
}

/** Initiate a conversion by writing to the configuration register
 */
MCP342x::error_t MCP342x::convert(channel_t channel, mode_t mode, resolution_t resolution, gain_t gain)
{
  config = (channel | mode | resolution | gain);
  Wire.beginTransmission(address);
  Wire.write(config | 0x80); // Set the convert bit (0x80)
  if (Wire.endTransmission())
    return errorConvertFailed;
  else
    return errorNone;
}

MCP342x::error_t MCP342x::read(long &result, uint8_t& readConfig) const
{
  uint8_t len;     // Number of bytes for result and configuration register
  long signBit;    // Location of sign bit
  long signExtend; // Bits to be set if sign is set
  switch (config & res18) {
  case res12:
    len = 3;
    signBit = 0x800;
    signExtend = 0xFFFFF000;
    break;
  case res14:
    len = 3;
    signBit = 0x2000;
    signExtend = 0xFFFFC000;
    break;
  case res16:
    len = 3;
    signBit = 0x8000;
    signExtend = 0xFFFF0000;
    break;
  case res18:
    len = 4;
    signBit = 0x20000;
    signExtend = 0xFFFC0000;
    break;
  }

  Wire.requestFrom(address, len);
  if (Wire.available() != len)
    return errorReadFailed;

  result = 0;
  for (uint8_t i = 1; i < len; ++i) {
    uint8_t b = Wire.read();
    result <<= 8;
    result |= (long)b;
  }
  
  readConfig = Wire.read();
  
  // Fill/blank remaining bits  
  if ((result & signBit) != 0)
    result |= signExtend; // Sign bit is set, sign-extend
  
  return errorNone;  
}

MCP342x::error_t MCP342x::convertAndRead(channel_t channel, mode_t mode, resolution_t resolution, gain_t gain, unsigned long timeout, long &result, uint8_t& readConfig)
{
  error_t err = convert(channel, mode, resolution, gain);
  if (err != errorNone)
    return err;
  unsigned long t = micros();
  do {
    err = read(result, readConfig);
  } while (micros() - t < timeout && err == errorNone && (config & notReadyMask));
  return err;
}
