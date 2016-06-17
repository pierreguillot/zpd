#!/bin/bash

mkdir zpd
cp -R lib zpd/lib
cp -R cpd zpd/cpd
cp -R xpd zpd/xpd
cp README.md zpd/readme.txt
cp LICENSE.txt zpd/license.txt

zip -r zpd_${TRAVIS_OS_NAME}_${TRAVIS_TAG}.zip zpd
