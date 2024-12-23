

#include QMK_KEYBOARD_H
#include "print.h"
#include "i2c_master.h"

// Define the pins connected to your RGB LED
#define RGB_PIN_RED B6
#define RGB_PIN_GREEN C6
#define RGB_PIN_BLUE D7

void custom_config_set_value(uint8_t *data);
void custom_config_get_value(uint8_t *data);
void custom_config_save(void);
void setColor(void);

struct custom_config{
    uint8_t color;
    uint16_t CW;
    uint16_t CCW;
} g_custom_config;

struct custom_config g_custom_config = {
    .color = 0,         // Default value for `color`
    .CW = KC_VOLU,      // Default value for `CW`
    .CCW = KC_VOLD      // Default value for `CCW`
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_MEDIA_PREV_TRACK,   KC_MEDIA_PLAY_PAUSE,   KC_MEDIA_NEXT_TRACK
    )
};


void keyboard_pre_init_user(void){
    // Initialize the I2C driver
    i2c_init();
    gpio_set_pin_input(D0); // Try releasing special pins for a short time
    gpio_set_pin_input(D1);
    wait_ms(10); // Wait for the release to happen
    // You can print a message to verify I2C initialization
    uprintf("I2C Initialized\n");
    
    // Set the RGB pins as outputs
    setPinOutput(RGB_PIN_RED);
    setPinOutput(RGB_PIN_GREEN);
    setPinOutput(RGB_PIN_BLUE);

    setColor();
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
        // Do on CW
        lastValue = scaled_angle;  // Update last value
        register_code(g_custom_config.CW);  // Send key press
        wait_ms(10);              // Wait for the key to register
        unregister_code(g_custom_config.CW);  // Release key
    } 
    
    else if (delta < -stepSize) {
        // Do on CCW
        lastValue = scaled_angle;  // Update last value
        register_code(g_custom_config.CCW);  // Send key press
        wait_ms(10);              // Wait for the key to register
        unregister_code(g_custom_config.CCW);  // Release key
 
    }
}






enum via_buttglow_value {
    id_key_color   = 1,
    id_CW_keycode  = 2,
    id_CCW_keycode = 3
};

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    // data = [ command_id, channel_id, value_id, value_data ]
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if (*channel_id == id_custom_channel) {
        
        switch (*command_id) {
            case id_custom_set_value: {
                custom_config_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value: {
                custom_config_get_value(value_id_and_data);
                break;
            }
            case id_custom_save: {
                custom_config_save();
                break;
            }
            default: {
                // Unhandled message.
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }

    // Return the unhandled state
    *command_id = id_unhandled;

    // DO NOT call raw_hid_send(data,length) here, let caller do this
}

void custom_config_set_value(uint8_t *data) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_key_color: {
            g_custom_config.color = *value_data;
            //uprintf(" colorSet: %d\n",  g_custom_config.color);
            //wait_ms(100);
            setColor();
            break;
        }

        case id_CW_keycode: {
            g_custom_config.CW = value_data[0] << 8 | value_data[1];
            //uprintf(" CWSet: %d\n",  g_custom_config.CW);
            //wait_ms(100);
            break;
        }        
        
        case id_CCW_keycode: {
            g_custom_config.CCW = value_data[0] << 8 | value_data[1];
            //uprintf(" CCWSet: %d\n",  g_custom_config.CCW);
            //wait_ms(100);
            break;
        }  
        
    }
}

void custom_config_get_value(uint8_t *data) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_key_color: {
            *value_data = g_custom_config.color;
            //uprintf(" colorGet: %d\n",  g_custom_config.color);
            //wait_ms(100);
            break;
        }
        case id_CW_keycode: {
            value_data[0] = g_custom_config.CW >> 8;
            value_data[1] = g_custom_config.CW & 0xFF;
            //uprintf(" CWGet: %d\n",  g_custom_config.CW);
            //wait_ms(100);
            break;
        }        

        case id_CCW_keycode: {
            value_data[0] = g_custom_config.CCW >> 8;
            value_data[1] = g_custom_config.CCW & 0xFF;
            //uprintf(" CCWGet: %d\n",  g_custom_config.CCW);
            //wait_ms(100);
            break;
        } 
        
    }
}

#define custom_config_EEPROM_ADDR 0x000

void custom_config_save(void) {
    //TODO: Find an appropriate address that doesn't conflict with other stuff
    // eeprom_update_block(&g_custom_config, ((void *)custom_config_EEPROM_ADDR), sizeof(custom_config));
}

//"content": ["id_qmk_rgblight_color", 2, 1]
//"content": ["value_key", channel_id, value_id]



void setColor() {
     if(g_custom_config.color == 0){
        writePinHigh(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
     }
     else if(g_custom_config.color == 1){
        writePinLow(RGB_PIN_RED);
        writePinHigh(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
     }
    else if(g_custom_config.color == 2){
        writePinLow(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinHigh(RGB_PIN_BLUE);
     }
    else if(g_custom_config.color == 3){
        writePinHigh(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinHigh(RGB_PIN_BLUE);
     }
     else if(g_custom_config.color == 4){
        writePinHigh(RGB_PIN_RED);
        writePinHigh(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
     }
     else if(g_custom_config.color == 5){
        writePinLow(RGB_PIN_RED);
        writePinHigh(RGB_PIN_GREEN);
        writePinHigh(RGB_PIN_BLUE);
     }
     else{
        writePinLow(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
     }
}