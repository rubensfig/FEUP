#!/bin/bash

if [ ! -f download ]
then
    make 
fi

./download ftp://[Anonymous:qqmerda@]ftp.up.pt/pub/CentOS/2/readme.txt

make clean
