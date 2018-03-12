#!/bin/bash
rm raport3a.txt
make all-static
echo "Static loading">raport3a.txt optim=0
echo " " >> raport3a.txt
./zad3 10000 1000 1 Create >> raport3a.txt 
./zad3 10000 1000 1 Find 2 >> raport3a.txt 
./zad3 10000 1000 1 DelAdd 1000 >> raport3a.txt 
./zad3 10000 1000 1 DelAddAlternating 100 >> raport3a.txt 
./zad3 10000 1000 2 Create >> raport3a.txt 
./zad3 10000 1000 2 Find 2 >> raport3a.txt 
./zad3 10000 1000 2 DelAdd 1000 >> raport3a.txt 
./zad3 10000 1000 2 DelAddAlternating 100 >> raport3a.txt 
make clean
make all-dynamic optim=0
echo "Daynamic loading">>raport3a.txt
echo " " >> raport3a.txt
./zad3 10000 1000 1 Create >> raport3a.txt 
./zad3 10000 1000 1 Find 2 >> raport3a.txt 
./zad3 10000 1000 1 DelAdd 1000 >> raport3a.txt 
./zad3 10000 1000 1 DelAddAlternating 100 >> raport3a.txt 
./zad3 10000 1000 2 Create >> raport3a.txt 
./zad3 10000 1000 2 Find 2 >> raport3a.txt 
./zad3 10000 1000 2 DelAdd 1000 >> raport3a.txt 
./zad3 10000 1000 2 DelAddAlternating 100 >> raport3a.txt 
make clean
make all-shared optim=0
echo "Shared loading" >> raport3a.txt
echo " " >> raport3a.txt
./zad3 10000 1000 1 Create >> raport3a.txt 
./zad3 10000 1000 1 Find 2 >> raport3a.txt 
./zad3 10000 1000 1 DelAdd 1000 >> raport3a.txt 
./zad3 10000 1000 1 DelAddAlternating 100 >> raport3a.txt 
./zad3 10000 1000 2 Create >> raport3a.txt 
./zad3 10000 1000 2 Find 2 >> raport3a.txt 
./zad3 10000 1000 2 DelAdd 1000 >> raport3a.txt 
./zad3 10000 1000 2 DelAddAlternating 100 >> raport3a.txt 
make clean
