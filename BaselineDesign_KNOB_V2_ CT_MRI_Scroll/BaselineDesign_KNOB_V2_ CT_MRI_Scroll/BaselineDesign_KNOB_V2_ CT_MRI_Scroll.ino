
/*
Modify "SWITCH ACTIONS" to change what keystroke is sent when interacting with the dial or the switches
A list of possible media controls can be found here:
https://github.com/NicoHood/HID/wiki/Consumer-API

A list of possible keystrokes and instructions on how to send them can be found here:
https://github.com/NicoHood/HID/wiki/Keyboard-API

A list of media controller information can be found here:
https://github.com/NicoHood/HID/wiki/Consumer-API

see https://github.com/NicoHood/HID for the complete documentation of what can be controlled

If you cant upload code to your KNOB verify the following:
  -these librarys are installed:
    HID-Project.h
    ezButton.h
    AS5600.h
    
  -Under Tools->Board "Arduino Micro" is selected
  -The correct port is selected under Tools->Port
  -you are using a Type-C cable that supports data transmission and is not charge only

 Happy Customizing!
 -Connor Weller
 */

//LIBRARY CONFIGURATION
#include <HID-Project.h>
#include <ezButton.h>
#include <AS5600.h>
#include <EEPROM.h>


//USER CONFIGURABLE OPTIONS
  //SWITCH ACTIONS
  #define leftSwitch MEDIA_PREVIOUS
  #define centerSwitch MEDIA_PLAY_PAUSE
  #define rightSwitch MEDIA_NEXT
  void scrollCWMode1() {Mouse.move(0, 0, -1);}
  void scrollCCWMode1() {Mouse.move(0, 0, 1);}

  //SENSITIVITY ADJUSTMENTS
  #define volumeSensitivity 80
  #define holdTime 500
  #define switchDebounce 30

  //PIN CONFIGURATION
  ezButton leftSwitchObject(9);
  ezButton centerSwitchObject(8);
  ezButton rightSwitchObject(7);
  #define red 10
  #define green 5
  #define blue 6
  AS5600 as5600;   //  use default Wire
  int state;

void setup() {
  pinMode (red,OUTPUT);
  pinMode (green,OUTPUT);
  pinMode (blue,OUTPUT);
   
  leftSwitchObject.setDebounceTime(switchDebounce); //set debounce time 
  centerSwitchObject.setDebounceTime(switchDebounce); 
  rightSwitchObject.setDebounceTime(switchDebounce); 
  
  Mouse.begin();
  Consumer.begin();   //Initializes the HID Library
  Wire.begin(4);      //set Encoder direction pin and start Wire
  as5600.begin();     //Start as5600
  Serial.begin(9600); //Opens the serial connection used for communication with the PC.
  changeState();

}

void loop() {
  
//This part of the code is responsible for the actions when you rotate the encoder
  static long encoderOld = 0; //where the old encoder reading is stored
  static uint32_t lastTime = 0; //where the last value for millis is stored
  as5600.getCumulativePosition();
  
  if (millis() - lastTime >= 5){ 
    lastTime = millis();
    long encoderNew = as5600.getCumulativePosition();
    if (encoderNew-encoderOld > volumeSensitivity){
        scrollCWMode1();
      encoderOld=encoderNew;
    }
    else if (abs(encoderNew-encoderOld) > volumeSensitivity){
        scrollCCWMode1();
      encoderOld=encoderNew;
    }
  }

  //resets rotation before overflow
  if (as5600.getRevolutions() >= 10){
    as5600.resetPosition();
  }
  
//This part handles all the button presses
  else {
    static bool centerHeld = false;
    static long timePressed;
    static long timeReleased;
    static long ogTimePressed;
    leftSwitchObject.loop();
    centerSwitchObject.loop();
    rightSwitchObject.loop();
        
    if(leftSwitchObject.isPressed())
      Keyboard.write('a');
  
    else if(centerSwitchObject.isPressed()){
          Keyboard.write('r');
    }
  
    else if(rightSwitchObject.isPressed())
      Keyboard.write('e');    
  }
}

//this runs everytime the center button is held to change modes
void changeState(){

}
