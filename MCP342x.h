#ifndef MCP342X_h
#define MCP342X_h

class MCP342x {
public:
  enum channel_t {
    channel0 = 0x00,
    channel1 = 0x20,
    channel2 = 0x40,
    channel3 = 0x60,
  };
  
  enum mode_t {
    oneShotMode = 0x00,
    continousMode = 0x10,
  };

  enum resolution_t {
    res12 = 0x00,
    res14 = 0x04,
    res16 = 0x08,
    res18 = 0x0c,
  };
  
  enum gain_t {
    gain1 = 0x00,
    gain2 = 0x01,
    gain4 = 0x02,
    gain8 = 0x03,
  };

  static const uint8_t notReadyMask = 0x80;
  
  enum error_t {
    errorNone = 0,
    errorConvertFailed = 1,
    errorReadFailed = 2,
  };

  static uint8_t generalCallReset(void);
  static uint8_t generalCallLatch(void);
  static uint8_t generalCallConversion(void);

  MCP342x(uint8_t address);

  /** Return the I2C address used for communicating with this device.
   */
  uint8_t getAddress(void) const {
    return address;
  }

  /** Return the last configuration setting written to the device.
   */
  uint8_t getConfig(void) const {
    return config;
  }


  /** Instruct the MCP342x device to begin a conversion.
   * @param channel The ADC channel, one of chnnael0, channel1,
   * channel2 or * channel3. Not all supported devices have 4
   * channels.
   * @param mode The conversion mode, oneShotMode or continousMode.
   * @param resolution Number of bits in the result, one of res12,
   * res14, res16 or res18. Not all devices support 18-bit resolution.
   * @param gain The gain setting of the programmable gain amplifier,
   * one of gain1, gain2, gain4 or gain8.
   * @return Value indicating error (if any).
   */
  error_t convert(channel_t channel, mode_t mode, resolution_t resolution, gain_t gain);

  /** Read the sample value from the MCP342x device.
   * @param result The signed result. Valid
   * @param config The contents of the configuration register.
   * @return Value indicating error (if any).
   */
  error_t read(long &result, uint8_t& config) const;

  
private:
  uint8_t address;
  // For easy readout need to know whether 18 bit mode was selected
  uint8_t config;
};

#endif
