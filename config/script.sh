#!/bin/bash

mkdir build && cd build
if [ -z "$COVERALLS" ]
then
cmake -DCOVERALLS=On -DCMAKE_BUILD_TYPE=Debug ..
else
cmake -DCMAKE_BUILD_TYPE=Debug ..
fi
cmake --build .
cd ../
./build/test
