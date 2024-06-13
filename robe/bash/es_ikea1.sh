#!/bin/bash
# Si scriva uno script shell che prenda in input esattamente 2 parametri
# e che verifichi se il primo è una cartella. Se non è una cartella stampa un messaggio
# di errore e termina.
# Lo script deve analizzare i file (non le directory) contenuti nella cartella
# data in input come primo parametro e creare un log il cui nome è passato come secondo parametro. 
# Se il file i-simo comincia con una lettera minuscola bisogna salvare nel file di log il nome del file,
# il path e il numero di parole che contiene. Se il file i-simo comincia con una lettera maiuscola si
# deve salvare nel file di log il nome, il path e la seconda stringa (se non c'è allora lascia in bianco).
# Lo script deve inoltre stampare il numero di file che iniziano con una lettera minuscola e il numero di
# file che iniziano con una lettera maiuscola

mkdir myfiles
mkdir myfiles/myfiles2

cat > myfiles/file1.txt << EOF
sopra la panca
la capra campa
sotto niente
EOF

cat > myfiles/File2.txt << EOF
EOF

cat > myfiles/file3.txt << EOF

SAVAGAY
EOF

if [[ $# != 3 ]] || [[ ! -d $1 ]];then
    echo "SYNTAX: <directory> <string>"
    exit 1
fi

a=0
b=0

for e in ./$1/*;do

    if [[ -f $e ]]; then
        name=$(basename $e)
        if [[ ${name:0:1} == [[:upper:]] ]]; then
            if [[ $(cat $e | wc -l) -lt 2 ]]; then
                echo "$name $e"
            else
                str=$(head -n 2 $e | tail -n 1)
                echo "$name $e $str"
            fi
            ((a++))
        elif [[ ${name:0:1} == [[:lower:]] ]]; then
            n=$(cat $e | wc -w)
            echo "$name $e $n"
            ((b++))
        fi
    fi
done > $2

echo "$a files iniziano con lettera maiuscola"
echo "$b files iniziano con lettera minuscola"
