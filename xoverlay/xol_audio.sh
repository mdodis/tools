#!/bin/bash
pidof ./xoverlay > /dev/null && {
	exit
}

./xoverlay -l 0 -m 100 -c "pamixer --get-volume"