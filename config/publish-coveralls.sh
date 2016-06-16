#!/bin/bash

if [ $ZPD_COV == 'on' ]
then
coveralls -e build/CMakeFiles/feature_tests.cxx -e build/CMakeFiles/feature_tests.c -e build/CMakeFiles/CompilerIdCXX/CMakeCXXCompilerId.cpp -e build/CMakeFiles/CompilerIdCXX/CMakeCXXCompilerId.c -e build/CMakeFiles/3.2.2/CompilerIdCXX/CMakeCXXCompilerId.cpp -e build/CMakeFiles/3.2.2/CompilerIdC/CMakeCCompilerId.c -e test -e pd -e thread --gcov-options '\-lp'
fi
