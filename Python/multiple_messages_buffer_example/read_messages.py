from protos.ExampleMessage_pb2 import Company, Person
import struct

INPUT_FILENAME = 'companies.bin'


def main():
    print('This example reads the messages stored in file and re-constructs them')

    with open(INPUT_FILENAME, 'br') as input:
        while True:
            try:
                message_length = struct.unpack('H', input.read(2))[0]
            except struct.error:
                print('End of data stream, exiting...')
                break

            print('Reading message with length {0}...'.format(message_length))

            message_data = input.read(message_length)
            company = Company()
            company.ParseFromString(message_data)

            print("Read message:\n{0}".format(company))


if __name__ == '__main__':
    main()
