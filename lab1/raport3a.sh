#!/bin/bash
mkdir raporty
make all-static optim=0
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`
echo "Static loading">./raporty/raport3a.txt 
echo " " >> raport3a.txt
./zad 10000 1000 1 Create >> ./raporty/raport3a.txt 
./zad 10000 1000 1 Find 2 >> ./raporty/raport3a.txt 
./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 1 DelAddAlternating 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 Create >> ./raporty/raport3a.txt 
./zad 10000 1000 2 Find 2 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3a.txt 
make clean
make all-dynamic optim=0
echo "Daynamic loading">>./raporty/raport3a.txt
echo " " >> raport3a.txt
./zad 10000 1000 1 Create >> ./raporty/raport3a.txt 
./zad 10000 1000 1 Find 2 >> ./raporty/raport3a.txt 
./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 1 DelAddAlternating 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 Create >> ./raporty/raport3a.txt 
./zad 10000 1000 2 Find 2 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3a.txt 
make clean
make all-shared optim=0
echo "Shared loading" >> ./raporty/raport3a.txt
echo " " >> raport3a.txt
./zad 10000 1000 1 Create >> ./raporty/raport3a.txt 
./zad 10000 1000 1 Find 2 >> ./raporty/raport3a.txt 
./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 1 DelAddAlternating 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 Create >> ./raporty/raport3a.txt 
./zad 10000 1000 2 Find 2 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3a.txt 
./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3a.txt 
make clean
