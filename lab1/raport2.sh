#!/bin/bash
mkdir raporty
make all-static
./zad 10000 1000 1 Create > ./raporty/raport2.txt
./zad 10000 1000 1 Find 2 >> ./raporty/raport2.txt
./zad 10000 1000 1 DelAdd 10000 >> ./raporty/raport2.txt
./zad 10000 1000 1 DelAddAlternating 10000 >> ./raporty/raport2.txt
echo "Dynamic loading">>./raporty/raport2.txt
echo " ">> ./raporty/raport2.txt
./zad 10000 1000 2 Create >> ./raporty/raport2.txt
./zad 10000 1000 2 Find 2 >> ./raporty/raport2.txt
./zad 10000 1000 2 DelAdd 10000 >> ./raporty/raport2.txt
./zad 10000 1000 2 DelAddAlternating 10000 >> ./raporty/raport2.txt
make clean


