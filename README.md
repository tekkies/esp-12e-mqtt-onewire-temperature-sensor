[![Stability: Experimental](https://masterminds.github.io/stability/experimental.svg)](https://masterminds.github.io/stability/experimental.html)
# esp-12e-mqtt-onewire-temperature-sensor
Low power ESP8266 MQTT OneWire Temperature Sensor using DS18B20

## ToDo
- [ ] Write the code
- [ ] Connect WAKE to RESET for deep sleep wake
- [ ] Increase gap between power and 1-wire headers
- [ ] Trystoring config in flash - https://dalegi.com/2020/04/22/espflash-an-arduino-library-for-storing-data-in-the-esp-filesystem/
- [ ] Export state machine

![Installed](doc/photo-installed.jpg)

## Circuit 
![Circuit](board/ESP-12E-OneWire-TinyCAD.png)

## Board Layout
![Boad Layout](board/ESP-12E-OneWire-VeeCAD.png)

![Bare Board](doc/photo-bare-board.jpg)

## Built by Standing on the Shoulders of These Giants
 - [OneWire](https://github.com/PaulStoffregen/OneWire) by [Paul Stoffregen](https://github.com/PaulStoffregen)
 - [arduino-mqtt](https://github.com/256dpi/arduino-mqtt)  by [Joël Gähwiler](https://github.com/256dpi)