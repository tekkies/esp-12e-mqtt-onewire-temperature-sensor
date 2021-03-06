[![Stability: Active](https://masterminds.github.io/stability/active.svg)](https://masterminds.github.io/stability/active.html)
# esp-12e-mqtt-onewire-temperature-sensor
Low power ESP8266 MQTT OneWire Temperature Sensor using DS18B20

## ToDo
- [ ] Board: Increase gap between power and 1-wire headers
- [ ] Export state machine
- [_] Parallelize DS18x20 coversion when multiple devices
- [_] ADC Calibration
- [_] Map sensor voltage vs temperature
     - DS18x2x are not rated at 2.5v (2xAA), but still give readings showinga few degrees higher.  Try to characterise this.


![Installed](doc/photo-installed.jpg)

![MQTT Explorer](doc/mqtt-explorer-screenshot.png)


## Circuit 
![Circuit](board/ESP-12E-OneWire-TinyCAD.png)

## Board Layout
![Boad Layout](board/ESP-12E-OneWire-VeeCAD.png)

![Bare Board](doc/photo-bare-board.jpg)

## Built by Standing on the Shoulders of These Giants
 - [OneWire](https://github.com/PaulStoffregen/OneWire) by [Paul Stoffregen](https://github.com/PaulStoffregen)
 - [arduino-mqtt](https://github.com/256dpi/arduino-mqtt)  by [Joël Gähwiler](https://github.com/256dpi)