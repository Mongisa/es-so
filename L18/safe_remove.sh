#!/bin/bash

if (( $# < 1 ));then
	echo "Inserisci path"
	exit 1
fi

for arg in $*; do
	if [[ ! -f $arg ]]; then
		echo "$arg non e' un file"
		exit 1
	fi
done

if [[ ! -d recycle ]]; then
	mkdir recycle
fi

for arg in $*; do
	cp $arg ./recycle
	rm $arg
done

