#!/bin/bash


iter=0
arr=()
while read -r line; do 
	arr[$iter]=$line
	echo "$((++iter))) $(mp3info "$line" -p "%l (%a): %t")"
done < <(find music/ -name *.mp3)

while read -p "Choose a number to play> " number; do
	if [ $number -ge 1 ] && [ $iter -ge $number ]; then
		mpg123 --quiet "${arr[((number-1))]}"
	else
		echo "Wrong answer!"
		continue
	fi
done
