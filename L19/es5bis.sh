#!/bin/bash

if [[ $# < 2 ]];then
	echo "SYNTAX: <input_file> <output_file>"
	exit 1
fi

if [[ ! -f $1 ]] || [[ ! -f $2 ]];then
	echo "SYNTAX: <input_file> <output_file>"
	exit 1
fi

counter=1

while read row;do
	echo "$((counter++)) $row"
done < $1 > $2

exit 0
