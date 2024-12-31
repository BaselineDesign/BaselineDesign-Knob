#include QMK_KEYBOARD_H
#include "print.h"
#include "i2c_master.h"

// Define the pins connected to RGB LEDs
#define RGB_PIN_RED B6
#define RGB_PIN_GREEN C6
#define RGB_PIN_BLUE D7

void custom_config_set_value(uint8_t *data);
void custom_config_get_value(uint8_t *data);
void custom_config_save(void);
void setColor(void);

typedef union {
    uint32_t raw;
    struct{
        uint8_t color;
        int16_t sensitivity;
    };
} custom_config_t;

custom_config_t custom_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_MEDIA_PREV_TRACK,   KC_MEDIA_PLAY_PAUSE,   KC_MEDIA_NEXT_TRACK
    )
};

#if defined(ENCODER_MAP_ENABLE)
    const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {  ENCODER_CCW_CW(
        KC_VOLD, KC_VOLU
        )},
};
#endif

void eeconfig_init_user(void) {
    custom_config.sensitivity = 82;
    custom_config.color = 0;
    eeconfig_update_user(custom_config.raw);
}

void keyboard_pre_init_user(void){
    // Initialize the I2C driver
    i2c_init();
    
    // You can print a message to verify I2C initialization
    uprintf("I2C Initialized\n");
    
    // Set the RGB pins as outputs
    setPinOutput(RGB_PIN_RED);
    setPinOutput(RGB_PIN_GREEN);
    setPinOutput(RGB_PIN_BLUE);

    //turns leds on
    setColor(); 
}

uint8_t data_high[1];  // Array to store AS5600 high byte
uint8_t data_low[1];   // Array to store AS5600 low byte
uint8_t device_address = (0x36 << 1);  // 7-bit I2C address of AS6500
uint8_t register_address_high = 0x0E;  // Register to read from for high byte
uint8_t register_address_low = 0x0F;   // Register to read from for low byte
uint16_t timeout = 100;  // i2c Timeout in milliseconds
int16_t lastValue = 0;  // Stores the last value of the encoder that triggered a keystroke
bool sleeping = false; //tracks if the controller is sleeping and turns off lights

//AS5600 stores its angle measurements in two registrys that need to be recorded separately and then combined
void encoder_driver_task(void) {
    // Perform the read operation for the high byte
    i2c_status_t highVal = i2c_read_register(
        device_address,          // Device address
        register_address_high,   // Register address for high byte
        data_high,               // Pointer to the buffer for storing high byte
        1,                       // Number of bytes to read
        timeout                  // Timeout in milliseconds
    );

    //Troubleshooting messeges
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
    
    //Troubleshooting messeges
    if (lowVal == I2C_STATUS_SUCCESS) {
        //uprintf("Low byte read successful: %d\n", data_low[0]);  //unncomment if unsure if i2c is working
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
        uprintf("matrix[0] = %d, matrix[1] = %d, matrix[2] = %d, matrix[3] = %d", MATRIX_ROWS, MATRIX_ROWS, MATRIX_ROWS, MATRIX_ROWS);
        wait_ms(100);
    #endif 
    
    //CW Dial controll logic
    if (delta > custom_config.sensitivity) {
        lastValue = scaled_angle;  // Update last value
        encoder_queue_event(0, true); // send a CW encoder event
    } 
    
    //CCW Dial controll logic
    else if (delta < -custom_config.sensitivity) {
        lastValue = scaled_angle;  // Update last value
        encoder_queue_event(0, false); // send a CCW encoder event
    }
}

//VIA SETTINGS
enum via_buttglow_value {
    id_key_color   = 1,
    id_sensitivity  = 2
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
}

void custom_config_set_value(uint8_t *data) {
    // data = [ value_id, value_data ]
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_key_color: {
            custom_config.color = *value_data;
            setColor();
            break;
        }

        case id_sensitivity: {
             custom_config.sensitivity = *value_data;
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
            *value_data = custom_config.color;
            break;
        }
        case id_sensitivity: {
            *value_data = custom_config.sensitivity;
            break;
        }                
    }
}

void matrix_init_user(void) {
    custom_config.raw = eeconfig_read_user();
    setColor();
}

void custom_config_save(void) {
    eeconfig_update_user(custom_config.raw);
}

void setColor() {
    //OFF
    if(sleeping == true){
        writePinLow(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
    }
    //RED
    else if(custom_config.color == 0){
        writePinHigh(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
    }
    //GREEN
    else if(custom_config.color == 1){
        writePinLow(RGB_PIN_RED);
        writePinHigh(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
    }
    //BLUE
    else if(custom_config.color == 2){
        writePinLow(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinHigh(RGB_PIN_BLUE);
    }
    //PURPLE
    else if(custom_config.color == 3){
        writePinHigh(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinHigh(RGB_PIN_BLUE);
    }
    //YELLOW
    else if(custom_config.color == 4){
        writePinHigh(RGB_PIN_RED);
        writePinHigh(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
    }
    //CYAN
    else if(custom_config.color == 5){
        writePinLow(RGB_PIN_RED);
        writePinHigh(RGB_PIN_GREEN);
        writePinHigh(RGB_PIN_BLUE);
    }
    //OFF
    else{
        writePinLow(RGB_PIN_RED);
        writePinLow(RGB_PIN_GREEN);
        writePinLow(RGB_PIN_BLUE);
    }
}

void suspend_power_down_kb(void) {
    sleeping = true;
    setColor();
}

void suspend_wakeup_init_kb(void) {
    sleeping = false;
    setColor();
}