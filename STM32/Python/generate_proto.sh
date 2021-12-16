#!/bin/sh

protoc -I=../proto --python_out=. ExampleProto.proto