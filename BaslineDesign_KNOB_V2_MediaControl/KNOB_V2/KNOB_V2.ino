
/*
Modify "SWITCH ACTIONS" to change what keystroke is sent when interacting with the dial or the switches
A list of possible media controls can be found here:
https://github.com/NicoHood/HID/wiki/Consumer-API

A list of possible keystrokes and instructions on how to send them can be found here:
https://github.com/NicoHood/HID/wiki/Keyboard-API

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

//USER CONFIGURABLE OPTIONS
  //SWITCH ACTIONS
  #define leftSwitch MEDIA_PREVIOUS
  #define centerSwitch MEDIA_PLAY_PAUSE
  #define rightSwitch MEDIA_NEXT
  #define scrollCW MEDIA_VOLUME_UP
  #define scrollCCW MEDIA_VOLUME_DOWN
  
  //SENSITIVITY ADJUSTMENTS
  #define volumeSensitivity 80
  #define holdTime 350
  #define switchDebounce 30

//LIBRARY CONFIGURATION
#include <HID-Project.h>
#include <ezButton.h>
#include <AS5600.h>
#include <EEPROM.h>

//PIN CONFIGURATION
ezButton leftSwitchObject(9);
ezButton centerSwitchObject(8);
ezButton rightSwitchObject(7);
#define red 10
#define green 5
#define blue 6
AS5600 as5600;   //  use default Wire


void setup() {
  //PINMODE INITIALIZATION
  pinMode (7,INPUT);
  pinMode (8,INPUT);
  pinMode (9,INPUT);
  pinMode (red,OUTPUT);
  pinMode (green,OUTPUT);
  pinMode (blue,OUTPUT);
   
  leftSwitchObject.setDebounceTime(switchDebounce); //set debounce time 
  centerSwitchObject.setDebounceTime(switchDebounce); 
  rightSwitchObject.setDebounceTime(switchDebounce); 
  
  Consumer.begin(); //Initializes the HID Library
  as5600.begin(4);  //set Encoder direction pin.
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
      Consumer.write(scrollCW);
      encoderOld=encoderNew;
    }
    else if (abs(encoderNew-encoderOld) > volumeSensitivity){
      Consumer.write(scrollCCW);
      encoderOld=encoderNew;
    }
  }

  //resets rotation before overflow
  if (as5600.getRevolutions() >= 10){
    as5600.resetPosition();
  }
  
//This part handles all the button presses
  else {
    static long timePressed;
    static long timeReleased;
    leftSwitchObject.loop();
    centerSwitchObject.loop();
    rightSwitchObject.loop();

    //checks if the center button was held and if it was been triggers the changeState() (see the bottom)
    if (timeReleased - timePressed >= holdTime){
        Consumer.write(centerSwitch);
        changeState();
        timeReleased = 0;
        timePressed = 0;
    }
        
    else if(leftSwitchObject.isPressed())
      Consumer.write(leftSwitch);
  
    else if(centerSwitchObject.isPressed()){
      Consumer.write(centerSwitch);
      timePressed = millis();
    }
    else if(centerSwitchObject.isReleased())
      timeReleased = millis();
  
    else if(rightSwitchObject.isPressed())
      Consumer.write(rightSwitch);    
  }
}

//this runs everytime the center buttin is held to change modes
void changeState(){
  static int state = EEPROM.read(0);
  static bool startup = true;
  if(startup != true)
    state++;
  if(state == 1){
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
  }
  else if(state == 2){
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
  }
  else{
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    state=0;
  }
  startup = false;
  EEPROM.write(0,state);
}
