# LEDCeilingLight
A wifi-enabled ceiling light controlled by a WeMos D1 on a custom PCB with a TLC5971 driving MOSFETS to control 2 24V RGBW LED strips and a 12V WS2812 strip

![](https://i.imgur.com/ibRPM0L.jpg)
There's a few color pattern modes which can be controlled by toggling a physical wall switch or with HTTP get requests. A specific request to the wakeupLights page will schedule the light to turn on slowly at a certain time. Use with IFTTT to send an HTTP request with the contents of an Android "Upcoming Alarm" notification.

The light is powered by a 24V 5A supply for the non-addressable LEDs and a 12V 2A supply for the addressable LEDs and circuit power. A traditional light switch is monitored using a simple optocoupler circuit to pull down the "5V Switch Sense" pin when the switch is turned on.

![](https://i.imgur.com/35uN6Sx.jpg)
## Circuit Design

All circuit design files are available publically on CircuitMaker. https://workspace.circuitmaker.com/Projects/Details/Dan-Nichols/Strip-Ceiling-Lamp-Control

The 24V LEDs are driven through a rather complicated circuit. Since the ESP8266 does not do hardware PWM, bad things happen if you try to directly drive the FETs with the digital I/O when network activity happens. Therefore, an extrenal driver is required. 

To prevent audible noise from a cheap power supply coil whine, the TLC5971 was chosen since it does high-frequency enhanced-spectrum PWM. But it is an open-collector device so it can't drive NMOS directly. So its outputs simply drive a pullup resistor array. This signal is then put through a 74AC240NG inverter which provides significant drive current for the FETs, inverts the signal so that "on" on the TLC is "on" to the FETs, and provides an extra level of safety isolation in the event of TLC5971 failure. The 74AC240 then drives the FETs as strong as possible without exceeding maximum current rules. Since the high-frequency PWM has a minimum pusle width of 100ns, fast switching is essential.


## PCB Bodges
The 5V Switch Sense cirucit is changed since an optocoupler is used instead of a 5V plugpack connected to a physical light switch. Given that the optocoupler provides an open-collector output, the divider is replaced with a pullup.

Since I broke one of the channels on the TLC5971, one of the four extra channels (it's a 12-channel driver and only 8 channels are used) was connected replace the broken channel. Also a diode is installed so that the 5V rail can backfeed the 12V rail to not destroy the 7805 or WS2812s.

To solder the TLC5971 thermal pad through the vias underneath, solder mask was removed from the backside of the board under the component.

## Other Notes
The TLC5971 was soldered using [Dave's technique](https://www.youtube.com/watch?v=588iV07nEdM).

The file noGit/auth.hpp simply defines the device's email address and password for sending notifications and the email destination and is not included for obvious reasons.

This product includes software developed by the Adafruit Industries. Their TLC59711 driver library was used to control the TLC5971 with modification to adjust the rate of the SPI communication. The rest of the material on this site and the schematics are licensed under the GNU LGPLv3 license.
