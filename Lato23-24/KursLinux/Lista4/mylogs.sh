#!/bin/bash

mkfifo /tmp/mylog.fifo

while true; do
	while read -r log; do
		echo "$(date '+%Y-%m-%d %H:%M:%S') $log"
	done < /tmp/mylog.fifo
done
