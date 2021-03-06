# Esp32bot

[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FSoyM%2FEsp32bot.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2FSoyM%2FEsp32bot?ref=badge_shield)

NodeMcu-32s & ROS.

## Feature

* Reconnect WIFI automatically when the network is disconnected
* Board LED flashing when system is running
* Synchronize time from NtpServer
* Support multi WiFi and switch to available WIFI automatically
* Websocket support 

## Usage

* Install the current upstream [Arduino](https://www.arduino.cc/en/Main/Software) IDE at the 1.8 level or later. 
* Start Arduino and open Preferences window.
* Enter https://dl.espressif.com/dl/package_esp32_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
* Open Boards Manager from Tools > Board menu and install esp32 platform (and don't forget to select your ESP32 board from Tools > Board menu after installation).
* Install dependent libraries.
* Open "Esp32bot.ino" with Arduino.

## Ros

http://wiki.ros.org/rosserial_arduino

### Install Ros dependence

    $ sudo apt-get install ros-<your-ROS-distro>-rosserial-arduino && sudo apt-get install ros-<your-ROS-distro>-rosserial 

    $ cd <sketchbook>/libraries

    $ rosrun rosserial_arduino make_libraries.py .

### Command

    $ roscore && roslaunch rosserial_server socket.launch

## Dependent libraries

* ArduinoJson at version 5.13.1
* DHT_sensor_library at version 1.3.0
* pubsubclient-2.6 at version 2.6
* HTTPClient at version 1.1
* EEPROM at version 1.0
* Adafruit_Unified_Sensor at version 1.0.2
* WiFiClientSecure at version 1.0

## Development board

http://wiki.ai-thinker.com/esp32/boards/nodemcu_32s

## Resouce

esp32_datasheet:

https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_cn.pdf

esp32_technical_reference_manual:

https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_cn.pdf

nodemcu_32s_pin:
![nodemcu_32s_pin.png](doc/nodemcu_32s_pin.png)

nodemcu_32s_sch:
![nodemcu_32s_sch.png](doc/nodemcu_32s_sch.png)

## License
GPL V3

[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FSoyM%2FEsp32bot.svg?type=large)](https://app.fossa.io/projects/git%2Bgithub.com%2FSoyM%2FEsp32bot?ref=badge_large)
