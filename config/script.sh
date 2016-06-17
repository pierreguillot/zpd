#!/bin/bash

mkdir build && cd build
if [ $TRAVIS_OS_NAME = 'linux' ] && [ $COMPILER = 'gcc' ]
then
cmake -DCOVERALLS=On -DCMAKE_BUILD_TYPE=Debug ..
else
cmake -DCMAKE_BUILD_TYPE=Debug ..
fi
cmake --build .
cd ../
./build/test
