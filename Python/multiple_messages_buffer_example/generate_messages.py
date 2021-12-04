from protos.ExampleMessage_pb2 import Company, Person
from random import choice, randint
import struct


NAMES = [
    'Jonathan', 'Joseph', 'Jotaro', 'Josuke', 'Giorno', 'Jolynne'
]

DOMAINS = [
    'gmail.com', 'hotmail.com', 'outlook.com'
]

EMPLOYEES_COUNT = 5
COMPANIES_COUNT = 5
OUTPUT_FILENAME = 'companies.bin'


def generate_random_employee() -> Person:
    employee = Person()
    employee.id = randint(1, 10000)
    employee.name = choice(NAMES)
    employee.email = '{0}@{1}'.format(employee.name, choice(DOMAINS))
    return employee


def generate_random_company() -> Company:
    company = Company()
    company.name = 'Company # {0}'.format(randint(1, 1000))
    # You cannot assign to a message-type field - you have to use proper method, like CopyFrom
    # to copy message content into message-type field
    # See https://googleapis.dev/python/protobuf/latest/google/protobuf/message.html for details
    company.director.CopyFrom(generate_random_employee())
    return company


def add_random_employees_to_company(company: Company, amount: int):
    employees_list = [generate_random_employee() for _ in range(amount)]
    company.employees.extend(employees_list)


def generate_example_populated_company() -> Company:
    company = generate_random_company()
    add_random_employees_to_company(company, EMPLOYEES_COUNT)
    return company


def main():
    print('This example generates some messages, and saves them to file.')

    companies = [generate_example_populated_company()
                 for _ in range(COMPANIES_COUNT)]
    encoded_companies = [c.SerializeToString() for c in companies]

    with open(OUTPUT_FILENAME, 'bw') as output:
        for company_data in encoded_companies:
            print('Writing protobuf message with length {0}...'.format(
                len(company_data)))
            output.write(struct.pack('H', len(company_data)))
            output.write(company_data)


if __name__ == '__main__':
    main()
