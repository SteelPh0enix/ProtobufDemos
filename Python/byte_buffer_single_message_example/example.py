# The `ExampleMessage_pb2` is generated using `protoc`, with help of `generate_protos.sh` script
from protos.ExampleMessage_pb2 import Person, Company
from random import choice, randint

NAMES = [
    "Jonathan", "Joseph", "Jotaro", "Josuke", "Giorno", "Jolynne"
]

DOMAINS = [
    "gmail.com", "hotmail.com", "outlook.com"
]


def generate_random_employee() -> Person:
    employee = Person()
    employee.id = randint(1, 10000)
    employee.name = choice(NAMES)
    employee.email = "{0}@{1}".format(employee.name, choice(DOMAINS))
    return employee


def generate_random_company() -> Company:
    company = Company()
    company.name = "Company # {0}".format(randint(1, 1000))
    # You cannot assign to a message-type field - you have to use proper method, like CopyFrom
    # to copy message content into message-type field
    # See https://googleapis.dev/python/protobuf/latest/google/protobuf/message.html for details
    company.director.CopyFrom(generate_random_employee())
    return company


def add_random_employees_to_company(company: Company, amount: int):
    employees_list = [generate_random_employee() for _ in range(amount)]
    company.employees.extend(employees_list)


def main():
    print("Hello, this is byte_buffer_example!")

    # Generate some data
    example_company = generate_random_company()
    add_random_employees_to_company(example_company, 5)

    # Encode it into byte buffer
    encoded_message = example_company.SerializeToString()

    # Now, you can do anything you want with that serialized message.
    # Save to file, send throught network, anything.
    # Here, we do nothing and just decode it to check if it's working

    decoded_company = Company()
    decoded_company.ParseFromString(encoded_message)

    # Let's print it, and see what we decoded
    print("This is how the company looks like after decoding:\n{0}".format(
        decoded_company))

    # We can also compare it to original company, to see it is *really* is the same
    if (decoded_company == example_company):
        print("Decoded company is the same as original one! Success!")
    else:
        print("Something went wrong and decoded company is not the same as encoded one!")


if __name__ == '__main__':
    main()
