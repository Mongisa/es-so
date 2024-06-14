# sava
# Progettare uno script che prende 4 parametri, di cui i primi 3 sono pathname di dir,
# e l'ultimo è una stringa esclusivamente numerica.

# Lo script deve cercare i files presenti nella sotto directory specificata nel
# secondo parametro, la cui dir genitore è la dir specificata dal primo parametro.
# I files che deve cercare lo script devono essere copiati all'interno della sotto_dir_dest
# specificata nel terzo parametro, la cui dir genitore è sempre la dir specificata dal primo parametro,
# solo se i files hanno una dimensione > x Bytes, x specificato nel quarto parametro.
# In questa situazione lo script deve copiare il file nella sotto_dir_dest, SENZA sovrascrivere,
# per cui se nella sotto_dir_dest è già presente un file con lo stesso nome allora 
# non deve avvenire la copia

# se i files hanno una dimensione <= x Bytes, allora avviene uno spostamento e non la copia,
# considerando sempre che non deve avvenire l'overwrite.

# Per completezza stampare un mess per notificare l'avvenuta copia o spostamento 
# del file con il suo nome, ed eventualmente l'impossibilità nel farlo per evitare la sovrascrittura

# CONTROLLI:
# lo script deve controllare che i primi 3 parametri siano esistenti e che siano dir,
# e che il parametro 4 sia una stringa esclusivamente numerica,
# altrimenti stampare un mess di errore, senza creare cartelle.

# ESEMPIO:
# ./main.sh dir dir/sotto_dir_source dir/sotto_dir_dest 20
# produce come output:
# file1.txt -> copiato      in dir/sotto_dir_dest
# file2.txt -> non spostato in dir/sotto_dir_dest, per non sovrascrivere
# file3.txt -> non copiato  in dir/sotto_dir_dest, per non sovrascrivere
# file4.txt -> spostato     in dir/sotto_dir_dest

# SUGGERIMENTI:
# 1) invocare il comando rm -r $1 per evitare di dover cancellare manualmente i file creati
# 2) 2 modi per vedere i Bytes: tramite il comando wc -c o con l'opzione -size di find

# --- queste istruzioni creano i file di prova  ---
#!/bin/bash

mkdir dir
mkdir dir/sotto_dir_source
mkdir dir/sotto_dir_dest

#4 file di prova nella sotto cartella dir_source
cat > dir/sotto_dir_source/file1.txt <<EOF
ciao, io ho una dimensione > 20B sicuro
mi devi spostare o copiare?
EOF

cat > dir/sotto_dir_source/file2.txt <<EOF
ciao sono sava
EOF

cat > dir/sotto_dir_source/file3.txt <<EOF
ciao sono sava e sono grande grosso e 
vaccinato
EOF

cat > dir/sotto_dir_source/file4.txt <<EOF
ciaooo
EOF

#3 file di prova nella sotto cartella dir_dest
cat > dir/sotto_dir_dest/file2.txt <<EOF
ti prego non mi eliminare
EOF

cat > dir/sotto_dir_dest/file3.txt <<EOF
ti prego non mi eliminare
EOF

cat > dir/sotto_dir_dest/irrilevante.txt <<EOF
sono di passaggio
EOF

#------------- Inserire qui il codice dello script -------------

err="SYNTAX: $0 <directory> <directory> <directory> <byte-size>"

if [[ $# != 4 ]] || [[ ! -d $1 ]] || [[ ! -d $2 ]] || [[ ! -d $3 ]]; then
    echo $err
    exit 1
fi

if [[ $4 == *[[:alpha:]]* || $4 == *[[:punct:]]* ]]; then
    echo $err
    exit 1
fi

if [[ $(find $1 -name $(basename $2) | wc -l) == 0 ]]; then
    echo "$(basename $2) non è una sottodirectory di $1"
    exit 2
fi

if [[ $(find $1 -name $(basename $3) | wc -l) == 0 ]]; then
    echo "$(basename $3) non è una sottodirectory di $1"
    exit 2
fi

for e in $2/*; do
    name=$(basename $e)
    if [[ -f $e ]]; then
        
        if [[ $(cat $e | wc -c) > $4 ]]; then
            if [[ $(find $3 -maxdepth 1 -name "$name" -type f | wc -l) == 0 ]]; then
                cp $e $3/$name
                echo "$name -> copiato in $3"
            else
                echo "$name -> non copiato in $3, per non sovrascrivere"
            fi
        else
            if [[ $(find $3 -maxdepth 1 -name "$name" -type f | wc -l) == 0 ]]; then
                mv $e $3/$name
                echo "$name -> spostato in $3"
            else
                echo "$name -> non spostato in $3, per non sovrascrivere"
            fi
        fi
        
    fi
done

rm -r $1
