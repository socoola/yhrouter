if [ "$1" = "fast" ]; then
	gpio l 4 1 1 4000 1 4000 
fi
if [ "$1" = "slow" ]; then
	gpio l 4 3 3  4000 1 4000 
fi
if [ "$1" = "on" ]; then
	gpio l 4 4000 0 1 1 4000 
fi
if [ "$1" = "off" ]; then
	gpio l 4 0 4000 1 1 4000 
fi
