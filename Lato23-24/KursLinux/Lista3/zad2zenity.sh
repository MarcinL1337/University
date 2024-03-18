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


while CHOICE=$(zenity --title="Choose a song to play" --list --column="Song number" --column="Song name" --width=600 --height=400 "${arr[@]}"); do
    
    if zenity --question --text="Are you sure you want to play this song?"; then
    	mpg123 --quiet "${paths[((CHOICE-1))]}"
    fi


done

