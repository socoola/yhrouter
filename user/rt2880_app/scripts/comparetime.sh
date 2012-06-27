#/bin/sh

#add by zsf v1.4.27 date 2010-12-1 
# $1 Ah   $2   Am   $3 Bh   $4 Bm
#A > B exit 6
#A < B exit 4
#A = B exit 5 


if [ $# -eq 4 ];then
	if [ $1 -lt $3 ];then
		echo "4"
	elif [ $1 -gt $3 ];then
		echo "6"
	else
		if [ $2 -lt $4 ];then
			echo "4"
		elif [ $2 -gt $4 ];then
			echo "6"
		else
			echo "5"
		fi	
	fi
else
	echo "1"
fi
