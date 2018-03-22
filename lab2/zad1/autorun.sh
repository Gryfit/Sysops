#!/bin/bash
echo "" >> ./wyniki.txt
make all

record=2000
size=4
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3

record=1500
size=4
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3



record=2000
size=512
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3

record=1500
size=512
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3


record=2000
size=4096
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3

record=1500
size=4096
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3


record=2000
size=8192
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3

record=1500
size=8192
mode="sys"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
./main generate tmp $record $size
echo "    copy:" >> ./wyniki.txt
./main copy tmp tmp2 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp $record $size $mode >>./wyniki.txt
mode="lib"
echo "record = $record size = $size mode = $mode " >> ./wyniki.txt
echo "    copy:" >> ./wyniki.txt
./main copy tmp2 tmp3 $record $size $mode >>./wyniki.txt
echo "    sort:" >> ./wyniki.txt
./main sort tmp2 $record $size $mode >>./wyniki.txt
rm tmp tmp2 tmp3


make clean
