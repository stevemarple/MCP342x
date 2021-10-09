#include <gtest/gtest.h>
#include "Arduino.h"
#include "Wire.h"
#include "MCP342x.h"
#include "I2CDevice.h"
#include <list>

unsigned long cpu_time;

unsigned long micros()
{
    return cpu_time;
}

void delay(unsigned long ms)
{
    cpu_time += (ms * 1000);
}

void delayMicroseconds(unsigned long us)
{
    cpu_time += us;
}

uint8_t default_address = 0x12;

class MockWire : public FakeWire {
    I2CDevice& deviceOnBus;
    std::list<uint8_t> writeBytes;
    uint8_t writeAddress;

public:
    MockWire(I2CDevice& device) : deviceOnBus(device) {}

    unsigned char endTransmission() {
        if(deviceOnBus.address() != writeAddress) {
            return 1;
        }

        int reg = 0;
        for (const uint8_t & it : writeBytes) {
            deviceOnBus.write(reg++, it);
        }

        return 0;
    }

    void beginTransmission(unsigned char address) {
        writeAddress = address;
        writeBytes.clear();
    }

    unsigned char available() {
        return readLen;
    }

    void write(unsigned char byte) {
        writeBytes.push_back(byte);
    }

    int readIx;

    unsigned char read() {
        cpu_time += 1000;
        uint8_t value;
        deviceOnBus.read(readIx++, value);
        return value;
    }

    unsigned char readLen;

    void requestFrom(unsigned char address, unsigned char len) {
        readLen = len;
        readIx = 0;
    }
};

_Wire Wire;
FakeWire *_wire;

TEST(ConvertAndRead, ReturnsOkWhenReadDelayedButWithinTimeout)
{
  cpu_time = 0;
  MCP342x adc(default_address);
  MCPDevice mock(default_address, 67000);
  _wire = new MockWire(mock);

  long value = 0;
  MCP342x::Config status;
  auto err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot,
           MCP342x::resolution16, MCP342x::gain1, 1000000, value, status);

  ASSERT_EQ(MCP342x::errorNone, err);
  delete _wire;
}

TEST(ConvertAndRead, ReturnsTimeoutWhenReadDelayedOverTimeout)
{
  cpu_time = 0;
  MCP342x adc(default_address);
  MCPDevice mock(default_address, 110000);
  _wire = new MockWire(mock);

  long value = 0;
  MCP342x::Config status;
  uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot,
           MCP342x::resolution16, MCP342x::gain1, 100000, value, status);

  EXPECT_EQ(err, MCP342x::errorReadTimeout);
  delete _wire;
}

TEST(ConvertAndRead, ReturnsOkWhenReadDelayedButWithinTimeoutTickOverflow)
{
    cpu_time = std::numeric_limits<unsigned long>::max();
    MCP342x adc(default_address);
    MCPDevice mock(default_address, 67000);
    _wire = new MockWire(mock);

    long value = 0;
    MCP342x::Config status;
    auto err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot,
        MCP342x::resolution16, MCP342x::gain1, 1000000, value, status);

    ASSERT_EQ(MCP342x::errorNone, err);
    delete _wire;
}

TEST(ConvertAndRead, ReturnsTimeoutWhenReadDelayedOverTimeoutTickOverflow)
{
    cpu_time = std::numeric_limits<unsigned long>::max();
    MCP342x adc(default_address);
    MCPDevice mock(default_address, 110000);
    _wire = new MockWire(mock);

    long value = 0;
    MCP342x::Config status;
    uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot,
        MCP342x::resolution16, MCP342x::gain1, 100000, value, status);

    EXPECT_EQ(err, MCP342x::errorReadTimeout);
    delete _wire;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
