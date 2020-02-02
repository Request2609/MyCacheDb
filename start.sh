#!/bin/bash
cd server
protoc --cpp_out=. msg.proto
mv msg.pb.cc msg.pb.cpp
make
rm *.o
cd -
cd client
#编译.proto文件，生成相应的.cpp文件
protoc --cpp_out=. msg.proto
mv msg.pb.cc msg.pb.cpp
make
ar rcs -o libclient.a *.o
cp libclient.a ../test
mv libclient.a lib
rm *.o
cd -
