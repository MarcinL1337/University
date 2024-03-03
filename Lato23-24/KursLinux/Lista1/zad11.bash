#!/bin/bash

seq="$1"
prev_seq=""

length=${#seq}
iteration=0

while [ "$seq" != "$prev_seq" ]; do
	prev_seq="$seq"
	echo "Iteration: $iteration, $seq"
	for ((i = 0; i < length-1; i++)); do
		char="${seq:i:1}"
		charr="${seq:i+1:1}"
		if [[ "$char" != "$charr" ]]; then
			if [ "$char" == "G" ] && [ "$charr" == "R" ]; then
				seq=$(echo "$seq" | sed 's/GR/BB/')
				break
			fi
			if [ "$char" == "G" ] && [ "$charr" == "B" ]; then
				seq=$(echo "$seq" | sed 's/GB/RR/')
				break
			fi
			if [ "$char" == "R" ] && [ "$charr" == "B" ]; then
				seq=$(echo "$seq" | sed 's/RB/GG/')
				break
			fi
			if [ "$char" == "R" ] && [ "$charr" == "G" ]; then
				seq=$(echo "$seq" | sed 's/RG/BB/')
				break
			fi
			if [ "$char" == "B" ] && [ "$charr" == "R" ]; then
				seq=$(echo "$seq" | sed 's/BR/GG/')
				break
			fi
			if [ "$char" == "B" ] && [ "$charr" == "G" ]; then
				seq=$(echo "$seq" | sed 's/BG/RR/')
				break
			fi
		fi
	done
	iteration=$((iteration+1))
done
