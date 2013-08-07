#!/bin/sh

str1=log/run
str2=.log
str3=run
str4=.mac

for((i=$2; i<($1+$2); i++))
do
in=${str3}${i}${str4}
out=${str1}${i}${str2}
./exe.sh run1.mac ${i} > ${out} &
done
