# KNOB V2

![KNOB](https://github.com/user-attachments/assets/2c5d7582-cdef-45be-adfb-913d4c559ec1)




KNOB V2 is the first QMK supported device to use a hall effect rotary encoder with precise 12-bit resolution. 
[KNOB V2 is currently for sale!](https://baselinedesign.tech/products/knobv21)

* Keyboard Maintainer: [BaselineDesign](https://github.com/BaselineDesign)
* Hardware Supported: KNOB V2
* Hardware Availability: [baselinedesign.tech](https://baselinedesign.tech/products/knobv21)
* [3D Printing & CAD models](https://www.printables.com/model/927884-knob-v2-opensource-media-controller)

Make example for this keyboard:

    make knobv2:default

Flashing example for this keyboard:

    make knobv2:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the front left key and plug in KNOB
* **Jumper reset pin**: Remove the dial from KNOB and short the rst pin to gnd (should be right next to eachother on the top left)



## Using KNOB V2 with VIA

1. First [Download this VIA .json file](https://drive.google.com/uc?export=download&id=1-IM_kSFA1yx1T-PHw8p78PXtSrtHclPA) 
2. Connect your KNOB and click [here](https://usevia.app/settings) to open VIA. (Online VIA can only be opened by browsers based on Chromium currently.)
3. In VIA turn on the "Show Design tab" in the "Settings" tab. (If it's already on, skip this step.)
4. Drag the JSON file you downloaded in step 1 into the “Design” tab in via.
5. On the Configure tab, select the KEYMAP section, and click on the key that you want to program, and the key will then flash slowly. Click on a key you want to remap in the BASIC/MEDIA/MACRO/LAYERS/SPECIAL/QMK LIGHTING/CUSTOM section. And it's done.
