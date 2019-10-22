#!/bin/bash

cd ./bin

i=0
s=50
while [[ $s -le 70 ]]
do
    echo -n "$s, " >> ../timing_results.csv
    while [[ $i -lt 5 ]]
    do
        timeout 250 ./run $s
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
