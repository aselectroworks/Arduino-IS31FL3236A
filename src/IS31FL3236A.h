#pragma once

#include <Wire.h>
#include <stdint.h>
#include <stdbool.h>

#define IS31FL3236A_I2C_BASE_ADDR 0b0111100
typedef enum {
    IS31FL3236A_AD_PIN_GND = 0b00,
    IS31FL3236A_AD_PIN_VCC = 0b11,
    IS31FL3236A_AD_PIN_SCL = 0b01,
    IS31FL3236A_AD_PIN_SDA = 0b10
} IS31FL3236A_AD_PIN_t;

typedef enum { IS31FL3236A_IMAX = 0b00, IS31FL3236A_IMAX_DIV2 = 0b01, IS31FL3236A_IMAX_DIV3 = 0b10, IS31FL3236A_IMAX_DIV4 = 0b11 } IS31FL3236A_IOUT_t;

typedef enum {
    IS31FL3236A_OFS_3KHZ = 0,
    IS31FL3236A_OFS_22KHZ = 1,
} IS31FL3236A_OUTPUT_FREQUENCY_t;

typedef enum {
    IS31FL3236A_REG_SHUTDOWN = 0x00,
    IS31FL3236A_REG_PWM_BASE = 0x01,
    IS31FL3236A_REG_PWMUPDATE = 0x25,
    IS31FL3236A_REG_LEDCONTROL_BASE = 0x26,
    IS31FL3236A_REG_GLOBALCONTROL = 0x4A,
    IS31FL3236A_REG_OUTPUTFREQUENCY = 0x4B,
    IS31FL3236A_REG_RESET = 0x4F
} IS31FL3236A_REG_t;

// Uncomment to enable debug messages
#define IS31FL3236A_DEBUG

// Define where debug output will be printed
#define DEBUG_PRINTER Serial

// Setup debug printing macros
#ifdef IS31FL3236A_DEBUG
#define DEBUG_PRINT(...)                                                                  \
    {                                                                                     \
        DEBUG_PRINTER.printf("[IS31FL3236A(0x%02x)]: ", IS31FL3236A_I2C_BASE_ADDR + _ad); \
        DEBUG_PRINTER.print(__VA_ARGS__);                                                 \
    }
#define DEBUG_PRINTLN(...)                                                                \
    {                                                                                     \
        DEBUG_PRINTER.printf("[IS31FL3236A(0x%02x)]: ", IS31FL3236A_I2C_BASE_ADDR + _ad); \
        DEBUG_PRINTER.println(__VA_ARGS__);                                               \
    }
#define DEBUG_PRINTF(...)                                                                 \
    {                                                                                     \
        DEBUG_PRINTER.printf("[IS31FL3236A(0x%02x)]: ", IS31FL3236A_I2C_BASE_ADDR + _ad); \
        DEBUG_PRINTER.printf(__VA_ARGS__);                                                \
    }
#else
#define DEBUG_PRINT(...) \
    {                    \
    }
#define DEBUG_PRINTLN(...) \
    {                      \
    }
#define DEBUG_PRINTF(...) \
    {                     \
    }
#endif

class IS31FL3236A {
   public:
#if defined(ESP32) || defined(ESP8266)
    IS31FL3236A(int8_t sda, int8_t scl, IS31FL3236A_AD_PIN_t ad, int8_t sdb);
#else
    IS31FL3236A(IS31FL3236A_AD_PIN_t ad, int8_t sdb);

#endif
    virtual ~IS31FL3236A(void);

    void begin();

    void shutdown(bool en);
    void setPwm(uint8_t ch, uint8_t val);
    void updatePwm();
    void setLedControl(uint8_t ch, IS31FL3236A_IOUT_t sl, bool en);
    void setGlobalControl(bool g_en);
    void setOutputFrequency(IS31FL3236A_OUTPUT_FREQUENCY_t freq);
    void reset();

   private:
    int8_t _sda = -1;
    int8_t _scl = -1;
    IS31FL3236A_AD_PIN_t _ad;
    int8_t _sdb = -1;

    uint8_t writeMultiByte(uint8_t addr, uint8_t* data, uint8_t size);
    uint8_t writeWord(uint8_t addr, uint16_t data);
    uint8_t writeByte(uint8_t addr, uint8_t data);
};
