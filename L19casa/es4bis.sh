#!/bin/bash

#Password

n=3

while [[ $pw != "secret" ]]; do
	if ((n-- == 0)); then
		echo -e "Troppi tentativi!"
		exit 1;
	fi
	read -p "Inserisci password: " -s pw
	echo $'\n'
done
exit 0
