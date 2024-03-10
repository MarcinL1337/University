#!/bin/bash

if [[ $# == 0 ]]; then
	echo "No arugments passed"
	exit 1
fi

size_sum=$(awk '{sum += $1} END {print sum}' < <(du -s "$@"))


echo "Sum of arguments sizes: $size_sum"

#for line in $(df | tail -n +2); do
#    available=$(awk '{print $4}' <<< "$line")
#    if (( available >= TOTAL )); then
#       awk '{print $1 " mounted on " $6}' <<< "$line"
#    fi
#done


awk -v x=$size_sum '{
	if ($4 >= x)
		print $1 " mounted on " $6
}' < <(df | tail -n +2)
