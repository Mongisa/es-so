#!/bin/bash

if (($#!=1));then
	echo "SYNTAX: es9.sh <string>"
	exit 1
fi

r=$(find . -name "*$1*" -type f)
count=$(find . -name "*$1*" -type f | wc -l)

echo $r
echo "File totali: $count"

exit 0
