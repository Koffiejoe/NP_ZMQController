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
<p align="center"><img src="doc/overviewDiagram.png" alt=overView Diagram" height="621 width ="650></p>
  
## Protocol
This service runs on a simple string-based protocol where commands separated by a '>' sign are send over the network. Several commands can be send to the server, and controllerstatus and error codes are received. All the controllers connected to the server get assigned a certain ID. This list can be requested from the server by sending a certain request (see below).

The main channel that a client has to subscribe to is the "controllerService!>" channel. All responses from the server happen with this first command. All the request from the client have to be made to the "controllerService?>" channel. Below is a detailed overview of all commands used in this system. All values between ## have to be replaced by a certain value (do not include the #!).

### Request from client --> server

<b>controllerservice?>contr>#ID#>sUpdate>#update in ms#></b>
* Sets the updateSpeed of a controller with a certain ID in milliseconds

<b>controllerservice?>contr>#ID#>gUpdate></b>
* Gets the updateSpeed of a controller with a certain ID

<b>controllerservice?>contr>#ID#>gRumble>#1 or 0"></b>
* Sets the rumble to ON (1) or OFF (0) of a controller with a certain ID

<b>controllerservice?>contr>#ID#>sRumble></b>
* Gets the rumble of a controller with a certain ID

<b>controllerservice?>serv>gList></b>
* Gets a list of all the controllers connected to the server. (more info under respons)

### Respons from server --> client

<b>controllerservice!>contr>#ID#>gUpdate>#updatespeed in ms#></b>
* Gets the updateSpeed of a controller with a certain ID 

<b>controllerservice!>contr>#ID#>gRumble>#1 or 0"></b>
* Get the rumble ON (1) or OFF (0) of a controller with a certain ID

<b>controllerservice!>serv>gList></b>
* Gets a list of all the controllers connected to the server. Currently only 2 types are supported: NES and GC. The place in the respons string indicates which ID they have: the first one has ID 0, the second one ID1, ... <br>
The values returned are separated by comma's. e.g.: controllerService!>serv>gList>NES,GC> (NES=ID0, GC=ID1)



<b>controllerService!>err>INV_UPD_SPEED></b>
* a wrong updatespeed has been given. make sure it's between 10 and 20000 ms

<b>controllerService!>err>INV_1ST_COM></b>
* a wrong 1st command has been given (controllerService!> not included)

<b>controllerService!>err>INV_CONTR_NUM></b>
* a wrong ID has been given

<b>controllerService!>err>INV_3RD_COM></b>
* a wrong 3rd command has been given (controllerService!> not included)

