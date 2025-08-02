# KNOB V2.1

![KNOB](https://github.com/user-attachments/assets/2c5d7582-cdef-45be-adfb-913d4c559ec1)




KNOB is the first QMK supported device to use a hall effect rotary encoder with precise 12-bit resolution. 
[KNOB V2.1 is currently for sale!](https://www.etsy.com/listing/1748096655/knob-v2-volume-media-controller)

* Keyboard Maintainer: [BaselineDesign](https://github.com/BaselineDesign)
* Hardware Supported: KNOB V2.1
* Hardware Availability: [BaselineDesign.tech](https://baselinedesign.tech/products/knobv21)

# Customizing your KNOB (Using the VIA web configurator) 
KNOB comes pre mapped to work perfectly as a volume dial with volume control on the dial and media control on the keys, but with VIA you can remap your KNOB to do just about anything! VIA is an application that runs in your browser and lets you customize what the buttons and dial on your KNOB do without installing any software or writing any code. With VIA you'll also be able to change the color of the LEDs, change sensitivity, and enable smooth scrolling. To customize your KNOB with VIA you'll have to follow a couple steps and upload a file, the instructions are as follows: 

[Getting started youtube video](https://youtu.be/Hd0r6GVkbzM)
## Setting up VIA:
1. First [Download this VIA .json file](https://drive.google.com/uc?export=download&id=1Bo-KuTojrcoyr9Ovg1zOziSa1WW7xraH) we'll use it in a later step
3. Plug KNOB into your computer and right click [here](https://usevia.app/settings) to open VIA in a new tab. (Online VIA can only be opened by browsers based on Chromium currently.)
4. In VIA go to the "Settings" tab (gear icon) and turn on the "Show Design tab". (If it's already on, skip this step.)
5. Go to the "Design" tab (paintbrush icon) and select the "Load" button and select the .json file you downloaded in step 1 from your downloads folder.
7. Go to the "Configure" tab (keyboard icon) and click "Authorize Device", select your KNOB from the pop-up and click connect.<br>

Your KNOB is now ready to be Configured!

## Remapping
*Due to a quirk with VIA remapping keys is handled differently from remapping the dial*

### Remapping Keys
Select the key you want to remap on the KNOB graphic, the selected key will then flash slowly. To remap it you can either:
- Select the desired key option from the key library at the bottom of the page (More key options can be found in the BASIC/MEDIA/MACRO/LAYERS/SPECIAL section tabs of the key library)<br>

or
  
- If you know the keycode you can select "any" from the "Special" tab of the library and type in the keycode. A list of keycodes can be found [here](https://baselinedesign.tech/pages/via-keycode-cheatsheet)



### Remapping Dial
*(If you'd like to enable smooth scrolling skip ahead to "Smooth Scrolling")* <br> 
To remap the dial click on the dial in the KNOB graphic and enter your desired keycodes for Rotate Counterclockwise and Rotate Clockwise. Keycodes can be found [here](https://baselinedesign.tech/pages/via-keycode-cheatsheet) or by hovering over the key you'd like in the VIA key library and copying the keycode that apears at the bottom left of the page <br>

To adjust sensitivity click on the "KNOB CUSTOMIZATION" tab to the left (icon beneath the light bulb) and adjust the sensitivity slider. <br>
***WARNING** setting the sensitivity too high can overwhelm your computer with key commands and cause odd behavior*


### Smooth Scrolling
To enable Smooth Scrolling click on the "KNOB CUSTOMIZATION" tab to the left (icon beneath the light bulb). From here toggle "Enable Smooth Scroll" on and adjust the various settings as desired:
- Scroll Direction: Selects vertical or horizontal scrolling
- Smooth Scroll Sensitivity: Changes scroll speed
- Dead Zone: Changes how far you have to turn the dial before it starts to scroll
- Reverse Scroll Direction: Changes the direction of scroll

<br> * If Smooth scroll doesn't work when toggled on in VIA, your KNOB is running older firmware and needs to be updated using [these instructions](https://github.com/BaselineDesign/BaselineDesign-Knob/blob/main/KNOB_V2_QMK/knobv2_1/KNOBV2_1_Update_Firmare_Instructions.md)*


### Customizing LEDs
Click the "LIGHTING" tab on the left (lightbulb icon) and customize as desired
 <br>
 <br>
  <br>
 <br>
  <br>
 <br>



## KNOB Compiling Instructions
Make example for this keyboard:

    make knobv2_1:default

Flashing example for this keyboard:

    make knobv2_1:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down the front left key and plug in KNOB
* **Jumper reset pin**: Remove the dial from KNOB and short the rst pin to gnd (should be right next to eachother on the top left)


