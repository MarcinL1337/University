#!/bin/bash

while true; do
	free > /tmp/mylog.fifo
	sleep 3
done
