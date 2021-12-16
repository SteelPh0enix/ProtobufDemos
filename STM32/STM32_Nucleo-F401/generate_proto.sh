#!/bin/sh

if [ $# -eq 0 ]; then
    echo "You have to pass the path to protoc directory (the one with 'generator' directory) - if you don't have it, get it from here: https://jpa.kapsi.fi/nanopb/download/";
    echo "You only need the latest nanopb-*.tar.gz archive, not any OS-dependent ones.";
    exit 1;
fi

script_path="$1/generator/nanopb_generator.py"
proto_name="ExampleProto"

if [ -f "$script_path" ]; then
    python3 $script_path -I=../proto "$proto_name.proto"
    mv "$proto_name.pb.c" Core/Src/nanopb
    mv "$proto_name.pb.h" Core/Inc/nanopb
else
    echo "$script_path does not exists!";
fi