#!/bin/bash
count=$#
if [ $count -eq 0 ]; then
    echo "Количество аргументов: 0"
    echo "Среднее арифметическое: 0"
    exit 0
fi

sum=0
for num in "$@"; do
    sum=$((sum + num))
done

average=$((sum / count))

echo "Количество аргументов: $count"
echo "Среднее арифметическое: $average"
