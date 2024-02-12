# Stalker-RFM
This is a repository for a multiple device for a Stalker-RFM project

Photos about the project:
https://photos.app.goo.gl/sLBr1bHFhkoqBgjx7


Main goal was buid a long range localizator based on RadioFrequiency signal streght, to make a Stalker Echo detector replica, which actually can search for anomalies with a (With a reciever module).
Within this project we have created 3 working device:
- Stalker Echo detector
![IMG_20230712_110607](https://github.com/Kislac/Stalker-RFM/assets/34631881/71e75273-1545-4a7f-be1f-2332e5d9e567)
- Plant anomaly
![2024-02-12_22h31_29](https://github.com/Kislac/Stalker-RFM/assets/34631881/2ca0c38a-552d-4461-bd28-e93f631ab484)
- Bomb anomaly
![2024-02-12_22h32_14](https://github.com/Kislac/Stalker-RFM/assets/34631881/8be3693c-ebb9-46e1-a7db-bec9fbfefb88)



The echo detector can detect signals streght from the Plant & bomb over 100m (originally expectation was 1km in the forest, but we are not able the achive this distance)
Echo detector can show the signal strength on the olded display and/or can show the strentgh based on flashes
!!!!Gif a Flashelésről!!!!!


![StalkerSchematic-Page-1 drawio](https://github.com/Kislac/Stalker-RFM/assets/34631881/68502fcf-3064-4357-a525-93c93040df65)
![StalkerSchematic-Page-2 drawio](https://github.com/Kislac/Stalker-RFM/assets/34631881/9e5e1df6-b4ff-4776-9840-dee5af2c1ff2)



RFM module: RFM69HW-433S2 - https://www.hestore.hu/prod_10040954.html
MCU: 
Echo and bomb: Nodemcu Esp32
Plant: Bluepill (CKS32F103-MS)

Echo detector features:
- Can detect and show 433mh signals strenght from the plant, and bomb.
- Battery powered, and can charge via USB-C
- OTA flash
- Feedback for singal recivering: Buzzer, LED, Oled
- Buttons to configure RFM parameters
- Battery Display


Bomb features:
- Normally sending 433mh signals
- 4x18650 battery and Battery display
- Radio active noise making with a buzzer
- ARGB leds for flashing, and fading
- Acceleromter
- Accelerometer can activate the bomb --> flashing and counter can start (cannot be explode)
- Once's activated it can deactivate with a deactivation process based on multiple deactiavtion sheet.


Plant Feature:
- Normally sending 433mh signals
- 18650 battery and Battery display
- Led


Main Folders:
Stalker-RFM\VisualStudioCodes\StalkerEchoDetector
VisualStudioCodes\StalkerFAB-100_Bomb
VisualStudioCodes\STM32_AnomalyModule

The rest of them is just experimatal code for different micros or usefull code, which i wanted to save:
Brain-RFM69HW-Reciever-WIP
Brain-RFM69HW-Sender-WIP
RFM_LowPowerLab_Bluepill_Sender
RFM_LowPowerLab_ESP32_Reciever
RFM_LowPowerLab_ESP32_Sender
WaterLevelDetector

Youtube Video:
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/0eD7tyLPdFM/0.jpg)](https://www.youtube.com/watch?v=0eD7tyLPdFM)


Stalker Project Echo - Bomb Demo
[https://youtube.com/uerOrTpMtGg](https://youtube.com/shorts/uerOrTpMtGg?feature=share)


Stalker Project Echo - Detector Demo
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/R6MvO_zTWoM/0.jpg)](https://www.youtube.com/watch?v=R6MvO_zTWoM)
https://youtube.com/shorts/R6MvO_zTWoM?feature=share


Stalker Project Echo - Infiltrated
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/-nhfl9IZyeA/0.jpg)](https://www.youtube.com/watch?v=-nhfl9IZyeA)
https://youtu.be/-nhfl9IZyeA

Stalker Project Echo - The rescure team
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/WN6LKFeHjqI/0.jpg)](https://www.youtube.com/watch?v=WN6LKFeHjqI)
https://youtu.be/WN6LKFeHjqI

