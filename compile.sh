#!/bin/bash

files=`find . -name '*.*' -a \( -name '*.cpp' \)`

name=$1

if [ -z "$name" ]; then
    name="Tic-Tac-Toe"
fi

g++ -I./include -L./lib $files -o $name -g -lGL -lglut -lIL -lGLU
