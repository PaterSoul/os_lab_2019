#!/bin/bash
count=$#
sum=0
for x in "$@"; do
sum=$((sum + x))
done

if [ "$count" -gt 0 ]; then
avr=$((sum / count))
else
avr=0
fi
echo "count: $count"
echo "avr: $avr"
