# Multiple messages in single buffer example

This example shows one way of storing multiple messages in single data source - in this case, binary file.

Since protobuf does not provide any delimeters or other mechanics of telling where one message ends, and another begins, you have to do it yourself. This example stores message size in first two bytes to achieve that.

Run `generate_messages.py` to create some example messages and save them to file. Then, run `read_messages.py` to read them.

Run `generate_protos.sh` to re-generate Python proto code.