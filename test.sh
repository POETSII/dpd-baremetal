#!/bin/bash

cd ./bin

i=0
s=33
while [[ $s -le 50 ]]
do
    num=0
    while [[ $i -lt 5 ]]
    do
        if [[ $i -eq 0 ]] ; then
            if [[ $num -eq 0 ]] ; then
                echo -n "$s, " >> ../timing_results.csv
                num=1
            fi
        fi
        timeout 200s ./run $s -k
        if [[ $? -gt 0 ]] ; then
            i=$((i - 1))
        elif [[ $i -lt 4 ]] ; then
            echo -n ", " >> ../timing_results.csv
        fi
        i=$((i + 1))
        sleep 30s
    done
    i=0
    echo >> ../timing_results.csv
    s=$((s + 1))
done
