#!/bin/bash

err="SYNTAX: <directory> <up|down>"

if [[ $# != 2 ]] || [[ ! -d $1 ]];then
	echo $err
	exit 1
fi

if [[ $2 != "up" ]] && [[ $2 != "down" ]];then
	echo $err
	exit 1
fi

cd $1

r=$(find . -type f)

if [[ $2 == "up" ]];then
	wc -l $r | sort -n | grep -v "total"
else
	wc -l $r | sort -nr | grep -v "total"
fi
exit 0
