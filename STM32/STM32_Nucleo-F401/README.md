# STM32 Protobuf API project

## Re-generating the protobuf

For Linux, you have a handy script `generate_proto.sh` that you can use for that. You'll have to provide path to `nanopb` directory in order for it to work correctly, as the only argument. It'll automatically generate proto C files and put them in correct directories (Core/Inc/nanopb and Core/Src/nanopb)

For Windows, you'll have to do it manually - run the command below and copy the output files to correct directories. I assume you're running this command while being in this directory - if not, correct the `-I` path.

`python3 [path_to_nanopb]/generator/nanopb_generator.py -I=../proto ExampleProto.proto`

## Migration instructions

If you wanna run this on other STM32 MCU or board, you'll have to:

- Make a CubeIDE/CubeMX project
- Configure the MCU
  - [optional] Change debug mode (System Core -> SYS) from `Serial Wire` to `Trace Asynchronous Sw`, if you want to see ITM debug prints
  - Make sure UART that's accesible via USB is enabled and configured
    - Set the baud rate, default would be 115200
    - Enable it's global interrupt (NVIC Settings tab)
    - Enable it's UART RX DMA request (in normal mode, basically default settings)
  - in `Project Manager -> Code Generator` tab, make sure you've selected `Generate peripheral initialization as a pair of '.c/.h' files per peripheral` - i'm not sure if it'll be absolutely necessary, but i recommend doing that
- Copy the `app` and `nanopb` directories to a new project, setup include paths for `nanopb` directory (include path for `app` is not necessary)
- Add a call to `appSetup()` function just before the loop in `main.c`, and `appLoop()` inside the loop

And that's basically it. The actual logic code is completely separated inside `app` directory, so it should not cause any issues.

### Changing UART handle

You can change UART handle (huart2 by default) in `Core/Src/app/app.c` - change `UART_HANDLE` macro.
