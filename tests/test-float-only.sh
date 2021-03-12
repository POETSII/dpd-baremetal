#!/bin/bash

clear
clear

cd ..
make clean
make clean-tinsel

tests=(test-float-only
       test-float-only-better-verlet
       test-float-only-dt-change
       test-float-only-better-verlet-dt-change
       test-float-only-single-force-loop
       test-float-only-single-force-loop-better-verlet
       test-float-only-single-force-loop-dt-change
       test-float-only-single-force-loop-better-verlet-dt-change
       test-float-only-reduced-local-calcs
       test-float-only-reduced-local-calcs-better-verlet
       test-float-only-reduced-local-calcs-dt-change
       test-float-only-reduced-local-calcs-better-verlet-dt-change
       test-float-only-reduced-local-calcs-single-force-loop
       test-float-only-reduced-local-calcs-single-force-loop-better-verlet
       test-float-only-reduced-local-calcs-single-force-loop-dt-change
       test-float-only-reduced-local-calcs-single-force-loop-better-verlet-dt-change
       test-gals-float-only
       test-gals-float-only-better-verlet
       test-gals-float-only-dt-change
       test-gals-float-only-better-verlet-dt-change
       test-gals-float-only-single-force-loop
       test-gals-float-only-single-force-loop-better-verlet
       test-gals-float-only-single-force-loop-dt-change
       test-gals-float-only-single-force-loop-better-verlet-dt-change
       test-gals-float-only-reduced-local-calcs
       test-gals-float-only-reduced-local-calcs-better-verlet
       test-gals-float-only-reduced-local-calcs-dt-change
       test-gals-float-only-reduced-local-calcs-better-verlet-dt-change
       test-gals-float-only-reduced-local-calcs-single-force-loop
       test-gals-float-only-reduced-local-calcs-single-force-loop-better-verlet
       test-gals-float-only-reduced-local-calcs-single-force-loop-dt-change
       test-gals-float-only-reduced-local-calcs-single-force-loop-better-verlet-dt-change
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
