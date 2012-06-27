if [ "$1" = "fast" ]; then
	echo "power_led: blink fast"
	gpio l 19 1 1 4000 1 4000 
fi
if [ "$1" = "slow" ]; then
	echo "power_led: blink slow"
	gpio l 19 3 3 4000 1 4000 
fi
if [ "$1" = "normal" ]; then
	echo "power_led: normal"
	gpio l 19 10 10 4000 1 4000 
fi
if [ "$1" = "on" ]; then
	echo "power_led: turn on"
	gpio l 19 4000 0 1 1 4000 
fi
if [ "$1" = "off" ]; then
	echo "power_led: turn off"
	gpio l 19 0 4000 1 1 4000 
fi
