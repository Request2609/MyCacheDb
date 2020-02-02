#!/bin/bash
cd server
make
rm *.o
cd -
cd client
make
ar rcs -o libclient.a *.o
rm *.o
mv *.a lib
cd -
