#!/bin/bash

while true; do
	random=$((RANDOM%100+3))
	./../Lista2/zad4.sh $random > /tmp/mylog.fifo
	sleep 1
done
