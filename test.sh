#!/bin/bash

cd ./bin

i=0
s=10
while [[ $s -le 40 ]]
do
    echo -n "$s, " >> ../timing_results.csv
    while [[ $i -lt 3 ]]
    do
        t=$((s*20))
        echo "Timeout: $t seconds"
        timeout $t ./run $s
        if [[ $? -ne 0 ]] ; then
            i=$((i - 1))
        fi
        if [[ $i -lt 4 ]] ; then
            echo -n ", " >> ../timing_results.csv
        fi
        i=$((i + 1))
        sleep 10s
    done
    i=0
    echo >> ../timing_results.csv
    s=$((s + 1))
done
