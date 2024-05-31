#!/bin/bash

if [[ $# != 2 ]] || [[ ! -d $1 ]];then
	echo "Arg err"
	exit 1
fi

if [[ $2 != "up" ]] && [[ $2 != "down" ]];then
	echo "Accepted only up/down text"
	exit 1
fi

if [[ -z $(ls $1) ]];then
	echo "$1 is empty"
	exit 1
fi

str=""

for name in $1/*;do
	if [[ -f $name ]];then
		str="$str$name "
	fi
done

if [[ $2 == "up" ]]; then
	wc -l $str --total=never | sort -n
else
	wc -l $str --total=never | sort -nr
fi

exit 0
