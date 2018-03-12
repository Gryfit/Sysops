#!/bin/bash
rm raport3b.txt

for (( i=0; $i <= 3; i++ )) ; do
       echo " Optymalizacja O$i" >> raport3b.txt
	echo " " >> raport3b.txt
	make all-static optim=$i
	echo "Static loading" >> raport3b.txt
	echo " " >> raport3b.txt
	./zad3 10000 1000 1 Create >> raport3b.txt 
	./zad3 10000 1000 1 Find 2 >> raport3b.txt 
	./zad3 10000 1000 1 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 1 DelAddAlternating 100 >> raport3b.txt
	./zad3 10000 1000 2 Create >> raport3b.txt 
	./zad3 10000 1000 2 Find 2 >> raport3b.txt 
	./zad3 10000 1000 2 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 2 DelAddAlternating 100 >> raport3b.txt
	make clean
	make all-dynamic optim=$i
	echo "Daynamic loading">>raport3b.txt
	echo " " >> raport3b.txt
	./zad3 10000 1000 1 Create >> raport3b.txt 
	./zad3 10000 1000 1 Find 2 >> raport3b.txt 
	./zad3 10000 1000 1 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 1 DelAddAlternating 100 >> raport3b.txt
	./zad3 10000 1000 2 Create >> raport3b.txt 
	./zad3 10000 1000 2 Find 2 >> raport3b.txt 
	./zad3 10000 1000 2 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 2 DelAddAlternating 100 >> raport3b.txt
	make clean
	make all-shared optim=$i
	echo "Shared loading" >> raport3b.txt
	echo " " >> raport3b.txt
	./zad3 10000 1000 1 Create >> raport3b.txt 
	./zad3 10000 1000 1 Find 2 >> raport3b.txt 
	./zad3 10000 1000 1 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 1 DelAddAlternating 100 >> raport3b.txt
	./zad3 10000 1000 2 Create >> raport3b.txt 
	./zad3 10000 1000 2 Find 2 >> raport3b.txt 
	./zad3 10000 1000 2 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 2 DelAddAlternating 100 >> raport3b.txt 
	make clean
done
        echo "Optymalizacja Os" >> raport3b.txt
	echo " " >> raport3b.txt
	make all-static optim='s'
	echo "Static loading">>raport3b.txt
	echo " " >> raport3b.txt
	./zad3 10000 1000 1 Create >> raport3b.txt 
	./zad3 10000 1000 1 Find 2 >> raport3b.txt 
	./zad3 10000 1000 1 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 1 DelAddAlternating 100 >> raport3b.txt
	./zad3 10000 1000 2 Create >> raport3b.txt 
	./zad3 10000 1000 2 Find 2 >> raport3b.txt 
	./zad3 10000 1000 2 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 2 DelAddAlternating 100 >> raport3b.txt
	make clean
	make all-dynamic optim='s'
	echo "Daynamic loading">>raport3b.txt
	echo " " >> raport3b.txt
	./zad3 10000 1000 1 Create >> raport3b.txt 
	./zad3 10000 1000 1 Find 2 >> raport3b.txt 
	./zad3 10000 1000 1 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 1 DelAddAlternating 100 >> raport3b.txt
	./zad3 10000 1000 2 Create >> raport3b.txt 
	./zad3 10000 1000 2 Find 2 >> raport3b.txt 
	./zad3 10000 1000 2 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 2 DelAddAlternating 100 >> raport3b.txt
	make clean
	make all-shared optim='s'
	echo "Shared loading" >> raport3b.txt
	echo " " >> raport3b.txt
	./zad3 10000 1000 1 Create >> raport3b.txt 
	./zad3 10000 1000 1 Find 2 >> raport3b.txt 
	./zad3 10000 1000 1 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 1 DelAddAlternating 100 >> raport3b.txt
	./zad3 10000 1000 2 Create >> raport3b.txt 
	./zad3 10000 1000 2 Find 2 >> raport3b.txt 
	./zad3 10000 1000 2 DelAdd 1000 >> raport3b.txt 
	./zad3 10000 1000 2 DelAddAlternating 100 >> raport3b.txt 
	make clean
