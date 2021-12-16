@echo off

protoc -I=../proto --python_out=. ExampleProto.proto