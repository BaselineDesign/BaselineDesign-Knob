# Updating your Arduino KNOB V2 to QMk firmware

![KNOB](https://github.com/user-attachments/assets/2c5d7582-cdef-45be-adfb-913d4c559ec1)

# What you'll need: 

1. A paper clip or a piece of wire
2. [QMK Toolbox installed](https://qmk.fm/toolbox)
3. [This hex file downloaded](https://drive.google.com/uc?export=download&id=1pcAgK7doNLUKIhplbfqNpUiG-BOIIWfD)

# Flashing Steps
1. Open QMK Toolbox and load in the downloaded hex file. Make sure to check the "Auto-Flash" checkbox

![V2firmwareload](https://github.com/user-attachments/assets/ce5f958a-abb7-44cb-93a7-7890bb07d8b4)

2. Remove your KNOB dial by rocking the dial back and forth while pulling it away from the body
3. Take note of the "RST" & "GND" pins, we'll be shorting these with the paper clip to flash the new firmware

![RST & GND](https://github.com/user-attachments/assets/66663ef8-d0a4-4143-801a-b6280e025bb4)

4. Now with QMK toolbox open, the hex file you downloaded selected under local file, and autoflash selected go ahead and plug in your KNOB (Your volume will go crazy because it cant detect the magnet, this is normal)
5. Now short those two pins and the two red LEDs on the PCB should turn off for a millisecond, count to 5, and short the pins again (this may take a couple trys)

![P1200297](https://github.com/user-attachments/assets/3ee5f7b8-bee4-4fb8-bfd3-25801bf0696e)

If your screen looks like the following then congrats! If it doesnt look like this then retry step 5, it will likely take a few tries

![Capture](https://github.com/user-attachments/assets/bab7ba52-ff29-45c0-82c0-c7fc985d4362)

6. Now open VIA and click authorize device to confirm you where sucessfull

![image](https://github.com/user-attachments/assets/16f7ff4d-9b8b-482a-af04-2b18348c9166)

7. Congrats you can now use VIA to program your KNOB!
   
Follow [these](https://github.com/BaselineDesign/BaselineDesign-Knob/tree/main/KNOB_V2_QMK/knobv2#using-knob-v2-with-via) instructions to program your KNOB with VIA

