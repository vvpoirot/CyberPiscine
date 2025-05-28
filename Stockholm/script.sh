#!/bin/bash
mkdir -p /home/infection/

touch "/home/infection/b.isnotextention"
echo "Random file" >> "/home/infection/b.isnotextention"
touch "/home/infection/zuble.aaa"
echo "Random file" >> "/home/infection/zuble.aaa"
touch "/home/infection/becrypted.jpg.ft"
echo "Random file" >> "/home/infection/becrypted.jpg.ft"

for VAR in 1 2 3 4 5 6 7
do
    touch "/home/infection/another_file$VAR.txt"
    echo "Random string $VAR $VAR $VAR $VAR " >> "/home/infection/another_file$VAR.txt"
done