#include <stdio.h>

class I2CDevice
{
public:
    virtual uint8_t address() = 0;
    virtual bool read(int reg, uint8_t &value) = 0;
    virtual bool write(int reg, uint8_t value) = 0;
};

extern unsigned long cpu_time;

class MCPDevice : public I2CDevice
{
    uint8_t addr;
    unsigned long conversionReadyTime;
    uint8_t config;

    int conversionTimes[4] = {
       4167, 16667, 66667, 266667 
    };

    unsigned long readDelay;

public:
    MCPDevice(uint8_t address, int delay) : addr(address), readDelay(delay) { }

    uint8_t address() {
        return addr;
    }
    
    bool read(int reg, uint8_t &value) {
        value = 0;

        if(reg == 2) {
            value = config & ~0x80;
            if(cpu_time < conversionReadyTime) {
                value |= 0x80;
            }
        }
        return true;
    }

    bool write(int reg, uint8_t value) {
        if(reg == 0)
        {
            config = value;
            if(config & (1<<7)) {
                conversionReadyTime = cpu_time + conversionTimes[(config >> 2) & 0x3] + readDelay;
            }
            return true;
        }
        return false;
    }
};
