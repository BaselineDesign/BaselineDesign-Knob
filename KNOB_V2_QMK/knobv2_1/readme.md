# KNOB V2.1

![KNOB](https://github.com/user-attachments/assets/2c5d7582-cdef-45be-adfb-913d4c559ec1)




KNOB is the first QMK supported device to use a hall effect rotary encoder with precise 12-bit resolution. 
[KNOB V2.1 is currently for sale!](https://www.etsy.com/listing/1748096655/knob-v2-volume-media-controller)

* Keyboard Maintainer: [BaselineDesign](https://github.com/BaselineDesign)
* Hardware Supported: KNOB V2.1
* Hardware Availability: [BaselineDesign.tech](https://baselinedesign.tech/products/knobv21)

## Using KNOB V2.1 with VIA
1. First [Download this VIA .json file](https://drive.google.com/uc?export=download&id=1Bo-KuTojrcoyr9Ovg1zOziSa1WW7xraH)
3. Connect your KNOB and click [here](https://usevia.app/settings) to open VIA. (Online VIA can only be opened by browsers based on Chromium currently.)
4. In VIA turn on the "Show Design tab" in the "Settings" tab. (If it's already on, skip this step.)
5. Drag the JSON file you downloaded in step 1 into the “Design” tab in via.
6. On the Configure tab, select the KEYMAP section, and click on the key that you want to program, the selected key in VIA will then flash slowly. Click on a key you want to remap in the BASIC/MEDIA/MACRO/LAYERS/SPECIAL/QMK LIGHTING/CUSTOM section, that key has now been remapped
7. To program the dial (not smooth scroll) click on the dial and enter your desired keycodes for Rotate Counterclockwise and Rotate Clockwise. Keycodes can be found [here](https://baselinedesign.tech/pages/via-keycode-cheatsheet) or by hovering over the key you'd like in VIA and copying the keycode that apears at the bottom left of the page

<br> *If Smooth scroll doesnt work when toggled on in VIA you're going to have to update your firmware using [these instructions](https://github.com/BaselineDesign/BaselineDesign-Knob/blob/main/KNOB_V2_QMK/knobv2_1/KNOBV2_1_Update_Firmare_Instructions.md) or use [this older VIA .json file](https://drive.google.com/uc?export=download&id=1IPoGg8PWFGqJwyo9-glygwBleMjgLrtM)*


## KNOB Compiling Instructions
Make example for this keyboard:

    make knobv2_1:default

Flashing example for this keyboard:

    make knobv2_1:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the front left key and plug in KNOB
* **Jumper reset pin**: Remove the dial from KNOB and short the rst pin to gnd (should be right next to eachother on the top left)


