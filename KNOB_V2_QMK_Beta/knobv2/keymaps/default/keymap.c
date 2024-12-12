

#include QMK_KEYBOARD_H
#include "print.h"
#include "i2c_master.h"

// Define the pins connected to your RGB LED
#define RGB_PIN_RED B6
#define RGB_PIN_GREEN C6
#define RGB_PIN_BLUE D7

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_MEDIA_PREV_TRACK,   KC_MEDIA_PLAY_PAUSE,   KC_MEDIA_NEXT_TRACK
    )
};
uint16_t volume_key_codes[2] = {KC_VOLU, KC_VOLD};

void keyboard_pre_init_user(void){
    // Initialize the I2C driver
    i2c_init();
    gpio_set_pin_input(D0); // Try releasing special pins for a short time
    gpio_set_pin_input(D1);
    wait_ms(10); // Wait for the release to happen
    // You can print a message to verify I2C initialization
    uprintf("I2C Initialized\n");
    //backlight_enable();
    
    // Set the RGB pins as outputs
    setPinOutput(RGB_PIN_RED);
    setPinOutput(RGB_PIN_GREEN);
    setPinOutput(RGB_PIN_BLUE);

    // Set the RGB color
    // Set pins high to turn on the corresponding color
    writePinHigh(RGB_PIN_RED);   // Turn on red
    //writePinHigh(RGB_PIN_GREEN);  // Turn off green
    //writePinHigh(RGB_PIN_BLUE);  // Turn on blue
}

uint8_t data_high[1];  // Array to store the high byte
uint8_t data_low[1];   // Array to store the low byte
uint8_t device_address = (0x36 << 1);  // 7-bit I2C address of the device
uint8_t register_address_high = 0x0E;  // Register to read from for high byte
uint8_t register_address_low = 0x0F;   // Register to read from for low byte
uint16_t timeout = 100;  // Timeout in milliseconds
int16_t lastValue = 0;  // Stores the last value of the encoder that triggered a keystroke
int16_t stepSize = 82;  // How large of a change in encoder readings to trigger a keystroke (4096 resolution 50 volume clicks on windows so 82) 

//AS5600 stores its angle measurements in two registrys that need to be recorded separately and then combined
void matrix_scan_user(void) {
    // Perform the read operation for the high byte
    i2c_status_t highVal = i2c_read_register(
        device_address,          // Device address
        register_address_high,   // Register address for high byte
        data_high,               // Pointer to the buffer for storing high byte
        1,                       // Number of bytes to read
        timeout                  // Timeout in milliseconds
    );

    if (highVal == I2C_STATUS_SUCCESS) {
        //uprintf("High byte read successful: %d\n", data_high[0]); //unncomment if unsure if i2c is working
    } else if (highVal == I2C_STATUS_TIMEOUT) {
        uprintf("High byte read operation timed out.\n");
    } else if (highVal == I2C_STATUS_ERROR) {
        uprintf("High byte read operation failed with an error.\n");
    }

    // Perform the read operation for the low byte
    i2c_status_t lowVal = i2c_read_register(
        device_address,          // Device address
        register_address_low,    // Register address for low byte
        data_low,                // Pointer to the buffer for storing low byte
        1,                       // Number of bytes to read
        timeout                  // Timeout in milliseconds
    );

    if (lowVal == I2C_STATUS_SUCCESS) {
        //uprintf("Low byte read successful: %d\n", data_low[0]);
    } else if (lowVal == I2C_STATUS_TIMEOUT) {
        uprintf("Low byte read operation timed out.\n");
    } else if (lowVal == I2C_STATUS_ERROR) {
        uprintf("Low byte read operation failed with an error.\n");
    }
    // Combine the high and low byte to calculate the scaled angle
    int16_t scaled_angle = ((uint16_t)data_high[0] << 8) | data_low[0];

    //delta is now equal to the difference between the value that last triggered a keystroke and the most recent measured value
    int16_t delta = scaled_angle - lastValue; 

    // Handle the wraparound case when the value goes from 4096->0
    if (delta > 2047) {
        delta -= 4096;  // Angle wrapped around in the positive direction
    } else if (delta < -2047) {
        delta += 4096;  // Angle wrapped around in the negative direction
    }

    #ifdef CONSOLE_ENABLE
    uprintf(" Scaled angle: %d last Value: %d delta: %d stepsize %d\n",  scaled_angle, lastValue, delta, stepSize);
    wait_ms(100);
    #endif 
    
    // Volume control logic
    if (delta > stepSize) {
        // Increase volume
        register_code(volume_key_codes[0]);  // Send key press
        wait_ms(10);              // Wait for the key to register
        unregister_code(volume_key_codes[0]);  // Release key
        lastValue = scaled_angle;  // Update last value
    } 
    
    else if (delta < -stepSize) {
        // Decrease volume
        register_code(volume_key_codes[1]);  // Send key press
        wait_ms(10);              // Wait for the key to register
        unregister_code(volume_key_codes[1]);  // Release key
        lastValue = scaled_angle;  // Update last value
 
    }
}






//"content": ["id_qmk_rgblight_brightness", 2, 1]
//"content": ["value_key", channel_id, value_id]