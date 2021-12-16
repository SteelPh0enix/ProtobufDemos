# Protobuf API Example - PC <-> STM32 communication

This repository contains an example on how to communicate with STM32 using [Protocol Buffers](https://developers.google.com/protocol-buffers).

In [Python](./Python) directory, there's an example Python project that communicates with STM32 microcontroller via Protobuf. This project uses virtual enviroment managed by [pipenv](https://pipenv.pypa.io/en/latest/), although it's not required to use it in order to run it. See the README in this directory for details.

In [STM32](./STM32_Nucleo-F401) directory, there's an example STM32 project for Nucleo-F401 board that implements Protobuf API. If you'd like to run it on different board, see migration description which you can find in this directory README.

Used protocol file can be found in [proto](./proto) directory. Generated files are already included into Python and STM32 projects, however if you'd like to re-compile them, everything is described in this directory README.

## Implementation details

Protobuf is a relatively simple and robust binary data format. It's great for many purposes, but it shines best when it's used to communicate between systems written in different languages - since the actual, native code is generated from `proto` files, and it's constructed to preserve backward-compatibility, it's very versatile as it takes some of the responsibility of worrying about changing data format from the programmer.

However, since Protobuf does not support message termination by itself, streaming the data can be problematic. Here, i solve it by encapsulating Protobuf data into my own "packet", consisting of 2 bytes of packet length (excluding itself) and the actual data bytes, like that:

| Size header | Protobuf-encoded message |
|-------------|--------------------------|
| 2 bytes     | N bytes                  |

This is very simple and very naive implementation, but it suits this example. It's also easely expandable - if you'd want to know which kind of protobuf message is transmitted before decapsulating it into an object, you can add one or two additional header bytes. In that case it's not necessary, because of how Protobuf messages are defined.

## Implementation on STM32

On STM32 side, we use [nanopb](https://jpa.kapsi.fi/nanopb/) library, as it's probably the lightest implementation available with decent support. STM32 communicates via UART, since it's easiest way to transmit data between the board and PC. Internally, it uses DMA with idle line detection for RX, so in theory the size header is not really required on STM32 side, but we'll keep using it, as it provides an easy way to verify if the data we receieved or transmitted has at least correct size.

## Implementation on PC

On PC, we use Python with [pyserial](https://pypi.org/project/pyserial/) for communication, and official [protobuf](https://pypi.org/project/protobuf/) libraries. It's a very simple example application that you can just run and see the results. More details can be found in the project README, in [Python](./Python) directory.