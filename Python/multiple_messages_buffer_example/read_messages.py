from protos.ExampleMessage_pb2 import Company, Person
import struct

INPUT_FILENAME = 'companies.bin'


def person_to_string(person: Person) -> str:
    return '[ID: {0}] {1} ({2})'.format(person.id, person.name, person.email)


def company_to_string(company: Company) -> str:
    employees_count = len(company.employees)
    company_string = 'Name: {0}\nDirector: {1}\nEmployees count: {2}\nEmployees:\n'.format(
        company.name, person_to_string(company.director), employees_count)

    for employee in company.employees:
        company_string += '\t{0}\n'.format(person_to_string(employee))

    # also works like that:
    # for i in range(employees_count):
    # company_string += '\t{0}\n'.format(person_to_string(company.employees[i]))

    return company_string


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

            print(company_to_string(company))


if __name__ == '__main__':
    main()
