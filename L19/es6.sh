#!/bin/bash

if [[ $# -lt 2 ]];then
	echo -n "Inserisci n righe: "
	read r

	echo -n "Inserisci n colonne: "
	read c
else
	r=$1
	c=$2
fi

n=0
cr=0
cc=0
str=""

while [[ cr -lt r ]]; do
	while [[ cc -lt c ]]; do
		str="$str$n "
		((n++))
		((cc++))
	done
	echo $str
	str=""
	cc=0
	((cr++))
done

exit 0
