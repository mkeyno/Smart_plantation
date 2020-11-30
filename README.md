# Smart Plantation

This is a hardware schematic for my Integration Smart Farm. This hardware can monitor 3 phase AC motor and other peripheral for farms or plantations which need remote control by the GPRS/3G network. the plant owner can read the status of its farms such as the humidity of the soil, status of well water level, power consumption, irrigation schedule, manifold valve, and gates status ...

I replace the original wireless module with ESP(my custom ESP breakout), hence I could have a central station and can run my webserver and the user can connect it to get status report and command device, also one SIM module added and ESP can communicate with AT command. For measuring the V-I of 3 Phases one AVR microcontroller added as slave I2C to ESP and on request can send the measurements  to ESP for remote or local users or even store data 


following is the pin map for EPS module and AVR board

A0,A1,A2,A3,A6,Ay on arduino use for  measure I-V

one NRF24l socked installed for remote low power soil humidity measurements 

2 digital sensor socket break out for temperature and whether station

1 digital and 1 analogue socked break out from ESP module 

4 relay installed (3 control by AVR  , one by ESP)
