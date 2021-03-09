#!/bin/bash

export CMAKE_GENERATOR="MinGW Makefiles"

cd build && cmake .. && make && cd .. && ./bin/main.exe ./sample/s1.txt