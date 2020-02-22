#! /bin/bash
protoc --grpc_out=../protoCode --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto
protoc --cpp_out=../protoCode *.proto