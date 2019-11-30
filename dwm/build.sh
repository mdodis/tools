#!/bin/bash
TIMEFORMAT=%R
code="$PWD"
time make EXTRA_FLAGS=-DDWM_USER_SCRIPT_DIRECTORY="/home/miked/scripts/"
