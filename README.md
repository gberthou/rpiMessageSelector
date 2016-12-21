# rpiMessageSelector
Simple Raspberry Pi application driven by a rotary switch that displays a message based on the selected input.
It was designed for Raspberry Pi mod B+ (first generation).

## System
This program is to be compiled and run on a platform that supports `fbi`, the framebuffer image viewer.
I used Raspbian to keep things simple.
Since it is based on framebuffer, no graphical environment is needed.

## Hardware
- Raspberry Pi
- Rotary switch, the one I used is a 12 positions rotary switch
- LED and an adequate resistor to indicate whether a message is being displayed are not
- Button used to toggle display

## Pinout
    
         +-+                   +-+                  +-+
    GND  | |  GND        RS04  | |  GPIO 8     GND  | |  GND
         +-+                   +-+                  +-+
    LED  | |  GPIO 14    RS05  | |  GPIO 7    RS03  | |  GPIO 11
         +-+                   +-+                  +-+
    BTN  | |  GPIO 15    RS06  | |  GPIO 12   RS02  | |  GPIO 5
         +-+                   +-+                  +-+
                         RS07  | |  GPIO 16   RS01  | |  GPIO 6
                               +-+                  +-+
                         RS08  | |  GPIO 20   RS12  | |  GPIO 13
                               +-+                  +-+
                         RS09  | |  GPIO 21   RS11  | |  GPIO 19
                               +-+                  +-+
                                              RS10  | |  GPIO 26
                                                    +-+
    
    BTN: Button
    RS: Rotary switch
