#!/bin/bash

# find all .mp3 files in music directory
iter=0
index=1
arr=()
paths=()
while read -r line; do
	paths[$((index-1))]=$line
	arr[$iter]="$index"
	arr[$((iter + 1))]=$(mp3info "$line" -p "%l (%a): %t")
	iter=$((iter + 2))
	((++index))
done < <(find music/ -name *.mp3)

CHOICE=$(dialog --menu "Choose a song to play" 22 76 16 "${arr[@]}" 3>&1 1>&2 2>&3)

if [ -z "$CHOICE" ]; then
	clear
	echo "No option was chosen"
else
	clear
	echo "You're playing: ${arr[$((2*CHOICE - 1))]}"
	mpg123 --quiet "${paths[((CHOICE-1))]}"
fi

