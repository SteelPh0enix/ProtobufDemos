# STM32 <-> PC communication example - Python project

This project contains example code that can be used to communicate with STM32 via Protobuf API.

The communication is done via UART peripheral, accesible via USB thanks to UART-to-USB converter implemented on STLink/V2 debugger, since i'm using Nucleo board - however, any UART-to-USB converter supported by your OS will do the job here.

The version of Python i'm using is 3.9, and i don't intend to support any other. However, it should work just fine on slightly newer or older versions (you may need to change the version in `Pipfile`, if you're not using 3.9)

## Running the project with pipenv

If you wanna use prepared virtualenv for this project, run `pipenv run main.py [SERIAL PORT]` - everything should set-up automatically and the script should run, talking to STM32 and printing the results in console. Replace `[SERIAL PORT]` with STM32 port path, for example `COM3` or `/dev/ttyACM0`. This script will always list available COM devices, so you can run it without providing serial port name to check which port is the STM32.

If you've configured different baudrate, pass it as optional argument `--baud`, for example: `pipenv run main.py /dev/ttyACM0 --baud 921600`. The default baudrate is `115200`, and script will always print currently used baudrate.

## Running the project manually

Install the dependencies manually, via `pip`, or other package manager. This script requires only `pyserial` and `protobuf` to run.
