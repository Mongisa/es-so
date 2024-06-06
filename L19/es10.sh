#!/bin/bash

if [[ $#!=1 ]] && [[ ! -d $1 ]];then
	echo "SYNTAX: es10.sh <directory>"
	exit 1
fi

cd $1

r=$(find . -name "*[[:upper:]]*" -type f)

for name in $r;do
	newname=$(echo $name | tr A-Z a-z)
	mv $name $newname
	echo "$name to $newname"
done

exit 0
