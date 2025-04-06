# KNOB
KNOB is the first QMK supported device to use a hall effect rotary encoder with precise 12-bit resolution. 
[KNOB V2.1 is currently for sale!](https://baselinedesign.tech/products/knobv21)

* Keyboard Maintainer: [BaselineDesign](https://github.com/BaselineDesign)
* Hardware Availability: [Etsy.com](https://baselinedesign.tech/products/knobv21)

![KNOB](https://github.com/user-attachments/assets/2c5d7582-cdef-45be-adfb-913d4c559ec1)

## Which KNOB do I have?
There are essentially 3 versions of KNOB out in the wild:

KNOB V2.1:
* This is the most recent and currently shipping version of KNOB
* All KNOB V2.1 Shipped with QMK and can be remapped using [VIA](https://usevia.app/)
* When you press and hold the center key nothing should happen
* When you connect to your KNOB with [VIA](https://usevia.app/) it will tell you which version you have
*  Instructions and more information can be found [here](https://github.com/BaselineDesign/BaselineDesign-Knob/tree/main/KNOB_V2_QMK/knobv2_1)
* If you recieved your KNOB before 2/14/2025 your KNOB has a bug where 10% of the time it will send the wrong key when rotating the dial. [Follow these instructions to update your KNOB V2.1 to the current version](https://github.com/BaselineDesign/BaselineDesign-Knob/blob/main/KNOB_V2_QMK/knobv2_1/KNOBV2_1_Update_Firmare_Instructions.md)

KNOB V2 (QMK):
* Shipped with QMK programming and can be remapped using [VIA](https://usevia.app/)
* When you press and hold the center key nothing should happen
* When you connect to your KNOB with [VIA](https://usevia.app/) it will tell you which version you have
* Instructions and more information can be found [here](https://github.com/BaselineDesign/BaselineDesign-Knob/tree/main/KNOB_V2_QMK/knobv2)
* If you recieved your KNOB before 2/14/2025 your KNOB has a bug where 10% of the time it will send the wrong key when rotating the dial. [Follow these instructions to update your KNOB V2 to the current version](https://github.com/BaselineDesign/BaselineDesign-Knob/blob/main/KNOB_V2_QMK/knobv2/KNOBV2_Update_Firmare_Instructions.md)

KNOB V2 (Arduino):
* Shipped with arduino programming but can be upgraded to qmk
* Arduino Code can be found [here](https://github.com/BaselineDesign/BaselineDesign-Knob/tree/main/KNOB_Arduino/BaselineDesign_KNOB_V2_Arduino)
* Press and hold the center key, if the backlighting cycles through different colors then you have an arduino KNOB V2

KNOB V1:
* Only has Arduino Code support
* Only has one color underglow
* Code can be found [here](https://github.com/BaselineDesign/BaselineDesign-Knob/tree/main/KNOB_Arduino/BaselineDesign_KNOB_V1_Arduino)
* There are very few of these out in the wild but you can tell it's a V1 by the bump on the back:
![IMG_20250214_135036](https://github.com/user-attachments/assets/1a7074e6-e142-44f4-8527-b934c4e09c1a)






