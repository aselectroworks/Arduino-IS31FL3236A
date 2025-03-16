#include "IS31FL3236A.h"
#include "HardwareSerial.h"

#if defined(ESP32) || defined(ESP8266)
IS31FL3236A::IS31FL3236A(int8_t sda, int8_t scl, IS31FL3236A_AD_PIN_t ad, int8_t sdb) : _sda(sda), _scl(scl), _ad(ad), _sdb(sdb) {
#ifdef IS31FL3236A_DEBUG
    DEBUG_PRINTER.begin(115200);
#endif
}
#else
IS31FL3236A::IS31FL3236A(IS31FL3236A_AD_PIN_t ad, int8_t sdb) : _ad(ad), _sdb(sdb) {
#ifdef IS31FL3236A_DEBUG
    DEBUG_PRINTER.begin(115200);
#endif
}
#endif

IS31FL3236A::~IS31FL3236A() {}

void IS31FL3236A::begin() {
#if defined(ESP32) || defined(ESP8266)
    Wire.begin(_sda, _scl);
#else
    Wire.begin();
#endif
    pinMode(_sdb, OUTPUT);
    digitalWrite(_sdb, HIGH);
}

void IS31FL3236A::shutdown(bool en) {
    uint8_t data = en ? 0x00 : 0x01;
    writeByte(IS31FL3236A_REG_SHUTDOWN, data);
}

void IS31FL3236A::setPwm(uint8_t ch, uint8_t val) { writeByte(IS31FL3236A_REG_PWM_BASE + ch, val); }

void IS31FL3236A::updatePwm() { writeByte(IS31FL3236A_REG_PWMUPDATE, 0x00); }

void IS31FL3236A::setLedControl(uint8_t ch, IS31FL3236A_IOUT_t sl, bool en) {
    writeByte(IS31FL3236A_REG_LEDCONTROL_BASE + ch, (sl << 1) + (uint8_t)en);
}

void IS31FL3236A::setGlobalControl(bool g_en) {
    uint8_t data = g_en ? 0x01 : 0x00;
    writeByte(IS31FL3236A_REG_GLOBALCONTROL, data);
}

void IS31FL3236A::setOutputFrequency(IS31FL3236A_OUTPUT_FREQUENCY_t freq) { writeByte(IS31FL3236A_REG_OUTPUTFREQUENCY, freq); }

void IS31FL3236A::reset() { writeByte(IS31FL3236A_REG_RESET, 0x00); }

uint8_t IS31FL3236A::writeMultiByte(uint8_t addr, uint8_t* data, uint8_t size) {
    Wire.beginTransmission(IS31FL3236A_I2C_BASE_ADDR + _ad);
    DEBUG_PRINTF("writeI2C reg 0x%02x\n", addr)
    Wire.write(addr);
    for (uint8_t i = 0; i < size; i++) {
        DEBUG_PRINTF(" -> data[%d]:0x%02x\n", i, data[i])
        Wire.write(data[i]);
    }
    return Wire.endTransmission();
}

uint8_t IS31FL3236A::writeWord(uint8_t addr, uint16_t data) {
    uint8_t txData[2] = {(uint8_t)(data >> 8), (uint8_t)(data & 0x00FF)};
    DEBUG_PRINTF("write word = %d\n", data)
    return writeMultiByte(addr, txData, 2);
}

uint8_t IS31FL3236A::writeByte(uint8_t addr, uint8_t data) {
    DEBUG_PRINTF("write byte = %d\n", data)
    return writeMultiByte(addr, &data, 1);
}

