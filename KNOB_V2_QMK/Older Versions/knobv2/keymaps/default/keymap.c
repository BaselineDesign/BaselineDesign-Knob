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

bool sleeping = false; //tracks if the controller is sleeping and turns off lights

uint8_t data[2];
uint8_t device_address = (0x36 << 1);  // 7-bit I2C address of AS6500
uint16_t register_address_high = 0x0C;  // Register to read from for high byte
uint16_t length = 2;
uint16_t timeout = 100;  // i2c Timeout in milliseconds
int16_t lastValue = 0;  // Stores the last value of the encoder that triggered a keystroke

//AS5600 stores its angle measurements in two registrys that need to be recorded separately and then combined
void encoder_driver_task(void) {
    
    //single read testing
    i2c_status_t comboVal = i2c_read_register(
        device_address,          // Device address
        register_address_high,
        data,                // Pointer to the buffer for storing low byteab
        length,                       // Number of bytes to read
        timeout                  // Timeout in milliseconds
    );
    if (comboVal == I2C_STATUS_SUCCESS) {
        //uprintf("Low byte read successful: %d\n", data_low[0]);  //unncomment if unsure if i2c is working
    } else if (comboVal == I2C_STATUS_TIMEOUT) {
        uprintf("Low byte read operation timed out.\n");
    } else if (comboVal == I2C_STATUS_ERROR) {
        uprintf("Low byte read operation failed with an error.\n");
    }
    
    // Combine the high and low byte to calculate the scaled angle
    int16_t scaled_angle = ((uint16_t)data[0] << 8) | data[1];

    //delta is now equal to the difference between the value that last triggered a keystroke and the most recent measured value
    int16_t delta = scaled_angle - lastValue; 

    
    // Handle the wraparound case when the value goes from 4096->0
    if (delta > 2047) {
        delta = 0;  // Angle wrapped around in the positive direction
        lastValue = scaled_angle;

    } else if (delta < -2047) {
        delta = 0;  // Angle wrapped around in the negative direction
        lastValue = scaled_angle;
    }
    
    //CW Dial controll logic
    if (delta > custom_config.sensitivity) {
        lastValue = scaled_angle;  // Update last value
        encoder_queue_event(0, true); // send a CW encoder event
        //wait_ms(40);

    } 
    
    //CCW Dial controll logic
    else if (delta < -custom_config.sensitivity) {
        lastValue = scaled_angle;  // Update last value
        encoder_queue_event(0, false); // send a CCW encoder event
        //wait_ms(40);
    }

    #ifdef CONSOLE_ENABLE
        if(delta > custom_config.sensitivity || delta < -custom_config.sensitivity ){
            uprintf(" Scaled angle: %d last Value: %d dataHigh: %d dataLow: %d delta: %d\n",  scaled_angle, lastValue, data[0], data[1], delta);
            //wait_ms(100);
        }
    #endif 
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