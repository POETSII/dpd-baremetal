#!/bin/bash

cd ./bin

i=0
s=3
while [[ $s -le 100 ]]
do
    while [[ $i -lt 5 ]]
    do
        if [[ $i -eq 0 ]] ; then
            ./run $s 1
        else
            ./run $s
        fi
        if [[ $i -lt 4 ]] ; then
            echo -n ", " >> ../timing_results.csv
        fi
        i=$((i + 1))
        make print-stats -C ..
        sleep 5s
    done
    i=0
    echo >> ../timing_results.csv
    s=$((s + 1))
done
