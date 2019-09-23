#!/bin/bash
TIMEFORMAT=%R
code="$PWD"
opts=-g
cd . > /dev/null
time g++ $opts $code/main.c -o linux_sdl2vb -lSDL2
cd $code > /dev/null
