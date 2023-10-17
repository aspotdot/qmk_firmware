#pragma once


/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Audio */
#define AUDIO_PIN A5
#define AUDIO_PIN_ALT A4
#define AUDIO_PIN_ALT_AS_NEGATIVE

/* OLED */
#define EEPROM_I2C_24LC256
#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B6
#define I2C1_SDA_PIN B7
// #define I2C1_SCL_PAL_MODE 1
// #define I2C1_SDA_PAL_MODE 1
// #define I2C1_TIMINGR_PRESC 0x00U
// #define I2C1_TIMINGR_SCLDEL 0x03U
// #define I2C1_TIMINGR_SDADEL 0x01U
// #define I2C1_TIMINGR_SCLH 0x03U
// define I2C1_TIMINGR_SCLL 0x09U

// #define OLED_BRIGHTNESS 255
#define OLED_FONT_H "glcdfont_as.c"
#define OLED_TIMEOUT 30000


/* Taps (encoder tap_code) */
#define TAP_CODE_DELAY 10

/* Tap dance time */
#define TAPPING_TERM 175

#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED 7
#define MOUSEKEY_WHEEL_DELAY 0
