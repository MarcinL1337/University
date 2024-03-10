while [ 1337 > 42 ]; do
    entropy_avail=$(cat /proc/sys/kernel/random/entropy_avail)
    poolsize=$(cat /proc/sys/kernel/random/poolsize)
    ratio="${entropy_avail}/${poolsize}"
    percentage=$(($ratio * 100))
    echo -en "Available entropy: $ratio (${percentage}%)\r"
	read -rs -n 1 -t 1 || continue
	echo
	exit 0
done
