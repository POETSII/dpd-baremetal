#!/bin/bash

clear
clear

cd ..
make clean
make clean-tinsel

tests=(test
       test-new-verlet
       test-large
       test-new-verlet-large
       test-bonds
       test-bonds-new-verlet
       test-dt-change
       test-new-verlet-dt-change
       test-large-dt-change
       test-new-verlet-large-dt-change
       test-bonds-dt-change
       test-bonds-new-verlet-dt-change
       test-reduced-local-calcs
       test-new-verlet-reduced-local-calcs
       test-large-reduced-local-calcs
       test-new-verlet-large-reduced-local-calcs
       test-bonds-reduced-local-calcs
       test-bonds-new-verlet-reduced-local-calcs
       test-dt-change-reduced-local-calcs
       test-new-verlet-dt-change-reduced-local-calcs
       test-large-dt-change-reduced-local-calcs
       test-new-verlet-large-dt-change-reduced-local-calcs
       test-bonds-dt-change-reduced-local-calcs
       test-bonds-new-verlet-dt-change-reduced-local-calcs
       test-gals
       test-gals-new-verlet
       test-gals-large
       test-gals-new-verlet-large
       test-gals-bonds
       test-gals-bonds-new-verlet
       test-gals-dt-change
       test-gals-new-verlet-dt-change
       test-gals-large-dt-change
       test-gals-new-verlet-large-dt-change
       test-gals-bonds-dt-change
       test-gals-bonds-new-verlet-dt-change
       test-gals-reduced-local-calcs
       test-gals-new-verlet-reduced-local-calcs
       test-gals-large-reduced-local-calcs
       test-gals-new-verlet-large-reduced-local-calcs
       test-gals-bonds-reduced-local-calcs
       test-gals-bonds-new-verlet-reduced-local-calcs
       test-gals-dt-change-reduced-local-calcs
       test-gals-new-verlet-dt-change-reduced-local-calcs
       test-gals-large-dt-change-reduced-local-calcs
       test-gals-new-verlet-large-dt-change-reduced-local-calcs
       test-gals-bonds-dt-change-reduced-local-calcs
       test-gals-bonds-new-verlet-dt-change-reduced-local-calcs
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
      exit $result
    fi
    cd ..
    sleep 10s
}

for t in ${tests[@]}; do
  build_and_run $t
done

echo "ALL TESTING PASSED"
echo "====================================================================="

cd tests
