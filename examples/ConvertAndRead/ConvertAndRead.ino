#include <Wire.h>
#include <MCP342x.h>

/* Demonstate the use of read() and convert(). If read() is called
 * immediately after convert then the conversion will not have
 * completed. Two approaches to avoid this problem are possible, use
 * delay() or similar to wait a fixed amount of time, or to
 * continually read the device and check the config result.
 */


// 0x68 is the default address for all MCP342x devices
uint8_t address = 0x69;
MCP342x adc = MCP342x(address);

void setup(void)
{
  Serial.begin(9600);
  Wire.begin();

  // To do: general call reset
  MCP342x::generalCallReset();
  
  // Check device present
  Wire.requestFrom(address, (uint8_t)1);
  if (!Wire.available()) {
    Serial.print("No device found at address ");
    Serial.println(address, HEX);
    while (1)
      ;
  }

  MCP342x::generalCallReset();
  delay(1); // MC342x needs 300us to settle
}

void loop(void)
{
  convertAndReadUseDelay();
  convertAndReadNoDelay();

  delay(1000);
}

void convertAndReadUseDelay(void)
{
  long value = 0;
  uint8_t config;
  uint8_t err;
  Serial.println("===== convertAndReadUseDelay() =====");
  // Call convert(). For debugging purposes print the return value.
  err = adc.convert(MCP342x::channel0, MCP342x::oneShotMode, MCP342x::res16, MCP342x::gain1);
  Serial.print("Convert error: ");
  Serial.println(err);

  // Ensure conversion has completed. See the datasheet for conversion times.
  delay(500);

  // Call read(). For debugging purposes print the return value.
  err = adc.read(value, config);
  Serial.print("Value: ");
  Serial.println(value);
  Serial.print("Config: 0x");
  Serial.println(config, HEX);
  Serial.print("Convert error: ");
  Serial.println(err);

  if (config & MCP342x::notReadyMask)
    Serial.println("Conversion was not completed");
}

// No delay in the function name means without using delay(), it
// doesn't mean there is no conversion delay!
void convertAndReadNoDelay(void)
{
  long value = 0;
  uint8_t config;
  uint8_t err;
  Serial.println("===== convertAndReadNoDelay() =====");
  // Call convert(). For debugging purposes print the return value.
  err = adc.convert(MCP342x::channel0, MCP342x::oneShotMode, MCP342x::res16, MCP342x::gain1);
  Serial.print("Convert error: ");
  Serial.println(err);

  do {
    err = adc.read(value, config);
  } while (err == MCP342x::errorNone && (config & MCP342x::notReadyMask));
    
  // For debugging purposes print the return value.
  Serial.print("Value: ");
  Serial.println(value);
  Serial.print("Config: 0x");
  Serial.println(config, HEX);
  Serial.print("Convert error: ");
  Serial.println(err);
}
