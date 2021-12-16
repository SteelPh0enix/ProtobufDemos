import serial
import time
import threading
import struct
from proto_parser import ProtobufCommParser
from ExampleProto_pb2 import DataPacket, DeviceRequest, DeviceResponse


class ProtobufComm:
    def __init__(self, port: str, baudrate: int):
        self.stm = serial.Serial(port, baudrate)

        if not self.stm.is_open:
            raise RuntimeError(
                f'Couldn\'t open port {port} with baudrate {baudrate}')

        # these are used for LED async task
        self.led_state = False
        self.led_timer = None
        self.led_timer_delay = 0.5

    def read_packet_size(self, size_data: bytes) -> int:
        # using struct to unpack 16-bit unsigned value from byte array
        return struct.unpack('<H', size_data)

    def write_packet_size(self, packet_data: bytes) -> bytes:
        # similarly to read_packed_data, we use struct to pack the size into the byte array
        packet_size = len(packet_data)
        packet_data = struct.pack('<H', packet_size) + packet_data
        return packet_data

    def read_packet(self) -> bytes:
        # First, we fetch 2 bytes of size from internal serial buffer
        msg_len = -1
        msg_size_data = bytes()
        while msg_len < 0 or msg_len > 100:
            msg_size_data = self.stm.read(2)
            print(f"Read packet header: {msg_size_data.hex(' ')}")
            msg_len = self.read_packet_size(msg_size_data)[0]
        # Then, we fetch the rest of the message
        if msg_len > 0:
            msg_data = self.stm.read(msg_len)
            return msg_data
        else:
            return bytes()

    def read_response(self) -> DeviceResponse:
        # Let's try reading the packet
        msg_data = self.read_packet()
        # And we process it
        return ProtobufCommParser.parse_response(msg_data)

    def read_data_packet(self) -> DataPacket:
        # very similar to above
        msg_data = self.read_packet()
        return ProtobufCommParser.parse_data_packet(msg_data)

    def handle_response(self, response: DeviceResponse):
        if response.code == DeviceResponse.ResponseType.OK:
            print('Response OK!')
        elif response.code == DeviceResponse.ResponseType.BAD_TYPE:
            print('Bad response type!')
        elif response.code == DeviceResponse.ResponseType.PROTOBUF_ERROR:
            print('Some protobuf error happened while parsing the request on the device!')

    def start_data_output(self):
        # Let's create the request and add the size
        msg = ProtobufCommParser.create_start_request()
        msg = self.write_packet_size(msg)

        # And that's it, it's ready to go
        self.stm.write(msg)

        # Just wait until the device responds
        response = self.read_response()
        self.handle_response(response)

    def stop_data_output(self):
        # basically same as above - it could definitely be
        # shortened, since lots of code repeats, i just dont care about it now.
        # C'mon, it's just an example, no one will ever use this code in prod...
        # ...right?
        msg = ProtobufCommParser.create_start_request()
        msg = self.write_packet_size(msg)

        self.stm.write(msg)

        response = self.read_response()
        self.handle_response(response)

    def handle_data(self):
        data = self.read_data_packet()
        print(f'Packet # {data.packetNumber}\n\tValue A: {data.valueA}\n\tValue B: {data.valueB}\n\tValue C: {data.valueC}\n\tConstant value: {data.constantValue}')

    def toggle_led(self):
        print('Toggling LED!')
        # let's create proper message and encapsulate it into our packet
        msg = ProtobufCommParser.create_led_toggle_request(self.led_state)
        msg = self.write_packet_size(msg)

        # and off we go
        self.stm.write(msg)
        self.led_state = not self.led_state

        # we have to make sure the device responds before doing anything else
        response = self.read_response()
        self.handle_response(response)

    def _toggle_led_with_timer(self):
        self.toggle_led()
        self.led_timer = threading.Timer(
            self.led_timer_delay, self._toggle_led_with_timer)
        self.led_timer.start()

    def start_led_toggling(self, delay: int):
        self.led_timer_delay = delay
        self.led_timer = threading.Timer(delay, self._toggle_led_with_timer)
        self.led_timer.start()

    def stop_led_toggling(self):
        self.led_timer.cancel()
