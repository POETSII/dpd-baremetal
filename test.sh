#!/bin/bash

cd ./bin

i=0
s=3
while [[ $s -le 60 ]]
do
    echo -n "$s, " >> ../timing_results.csv
    while [[ $i -lt 3 ]]
    do
        t=$((s*30))
        echo "Timeout: $t seconds"
        timeout $t ./run $s
        if [[ $? -ne 0 ]] ; then
            i=$((i - 1))
        elif [[ $i -lt 2 ]] ; then
            echo -n ", " >> ../timing_results.csv
        fi
        i=$((i + 1))
        sleep 10s
    done
    i=0
    echo >> ../timing_results.csv
    s=$((s + 1))
done
