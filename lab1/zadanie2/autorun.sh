#!/bin/bash
rm raport2.txt
./zad2 1000 1000 1 Create > raport2.txt
./zad2 1000 1000 1 Find 2 >> raport2.txt
./zad2 1000 1000 1 DelAdd 100 >> raport2.txt
./zad2 1000 1000 1 DelAddAlternating 100 >> raport2.txt
./zad2 1000 1000 2 Create >> raport2.txt
./zad2 1000 1000 2 Find 2 >> raport2.txt
./zad2 1000 1000 2 DelAdd 100 >> raport2.txt
./zad2 1000 1000 2 DelAddAlternating 100 >> raport2.txt


