make all
echo "Satrun 1" >> ./Times.txt
echo "Satrun 1"
./main 1 ./saturn.pgm ./filter1.txt ./a.pgm
./main 2 ./saturn.pgm ./filter1.txt ./a.pgm
./main 4 ./saturn.pgm ./filter1.txt ./a.pgm
./main 8 ./saturn.pgm ./filter1.txt ./a.pgm
echo "Glassware 1"  >> ./Times.txt
echo "Glassware 1"
./main 1 ./glassware.pgm ./filter1.txt ./a.pgm
./main 2 ./glassware.pgm ./filter1.txt ./a.pgm
./main 4 ./glassware.pgm ./filter1.txt ./a.pgm
./main 8 ./glassware.pgm ./filter1.txt ./a.pgm
echo "Satrun 2"  >> ./Times.txt
echo "Satrun 2"
./main 1 ./saturn.pgm ./filter2.txt ./a.pgm
./main 2 ./saturn.pgm ./filter2.txt ./a.pgm
./main 4 ./saturn.pgm ./filter2.txt ./a.pgm
./main 8 ./saturn.pgm ./filter2.txt ./a.pgm
echo "Glassware 2"  >> ./Times.txt
echo "Glassware 2"
./main 1 ./glassware.pgm ./filter2.txt ./a.pgm
./main 2 ./glassware.pgm ./filter2.txt ./a.pgm
./main 4 ./glassware.pgm ./filter2.txt ./a.pgm
./main 8 ./glassware.pgm ./filter2.txt ./a.pgm
make clean
