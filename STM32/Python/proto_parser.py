from ExampleProto_pb2 import DataPacket, DeviceResponse, DeviceRequest, LED


class ProtobufCommParser:
    def create_start_request() -> bytes:
        request = DeviceRequest()
        request.code = DeviceRequest.RequestType.START
        return request.SerializeToString()

    def create_stop_request() -> bytes:
        request = DeviceRequest()
        request.code = DeviceRequest.RequestType.STOP
        return request.SerializeToString()

    def create_led_toggle_request(state: bool) -> bytes:
        request = DeviceRequest()
        request.code = DeviceRequest.RequestType.CHANGE_LED_STATE

        led = LED()
        led.state = state
        request.ledState.CopyFrom(led)

        return request.SerializeToString()

    def parse_response(response_data: bytes) -> DeviceResponse:
        response = DeviceResponse()
        response.ParseFromString(response_data)
        return response

    def parse_data_packet(packet_data: bytes) -> DataPacket:
        data_packet = DataPacket()
        data_packet.ParseFromString(packet_data)
        return data_packet
