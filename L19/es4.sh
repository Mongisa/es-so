#!/bin/bash

#Password

while [[ $pw != "secret" ]]; do
	read -p "Inserisci password: " -s pw
	echo $'\n'
done
exit 0
