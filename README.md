# CZMQ:Controller-Server
The purpose of this project is to create a server which can send the state of a game controller over a ZMQ network. It automatically handles the update interval and communication over serial connection.

Although you can add a wide variety of input, 2 types of controllers have already been implemented: a NES and an original GC controller. The code to program the microcontrollers which connect to the NES and GC controllers has been included in the microcontroller_code folder.

## Requirements

  ### Software
  * Visual studio 2019
  * Arduino IDE 1.8.10
  * PSoC creator 4.3
  
  ### Libraries
  * Serial library(.lib in proj)  https://github.com/wjwwood/serial
  * CZMQ                          https://joshuaburkholder.com/wordpress/2018/05/25/build-and-static-link-zeromq-on-windows/
  
  ### Hardware
  * PSoc 4 Pioneer kit (CY8CKIT-042)
  * Arduino Uno (or compatible)
  * NES controller (with 7 pin connector)
  * GC controller (original)
  

## Scheme
