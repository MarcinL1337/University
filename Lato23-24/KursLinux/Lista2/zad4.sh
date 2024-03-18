#!/bin/bash

PRIMES=(2)
i=1
MAX=${1:-1000}
for (( n=3; n<=MAX; n++ )); do
	j=0
	while ((PRIMES[j]*PRIMES[j] <= n)); do
		if ((n % PRIMES[j] == 0)); then
			continue 2
		fi
		((j++))
	done
	PRIMES[$((i++))]=$n
done
echo "${PRIMES[@]}"

