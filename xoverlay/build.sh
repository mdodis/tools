#!/bin/bash
TIMEFORMAT=%R
code="$PWD"
opts=-g
cd . > /dev/null
time g++ $opts $code/xoverlay.cpp -o xoverlay -lX11 -lXfixes -lXcomposite -lcairo -I /usr/include/cairo -lpthread
cd $code > /dev/null
