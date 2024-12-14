#include QMK_KEYBOARD_H
#include "i2c_master.h"

#define AS5600_I2C_ADDRESS 0x36  // AS5600 I2C address
#define AS5600_ANGLE_REG 0x0C    // Register for angle measurement

uint16_t last_angle = 0;  // Store the last angle to detect changes

// Function to read the angle from AS5600 sensor
uint16_t read_as5600_angle(void) {
    uint8_t data[2];  // Buffer to store the two bytes read from AS5600

    // Start I2C communication to write to the sensor
    if (i2c_transmit(AS5600_I2C_ADDRESS, (uint8_t[]){AS5600_ANGLE_REG}, 1, false)) {
        // Read 2 bytes from the AS5600 sensor
        if (i2c_receive(AS5600_I2C_ADDRESS, data, 2, false)) {
            // Combine MSB and LSB into a 12-bit value and return it
            return (data[0] << 8 | data[1]) & 0x0FFF;
        }
    }

    return 0;  // Return 0 if the I2C communication fails
}

// Function to check the encoder rotation and adjust the volume
void check_encoder_and_adjust_volume(void) {
    uint16_t current_angle = read_as5600_angle();
    int16_t delta = (int16_t)(current_angle - last_angle);

    // Handle wrapping from 4095 to 0 or vice versa
    if (delta > 2048) {
        delta -= 4096;
    } else if (delta < -2048) {
        delta += 4096;
    }

    // If the knob has rotated clockwise
    if (delta > 10) {
        tap_code(KC_VOLU);  // Send a "volume up" key press
    }
    // If the knob has rotated counterclockwise
    else if (delta < -10) {
        tap_code(KC_VOLD);  // Send a "volume down" key press
    }

    last_angle = current_angle;  // Store the current angle for the next comparison
}

// This function runs in the background to continuously check the encoder
void matrix_scan_user(void) {
    check_encoder_and_adjust_volume();
}

// Keymap definition
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK
    )
};
