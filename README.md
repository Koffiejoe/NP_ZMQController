# Network Programming Pong game 
Made by students of [the hogeschool PXL](https://www.pxl.be).

The purpose of this project is to develop a simple C/C++ application through a server.

for this Project the [ZMQ Library](https://zeromq.org/languages/cplusplus/) is used.

on the client you control the rackets for the pong game. data gets send through the broker to the game where the rackets respond to the clients commands.

Data on the broker:

<p align="center"><img src="broker.png"></p>
## Troubleshooting
during this project there are issues with filtering the buffer correctly.

<p align="center"><img src="buffer.png"></p>

