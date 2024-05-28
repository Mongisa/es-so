#!/bin/bash

#Number of login

echo -n "Inserisci username: "
read u

n=$(who | grep $u | wc -l)

echo "L'utente $u ha $n login"
