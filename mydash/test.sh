#!/bin/sh

for poolsize in 10
do
	for sleeptime in 10
	do
		for items in 100 1000
		do
			for p in  1 2 3 4 5 
			do
				for c in 1 2 3 4 5
				do
					pc $poolsize $items $p $c $sleeptime  > /dev/null
				
				done
			done
		done
	done
done
