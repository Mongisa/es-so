#!/bin/bash

if (($#!=1));then
	echo "SYNTAX: es7.sh <filename>"
	exit 1
fi

if [[ ! -f $1 ]];then
	echo "$1 is not a file"
	exit 1
fi

echo "$1 is a file"

if [[ -x $1 ]];then
	echo "You can execute $1"
else
	echo "You can't execute $1"
fi

exit 0
