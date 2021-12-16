import serial.tools.list_ports as list_ports
from serial.tools.list_ports_common import ListPortInfo
import argparse
import proto_parser
import proto_comm

comm = None


def port_to_string(port: ListPortInfo) -> str:
    return f'{port.device} - {port.description} - {port.name} - {port.manufacturer}'


def print_available_ports():
    port_list = [port_to_string(p) for p in list_ports.comports()]
    print('List of COM ports available:')
    for port in port_list:
        print(port)

    print('')


def main():
    print_available_ports()
    parser = argparse.ArgumentParser()
    parser.add_argument(
        'port', help='STM32 UART port, for example COM3 or /dev/ttyACM0', type=str)
    parser.add_argument('--baudrate', help='STM32 baudrate',
                        default=115200, type=int)

    args = parser.parse_args()
    print(f'Using port {args.port} with baudrate {args.baudrate}')

    comm = proto_comm.ProtobufComm(args.port, args.baudrate)
    comm.start_data_output()

    try:
        while True:
            comm.handle_data()
            # comm.toggle_led()
    except KeyboardInterrupt:
        print('I\'m done here!')
        comm.stop_data_output()
        exit(0)


if __name__ == '__main__':
    main()
