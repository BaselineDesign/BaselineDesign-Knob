 #include <ClickEncoder.h>
#include <TimerOne.h>
#include <HID-Project.h>
#include <ezButton.h>

//ENCODER PIN DEFINITION
#define ENCODER_CLK 2 //Sets the two pins that monitor the rotary encoder position
#define ENCODER_DT 3

//More information about HID-Project and the availible actions can be found here https://github.com/NicoHood/HID/wiki/Consumer-API

//USER ACTIONS
#define cwAction MEDIA_VOLUME_DOWN               //Change these definitions to to change what each user input does
#define ccwAction MEDIA_VOLUME_UP
#define leftSwitchAction MEDIA_PREVIOUS
#define centerSwitchAction MEDIA_PLAY_PAUSE
#define rightSwitchAction MEDIA_NEXT

//SWITCH PIN DEFINITION
ezButton leftSwitch(7);      //Left switch
ezButton centerSwitch(8);    //Center switch
ezButton rightSwitch(9);     //Right swithc

ClickEncoder *encoder; // variable representing the rotary encoder
int16_t last, value;   // variables for current and last rotation value

void timerIsr() {
  encoder->service();
}

void setup() {
  // Serial.begin(9600); // Opens the serial connection used for communication with the PC for debugging
  
  //SWITCH DEBOUNCE TIME
  leftSwitch.setDebounceTime(20);   // set debounce time to 20 milliseconds
  centerSwitch.setDebounceTime(20); 
  rightSwitch.setDebounceTime(20); 
  
  //ENCODER SETUP
  Consumer.begin(); // Initializes the media keyboard
  encoder = new ClickEncoder(ENCODER_DT, ENCODER_CLK); // Initializes the rotary encoder with the mentioned pins
  Timer1.initialize(1000); // Initializes the timer, which the rotary encoder uses to detect rotation
  Timer1.attachInterrupt(timerIsr);
  last = -1;
}

void loop() {
  
  value += encoder->getValue();

  //This part of the code is responsible for the actions when you rotate the encoder
  if (value != last) { //New value is different than the last one, that means to encoder was rotated
    if(last>value){ //Detecting the direction of rotation
      Consumer.write(ccwAction); // Replace this line to have a different function when rotating counter-clockwise
    }
    else{
      Consumer.write(cwAction); // Replace this line to have a different function when rotating clockwise
    }
    last = value; // Refreshing the "last" varible for the next loop with the current value
  }
 
  else { //If nothing has changed with the encoder then check if anything is happening with the buttons
    leftSwitch.loop();    // MUST call the loop() function first
    centerSwitch.loop();  // MUST call the loop() function first
    rightSwitch.loop();   // MUST call the loop() function first
    
    if(leftSwitch.isReleased())
      Consumer.write(leftSwitchAction);
  
    else if(centerSwitch.isReleased())
      Consumer.write(centerSwitchAction);
  
    else if(rightSwitch.isReleased())
      Consumer.write(rightSwitchAction);    
  }
}


/*
    This is just a long comment
    Here are some fun functions you can use to replace the default behaviour 
    Consumer.write(CONSUMER_BRIGHTNESS_UP);
    Consumer.write(CONSUMER_BRIGHTNESS_DOWN);
    Consumer.write(CONSUMER_BROWSER_HOME);
    Consumer.write(CONSUMER_SCREENSAVER);
    Consumer.write(HID_CONSUMER_AL_CALCULATOR); //launch calculator :)
    Consumer.write(HID_CONSUMER_AC_ZOOM_IN);
    Consumer.write(HID_CONSUMER_AC_SCROLL_UP);
    CONSUMER_SLEEP = 0x32,

    FULL LIST CAN BE FOUND HERE:
    https://github.com/NicoHood/HID/blob/master/src/HID-APIs/ConsumerAPI.h
*/
        
