# KNOB

![KNOB](https://github.com/user-attachments/assets/2c5d7582-cdef-45be-adfb-913d4c559ec1)

## Which KNOB Do I Have?
There are essentially 3 versions of KNOB out in the wild:
KNOB V1:
* Only has Arduino Code support
* Only has one color underglow
* Code can be found [here](https://github.com/BaselineDesign/BaselineDesign-Knob/tree/main/KNOB_Arduino/BaselineDesign_KNOB_V1_Arduino)
* There are very few of these out in the wild but you can tell it's a V1 by the bump on the back:
![IMG_20250214_135036](https://github.com/user-attachments/assets/1a7074e6-e142-44f4-8527-b934c4e09c1a)

KNOB V2 (Arduino):
* Shipped with arduino programming but can be upgraded to qmk
* Arduino Code can be found [here](https://github.com/BaselineDesign/BaselineDesign-Knob/tree/main/KNOB_Arduino/BaselineDesign_KNOB_V2_Arduino)
* Press and hold the center key, if the backlighting cycles through different colors then you have an arduino KNOB V2

KNOB V2 (QMK):
*Shipped with QMK programming and can be remapped using [VIA](https://usevia.app/)
*When you press and hold the center key nothing happens
*When you connect to your KNOB with [VIA](https://usevia.app/) it will tell you which version you have
*If you recieved your KNOB before 2/14/2025 your KNOB has a bug where 10% of the time it will send the wrong key when rotating the dial. Follow these instructions to update your KNOB V2 to the current version

KNOB V2.1:
*All KNOB V2.1 Shipped with QMK
*When you connect to your KNOB with [VIA](https://usevia.app/) it will tell you which version you have


KNOB is the first QMK supported device to use a hall effect rotary encoder with precise 12-bit resolution. 
[KNOB V2.1 is currently for sale!](https://www.etsy.com/listing/1748096655/knob-v2-volume-media-controller)

* Keyboard Maintainer: [BaselineDesign](https://github.com/BaselineDesign)
* Hardware Supported: KNOB V2.1
* Hardware Availability: [Etsy.com](https://www.etsy.com/listing/1748096655/knob-v2-volume-media-controller)

Make example for this keyboard:

    make knobv2_1:default

Flashing example for this keyboard:

    make knobv2_1:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the front left key and plug in KNOB
* **Jumper reset pin**: Remove the dial from KNOB and short the rst pin to gnd (should be right next to eachother on the top left)



## Using KNOB V2.1 with VIA
1. First [Download this VIA .json file](https://drive.google.com/uc?export=download&id=1IPoGg8PWFGqJwyo9-glygwBleMjgLrtM) 
2. Connect your KNOB and click [here](https://usevia.app/settings) to open VIA. (Online VIA can only be opened by browsers based on Chromium currently.)
3. In VIA turn on the "Show Design tab" in the "Settings" tab. (If it's already on, skip this step.)
4. Drag the JSON file you downloaded in step 1 into the “Design” tab in via.
5. On the Configure tab, select the KEYMAP section, and click on the key that you want to program, and the key will then flash slowly. Click on a key you want to remap in the BASIC/MEDIA/MACRO/LAYERS/SPECIAL/QMK LIGHTING/CUSTOM section. And it's done.

