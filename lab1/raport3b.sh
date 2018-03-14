#!/bin/bash
mkdir raporty
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`
       echo " " > ./raporty/raport3b.txt
for (( i=0; $i <= 3; i++ )) ; do
       echo " Optymalizacja O$i" >> ./raporty/raport3b.txt
	echo " " >> ./raporty/raport3b.txt
	make all-static optim=$i p=`pwd`
	echo "Static loading" >> ./raporty/raport3b.txt
	echo " " >> ./raporty/raport3b.txt
	./zad 10000 1000 1 Create >> ./raporty/raport3b.txt 
	./zad 10000 1000 1 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAddAlternating 100 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Create >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3b.txt
	make clean
	make all-dynamic optim=$i p=`pwd`
	echo "Daynamic loading">> ./raporty/raport3b.txt
	echo " " >> ./raporty/raport3b.txt
	./zad 10000 1000 1 Create >> ./raporty/raport3b.txt 
	./zad 10000 1000 1 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAddAlternating 100 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Create >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3b.txt
	make clean
	make all-shared optim=$i p=`pwd`
	echo "Shared loading" >> ./raporty/raport3b.txt
	echo " " >> ./raporty/raport3b.txt
	./zad 10000 1000 1 Create >> ./raporty/raport3b.txt 
	./zad 10000 1000 1 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAddAlternating 100 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Create >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3b.txt
	make clean
done
        echo "Optymalizacja Os" >>  ./raporty/raport3b.txt
	echo " " >>  ./raporty/raport3b.txt
	make all-static optim='s' p=`pwd`
	echo "Static loading">> ./raporty/raport3b.txt
	echo " " >> ./raporty/raport3b.txt
	./zad 10000 1000 1 Create >> ./raporty/raport3b.txt 
	./zad 10000 1000 1 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAddAlternating 100 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Create >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3b.txt
	make clean
	make all-dynamic optim='s' p=`pwd`
	echo "Daynamic loading">> ./raporty/raport3b.txt
	echo " " >> ./raporty/raport3b.txt
	./zad 10000 1000 1 Create >> ./raporty/raport3b.txt 
	./zad 10000 1000 1 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAddAlternating 100 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Create >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3b.txt
	make clean
	make all-shared optim='s' p=`pwd`
	echo "Shared loading" >> ./raporty/raport3b.txt
	echo " " >> ./raporty/raport3b.txt
	./zad 10000 1000 1 Create >> ./raporty/raport3b.txt 
	./zad 10000 1000 1 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 1 DelAddAlternating 100 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Create >> ./raporty/raport3b.txt
	./zad 10000 1000 2 Find 2 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAdd 1000 >> ./raporty/raport3b.txt
	./zad 10000 1000 2 DelAddAlternating 1000 >> ./raporty/raport3b.txt
	make clean

