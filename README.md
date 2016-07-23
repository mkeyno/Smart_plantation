# Smart_plantation
this is schematic  to monitor 3 phase AC motor and its peripheral for places which used such power source

Thanks to guys from **www.openenergymonitor.org** community which is be my inspiration to start on this project .

however I made and will plan to make some modification regard to original emonTx board.

as I'm big fan of ESP module I replace the original wireless module with ESP(my custom ESP breakout) , hence I could have center station and can run my webserver and user can connect it to get status report and command device , also one SIM module added and ESP can communicate with AT command. For measuring the V-I of 3 Phases one Arduino promini added as slave I2C to ESP and on request can send the measurements  to ESP for remote or local users or even store data 

this board would be center for controlling many device and sensor in plantation , like as manifold valve , gates , and like the weather station can measer the humidity, temperature ,  soil humidity , lighting 

## Image Device
  * [ESP-12F front](#esp12f .jpg)
  * [ESP-12F back](#esp12f-.jpg)
  * [DC-DC convertor](#dc-dc.jpg)
  * [Arduino promini] 
  * [SIM800L](#sim.jpg)
##
following is the pin map for EPS module and Arduino board

A0,A1,A2,A3,A6,Ay on arduino use for  measure I-V

one NRF24l socked installed for remote low power soil humidity measurements 

2 digital sensor socket break out for temperature and whether station

1 digital and 1 analogue socked break out from ESP module 

4 relay installed (3 control by Arduino , one by ESP)
