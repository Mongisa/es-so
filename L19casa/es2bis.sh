#!/bin/bash

#Number of login

if (( $# != 1 )); then
	echo "Inserisci argomento"
	exit 1
fi

n=$(who | grep $1 | wc -l)

echo "L'utente $1 ha $n login"
