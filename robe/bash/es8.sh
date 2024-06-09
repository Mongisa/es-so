#!/bin/bash

if [[ $# != 1 ]] || [[ ! -d $1 ]] ;then
	echo "SYNTAX: <directory>"
	exit 1
fi

cd $1

r=$(find . -maxdepth 1 -type f -name "*[A-Z]*");

for e in $r;do
	n=$(echo $e | tr A-Z a-z)
	mv $e $n
done

exit 0
