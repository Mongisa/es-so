#!/bin/bash

filename=out.txt

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

if [[ -f $filename ]];then
	rm $filename
fi

str=""

for name in $1/*;do
	if [[ -f $name ]];then
		str="$str$name "
	fi
done

if [[ $2 == "up" ]]; then
	r=$(wc -l $str --total=never | sort -n)
else
	r=$(wc -l $str --total=never | sort -nr)
fi

for name in $r;do
	if [[ -f $name ]];then
		cat $name >> $filename
	fi
done

echo $(cat $filename | wc -l --total=never)

exit 0
