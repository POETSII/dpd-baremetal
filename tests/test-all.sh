#!/bin/bash

clear
clear

cd ..
make clean
make clean-tinsel

tests=(test-sync
test-sync-betterverlet
test-sync-dtchange
test-sync-doublesqrt
test-sync-msgmgmt
test-sync-reducedlocalcalcs
test-sync-singleforceloop
test-sync-singlereducedforceloop
test-sync-onebyone
test-sync-onebyone-betterverlet
test-sync-onebyone-dtchange
test-sync-onebyone-doublesqrt
test-sync-onebyone-msgmgmt
test-sync-onebyone-reducedlocalcalcs
test-sync-onebyone-singleforceloop
test-sync-onebyone-singlereducedforceloop
test-sync-sendtoself
test-sync-sendtoself-betterverlet
test-sync-sendtoself-dtchange
test-sync-sendtoself-doublesqrt
test-sync-sendtoself-msgmgmt
test-sync-sendtoself-singleforceloop
test-sync-sendtoself-singlereducedforceloop
test-sync-large
test-sync-large-betterverlet
test-sync-large-dtchange
test-sync-large-doublesqrt
test-sync-large-msgmgmt
test-sync-large-reducedlocalcalcs
test-sync-large-singleforceloop
test-sync-large-singlereducedforceloop
test-sync-large-onebyone
test-sync-large-onebyone-betterverlet
test-sync-large-onebyone-dtchange
test-sync-large-onebyone-doublesqrt
test-sync-large-onebyone-msgmgmt
test-sync-large-onebyone-reducedlocalcalcs
test-sync-large-onebyone-singleforceloop
test-sync-large-onebyone-singlereducedforceloop
test-sync-large-sendtoself
test-sync-large-sendtoself-betterverlet
test-sync-large-sendtoself-dtchange
test-sync-large-sendtoself-doublesqrt
test-sync-large-sendtoself-msgmgmt
test-sync-large-sendtoself-singleforceloop
test-sync-large-sendtoself-singlereducedforceloop
test-sync-bonds
test-sync-bonds-betterverlet
test-sync-bonds-dtchange
test-sync-bonds-doublesqrt
test-sync-bonds-msgmgmt
test-sync-bonds-reducedlocalcalcs
test-sync-bonds-singleforceloop
test-sync-bonds-singlereducedforceloop
test-sync-bonds-onebyone
test-sync-bonds-onebyone-betterverlet
test-sync-bonds-onebyone-dtchange
test-sync-bonds-onebyone-doublesqrt
test-sync-bonds-onebyone-msgmgmt
test-sync-bonds-onebyone-reducedlocalcalcs
test-sync-bonds-onebyone-singleforceloop
test-sync-bonds-onebyone-singlereducedforceloop
test-sync-bonds-sendtoself
test-sync-bonds-sendtoself-betterverlet
test-sync-bonds-sendtoself-dtchange
test-sync-bonds-sendtoself-doublesqrt
test-sync-bonds-sendtoself-msgmgmt
test-sync-bonds-sendtoself-singleforceloop
test-sync-bonds-sendtoself-singlereducedforceloop
test-gals
test-gals-betterverlet
test-gals-dtchange
test-gals-doublesqrt
test-gals-msgmgmt
test-gals-reducedlocalcalcs
test-gals-singleforceloop
test-gals-singlereducedforceloop
test-gals-onebyone
test-gals-onebyone-betterverlet
test-gals-onebyone-dtchange
test-gals-onebyone-doublesqrt
test-gals-onebyone-msgmgmt
test-gals-onebyone-reducedlocalcalcs
test-gals-onebyone-singleforceloop
test-gals-onebyone-singlereducedforceloop
test-gals-large
test-gals-large-betterverlet
test-gals-large-dtchange
test-gals-large-doublesqrt
test-gals-large-msgmgmt
test-gals-large-reducedlocalcalcs
test-gals-large-singleforceloop
test-gals-large-singlereducedforceloop
test-gals-large-onebyone
test-gals-large-onebyone-betterverlet
test-gals-large-onebyone-dtchange
test-gals-large-onebyone-doublesqrt
test-gals-large-onebyone-msgmgmt
test-gals-large-onebyone-reducedlocalcalcs
test-gals-large-onebyone-singleforceloop
test-gals-large-onebyone-singlereducedforceloop
test-gals-bonds
test-gals-bonds-betterverlet
test-gals-bonds-dtchange
test-gals-bonds-doublesqrt
test-gals-bonds-msgmgmt
test-gals-bonds-reducedlocalcalcs
test-gals-bonds-singleforceloop
test-gals-bonds-singlereducedforceloop
test-gals-bonds-onebyone
test-gals-bonds-onebyone-betterverlet
test-gals-bonds-onebyone-dtchange
test-gals-bonds-onebyone-doublesqrt
test-gals-bonds-onebyone-msgmgmt
test-gals-bonds-onebyone-reducedlocalcalcs
test-gals-bonds-onebyone-singleforceloop
test-gals-bonds-onebyone-singlereducedforceloop
test-improvedgals
test-improvedgals-betterverlet
test-improvedgals-dtchange
test-improvedgals-doublesqrt
test-improvedgals-reducedlocalcalcs
test-improvedgals-singleforceloop
test-improvedgals-singlereducedforceloop
test-improvedgals-onebyone
test-improvedgals-onebyone-betterverlet
test-improvedgals-onebyone-dtchange
test-improvedgals-onebyone-doublesqrt
test-improvedgals-onebyone-reducedlocalcalcs
test-improvedgals-onebyone-singleforceloop
test-improvedgals-onebyone-singlereducedforceloop
test-improvedgals-large
test-improvedgals-large-betterverlet
test-improvedgals-large-dtchange
test-improvedgals-large-doublesqrt
test-improvedgals-large-reducedlocalcalcs
test-improvedgals-large-singleforceloop
test-improvedgals-large-singlereducedforceloop
test-improvedgals-large-onebyone
test-improvedgals-large-onebyone-betterverlet
test-improvedgals-large-onebyone-dtchange
test-improvedgals-large-onebyone-doublesqrt
test-improvedgals-large-onebyone-reducedlocalcalcs
test-improvedgals-large-onebyone-singleforceloop
test-improvedgals-large-onebyone-singlereducedforceloop
test-improvedgals-bonds
test-improvedgals-bonds-betterverlet
test-improvedgals-bonds-dtchange
test-improvedgals-bonds-doublesqrt
test-improvedgals-bonds-reducedlocalcalcs
test-improvedgals-bonds-singleforceloop
test-improvedgals-bonds-singlereducedforceloop
test-improvedgals-bonds-onebyone
test-improvedgals-bonds-onebyone-betterverlet
test-improvedgals-bonds-onebyone-dtchange
test-improvedgals-bonds-onebyone-doublesqrt
test-improvedgals-bonds-onebyone-reducedlocalcalcs
test-improvedgals-bonds-onebyone-singleforceloop
test-improvedgals-bonds-onebyone-singlereducedforceloop
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
    sleep 10s
}

for t in ${tests[@]}; do
  build_and_run $t
done

echo "ALL POLITE TESTING PASSED"
echo "====================================================================="

cd tests

./test-serial.sh
