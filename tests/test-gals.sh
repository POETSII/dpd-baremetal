#!/bin/bash

cd ..
make clean
make clean-tinsel

tests=(test-gals
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
