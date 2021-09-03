[![Build Status](https://travis-ci.com/mkeyno/KeynoRobot.svg?branch=master)](https://travis-ci.com/mkeyno/KeynoRobot)
[![C++](https://img.shields.io/badge/c++-%2300599C.svg)](https://www.python.org/downloads/)
![GitHub](https://img.shields.io/github/license/mkeyno/KeynoRobot.svg) 

# Smart Plantation

My country has limited water resources while most of the farmssteel using the old methods for farming and irrigation. Most of the plantation using 3phase electrical motor for pulling water from deep well, This motor put a heavy load on the electrical grid and the government triedto encourage the farmers to not use the electricity in high peak hours on thegrid by subsidizing those farms which use the electricity in  not rush hours This Repository consists of the hardware and firmware for controllingthe farm water & electricity usage base on IoT technology. The different scenarios can be  set by owners of the plantation regards to the information that comes from the water pumpirrigation systems and lands. 
Water irrigation system consist electrical pump for the well & storagepool, all electrical actuator valve, sensors for soil humidity  and water level in wellSoil humidity detects by a low-power solar humidity sensor equipped with an nRF24lP low power transmitter.Every 3 hours sample of humidity sends to the main unit. Different scenarios base onwater level, time, total electricity consumption will be trigged and report to the landowner via SMS


# Hardware

This controller consists of 4 main modules, 
  - ESP8266 for IoTinterface and local webserver for setting up the plantation control and select a  variety of scenario  
  
  - SIM700 GSM/GPRSmodule for remote control of plantation with a variety of predefined SMS commandwhich is authenticated by mobile number 
  
  - nRF24Lp lowpower transmitter link for gathering data of humidity sensors across theplantation
  
  - STM32F103C8module for handling all sub-control routines such as all valve actuators andmotors and get all sensor information from soil humidity sensor and ultrasonicwater level sensor. 
  
This module activates the electrical motors regards thesensor data and scenario which has been defined by the landowner. To monitor power consumption and power factor of electricalmotors, three ST sensors for measuring the current of each phase, and one transformer formeasuring the voltage of lines has been considered
following are the image of the designed PCB 

<img src="/resource/back.png" width="400" height="350">,,
<img src="/resource/front.png" width="400" height="350">

# Firmware

Two MCUs were used in this design, one ESP8266 as pilot controland data link  for local webserver and one STM32F103C8 forcontrolling the plantation, these MCUs communicate together via I2C protocol inmaster-slave modeESP8266 can restart the STM32F103C8 and adjust the setting byIOT webserver portal, the following image showing the different pages which designedfor different parts of the controller  


![Image](https://github.com/mkeyno/Smart_plantation/blob/master/resource/home.png) 

![Image](https://github.com/mkeyno/Smart_plantation/blob/master/resource/farm.png)

![Image](https://github.com/mkeyno/Smart_plantation/blob/master/resource/GSM.png)

![Image](https://github.com/mkeyno/Smart_plantation/blob/master/resource/wifi.png)

