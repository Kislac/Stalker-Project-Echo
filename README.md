# Stalker-RFM
This is a repository for a multiple device for a Stalker-RFM project

Photos about the project:
https://photos.app.goo.gl/sLBr1bHFhkoqBgjx7


Main goal was buid a long range localizator based on RadioFrequiency signal strength, to make a Stalker Echo detector replica, which actually can search for anomalies with a (With a reciever module) for an Airsoft LARP.

Within this project we have created 3 working device:
- Stalker Echo detector

https://www.thingiverse.com/thing:6482542

<img src="https://github.com/Kislac/Stalker-RFM/assets/34631881/71e75273-1545-4a7f-be1f-2332e5d9e567" width="400" title="Stalker Echo detector">

- Plant anomaly
<img src="https://github.com/Kislac/Stalker-RFM/assets/34631881/2ca0c38a-552d-4461-bd28-e93f631ab484" width="400" title="Plant anomaly">

- Bomb anomaly

https://www.thingiverse.com/thing:6482546
  
<img src="https://github.com/Kislac/Stalker-RFM/assets/34631881/8be3693c-ebb9-46e1-a7db-bec9fbfefb88" width="400" title="Bomb anomaly">


The echo detector can detect signals streght from the Plant & bomb over 100m (originally expectation was 1km in the forest, but we are not able the achive this distance)
Echo detector can show the signal strength on the olded display and/or can show the strentgh based on flashes

Stalker Project Echo - Bomb Demo [Video]

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/uerOrTpMtGg/0.jpg)](https://www.youtube.com/watch?v=uerOrTpMtGg)

Stalker Project Echo - Detector Demo [Video]

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/R6MvO_zTWoM/0.jpg)](https://www.youtube.com/watch?v=R6MvO_zTWoM)


Main wiring:

<img src="https://github.com/Kislac/Stalker-RFM/assets/34631881/285cc75f-890e-4294-b87b-0da57f0ed5c7" width="400" title="Main Wiring">

Pinout:

<img src="https://github.com/Kislac/Stalker-RFM/assets/34631881/63aa3e86-3e76-48ad-b24a-a30579526556" width="400" title="ESP pinout">

Main HW's
- RFM module: RFM69HW
MCU: 
- Echo and bomb: Nodemcu Esp32
- Plant: Bluepill (CKS32F103-MS)

BOM:
https://docs.google.com/spreadsheets/d/1qDEaFcn8x45I9WfJCfd5FSGOwXvvdaZVDlj4wth8O7k/edit?usp=sharing

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
- Stalker-RFM\VisualStudioCodes\StalkerEchoDetector
- VisualStudioCodes\StalkerFAB-100_Bomb
- VisualStudioCodes\STM32_AnomalyModule

The rest of them is just experimatal code for different micros or usefull code, which i wanted to save:
- Brain-RFM69HW-Reciever-WIP
- Brain-RFM69HW-Sender-WIP
- RFM_LowPowerLab_Bluepill_Sender
- RFM_LowPowerLab_ESP32_Reciever
- RFM_LowPowerLab_ESP32_Sender
- WaterLevelDetector







## How its worked
Stalker Project Echo - Infiltrated

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/-nhfl9IZyeA/0.jpg)](https://www.youtube.com/watch?v=-nhfl9IZyeA)


Stalker Project Echo - The rescure team

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/WN6LKFeHjqI/0.jpg)](https://www.youtube.com/watch?v=WN6LKFeHjqI)


