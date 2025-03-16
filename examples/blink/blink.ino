#include "IS31FL3236A.h"

#define SDA_PIN 9
#define SCL_PIN 46
#define AD_PIN IS31FL3236A_AD_PIN_GND
#define SDB_PIN -1

IS31FL3236A is31fl3236a(SDA_PIN, SCL_PIN, AD_PIN, SDB_PIN);

void setup() {
    is31fl3236a.begin();
    is31fl3236a.shutdown(false);
    for (uint8_t i = 0; i < 36; i++) {
        is31fl3236a.setLedControl(i, IS31FL3236A_IMAX_DIV4, true);
    }
}

void loop() {
    for (uint8_t i = 0; i < 36; i++) {
        is31fl3236a.setPwm(i, 0x3F);
        is31fl3236a.updatePwm();
        delay(250);
    }
    for (uint8_t i = 0; i < 36; i++) {
        is31fl3236a.setPwm(i, 0x00);
        is31fl3236a.updatePwm();
        delay(250);
    }
}