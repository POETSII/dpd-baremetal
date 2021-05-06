#!/bin/bash

clear
clear

cd ..
make clean
make clean-tinsel

tests=(test-serial
test-serial-betterverlet
test-serial-dtchange
test-serial-doublesqrt
test-serial-reducedlocalcalcs
test-serial-singleforceloop
test-serial-large
test-serial-large-betterverlet
test-serial-large-dtchange
test-serial-large-doublesqrt
test-serial-large-reducedlocalcalcs
test-serial-large-singleforceloop
test-serial-bonds
test-serial-bonds-betterverlet
test-serial-bonds-dtchange
test-serial-bonds-doublesqrt
test-serial-bonds-reducedlocalcalcs
test-serial-bonds-singleforceloop
)

function build_and_run {
    make clean
    echo
    echo "======================= RUNNING $1 ======================="
    make $1
    cd bin
    timeout 60s ./test
    result=$?
    if [[ $result -ne 0 ]] ; then
        echo "$1 failed"
        exit $result
    fi
    cd ..
}

for t in ${tests[@]}; do
  build_and_run $t
done

echo "ALL TESTING PASSED"
echo "====================================================================="

cd tests
