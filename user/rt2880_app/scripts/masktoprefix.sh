count_mask()
{
    MASK=$1
    count=0
    for num in `echo -e $MASK|tr '.' '\n'`; do
        while [ ! $num -eq 0 ] ; do
            rem=`expr $num % 2 `
            if [ $rem -eq 1 ];then 
            	 count=`expr $count + 1`
           fi
            num=`expr $num / 2 `
        done
    done
    echo "$count"
}


count_mask $1






