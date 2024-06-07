#!/bin/bash

err="SYNTAX: ./es11.sh <source_directory> <target_directory> <filename> <VOTO|CFU>"

if [[ $# < 4 ]] || [[ ! -d $1 ]] || [[ -f $2 ]];then
	echo $err
	exit 1
fi

if [[ $4 != "VOTO" ]] && [[ $4 != "CFU" ]];then
	echo $err
	exit 1
fi

if [[ ! -e $2 ]];then
	mkdir $2
fi

cd $1

if [[ -e $3 ]];then
	rm $3
fi

for file in ./*;do
	if [[ -f $file ]];then
		echo $(cat $file | grep $4 | awk '{print $2}')
	fi
done > ../$2/$3

somma=0
count=0

while read row;do
	somma=$((somma+row))
	((count++))
done < ../$2/$3

media=$(echo "scale=2; $somma/$count" | bc -l)

echo "Media: $media"

exit 0
