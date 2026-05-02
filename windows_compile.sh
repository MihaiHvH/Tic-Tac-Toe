#!/bin/bash

name=$1
if [ -z "$name" ]; then
    name="Tic-Tac-Toe.exe"
fi

# 1. Setup paths and flags
# We add -DFREEGLUT_STATIC to make sure headers match your .a file
INCLUDES="-I./include"
FLAGS="-O3 -s -static -DFREEGLUT_STATIC -DIL_STATIC_LIB"

# 2. Define Libraries in the correct order (Dependencies go AFTER)
# Note: freeglut usually needs winmm and gdi32 for windowing/timers
LIBS="-L./lib/windows -lfreeglut -lDevIL -lopengl32 -lglu32 -lwinmm -lgdi32 -lcomdlg32 -luser32 -lkernel32"

# 3. Find source files
CPP_FILES=$(find . -name "*.cpp")

# 4. Compile and Link
echo "Compiling for Windows..."
x86_64-w64-mingw32-g++ $INCLUDES $FLAGS $CPP_FILES -o "$name" $LIBS

if [ $? -eq 0 ]; then
    echo "Successfully Built: $name"
else
    echo "Build Failed."
fi
