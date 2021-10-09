class FakeWire
{
public:
    virtual unsigned char endTransmission() = 0;
    virtual void beginTransmission(unsigned char address) = 0;
    virtual unsigned char available() = 0;
    virtual void write(unsigned char byte) = 0;
    virtual unsigned char read() = 0;
    virtual void requestFrom(unsigned char address, unsigned char len) = 0;
};

extern FakeWire* _wire;

class _Wire {
public:
    unsigned char endTransmission() {
        return _wire->endTransmission();
    }

    void beginTransmission(unsigned char address) {
        _wire->beginTransmission(address);
    }

    unsigned char available() {
        return _wire->available();
    }

    void write(unsigned char byte) {
        _wire->write(byte);
    }

    unsigned char read() {
        return _wire->read();
    }

    void requestFrom(unsigned char address, unsigned char len) {
        _wire->requestFrom(address, len);
    }
};

extern _Wire Wire;
