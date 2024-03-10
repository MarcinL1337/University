#!/bin/bash

PRIMES=(2)
i=1
n=3
MAX=$1
while [ $MAX -ge $n ]; do
	j=0
	#echo "$n $((${PRIMES[$j]}*${PRIMES[$j]}))"
	while [ $n -ge $((${PRIMES[$j]}*${PRIMES[$j]})) ]; do
		if [[ $(($n % ${PRIMES[$j]})) -eq 0 ]]; then
			((n++))
			continue 2
		fi
		((j++))
		#echo "$n"
	done
	PRIMES[$((i++))]=$n;
	((n++))
done
echo "${PRIMES[@]}"

