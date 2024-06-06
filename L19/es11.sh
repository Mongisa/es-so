#!/bin/bash

if [[ $# < 2 ]] && [[ ! -f $1 ]];then
	echo "SYNTAX: ./es11.sh <directory> <VOTO|CFU>"
	exit 1
fi

media=0
counter=0

cd $1

for e in "./*";do
	n=$(cat $e | grep $2 | awk '{print $2}')
	media=$(($media+$n))
	((counter++))
done

#((media=media/counter))

#echo "Media $counter"

exit 0
