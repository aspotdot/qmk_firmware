/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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


// #define OLED_BRIGHTNESS 255
#define OLED_FONT_H "glcdfont_as.c"
#define OLED_TIMEOUT 120000


/* ENCODER */
#define ENCODER_MAP_KEY_DELAY 10

/* Tap dance time */
#define TAPPING_TERM 175

#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED 7
#define MOUSEKEY_WHEEL_DELAY 0

/* caps word */
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD


