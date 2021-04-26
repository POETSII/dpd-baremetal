#!/bin/bash

# Clean tinsel and dpd-baremetal directories for good measure
make clean -C ..
make clean-tinsel -C ..

top_dir=sync_single_bond_graphs

# Table of forces to be switch on/off
# We are only using interesting ones
# We disable CONS, DRAG and RAND - We enable BOND hence being inverted
# Think of a 1 representing a flag for that bond, not for it being enable or disabled
# CONS | DRAG | RAND | BOND
#   0  |   0  |   0  |   1  # All enabled
#   0  |   0  |   0  |   0  # Bonds disabled
#   0  |   0  |   1  |   1  # Rand disabled
#   0  |   0  |   1  |   0  # Rand and bonds disabled
#   0  |   1  |   0  |   1  # Drag disabled
#   0  |   1  |   0  |   0  # Drag and bonds disabled
#   0  |   1  |   1  |   1  # Drag and rand disabled
#   0  |   1  |   1  |   0  # Drag, rand and bonds disabled
#   1  |   0  |   0  |   1  # Cons disabled
#   1  |   0  |   0  |   0  # Cons and bonds disabled
#   1  |   0  |   1  |   1  # Cons and rand disabled
#   1  |   0  |   1  |   0  # Cons, rand and bonds disabled
#   1  |   1  |   0  |   1  # Cons and drag disabled
#   1  |   1  |   0  |   0  # Cons, drag and bonds disabled
#   1  |   1  |   1  |   1  # Cons, drag and rand disabled
#   1  |   1  |   1  |   0  # All disabled

function build_and_run {
    cd ..
    make clean
    echo $1
    EXTRA_FLAGS=$extra_flags make visual-sync-oil-water-bonds
    cd bin/
    pass=1
    while [[ pass -ne 0 ]]
    do
        ./run 25 --time 2010
        pass=$?
        sleep 10s
    done
    cd ../utils/
}

function perform_analysis {
    # python3 init-dist-graph.py
    # mv min-dists.pdf ./$top_dir/$1/min-dists-$1.pdf
    python3 temp-graph.py
    mv temp.pdf ./$top_dir/temp-$1.pdf
    python3 bond-graph.py
    mv test.pdf ./$top_dir/$1.pdf
}

# CONS | DRAG | RAND | BOND
#   0  |   0  |   0  |   1  # All enabled
echo "All enabled"
dir_name=all-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   0  |   0  |   0  |   0  # Bonds disabled
echo "Bonds disabled"
dir_name=cons-drag-rand-endabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   0  |   0  |   1  |   1  # Rand disabled
echo "Rand disabled"
dir_name=cons-drag-bonds-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_RAND_FORCE\ -DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   0  |   0  |   1  |   0  # Rand and bonds disabled
echo "Rand and bonds disabled"
dir_name=cons-drag-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_RAND_FORCE
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   0  |   1  |   0  |   1  # Drag disabled
echo "Drag disabled"
dir_name=cons-rand-bonds-endabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_DRAG_FORCE\ -DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   0  |   1  |   0  |   0  # Drag and bonds disabled
echo "Drag and bonds disabled"
dir_name=cons-rand-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_DRAG_FORCE
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   0  |   1  |   1  |   1  # Drag and rand disabled
echo "Drag and rand disabled"
dir_name=cons-bonds-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_DRAG_FORCE\ -DDISABLE_RAND_FORCE\ -DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   0  |   1  |   1  |   0  # Drag, rand and bonds disabled
echo "Drag, rand and bonds disabled"
dir_name=cons-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_DRAG_FORCE\ -DDISABLE_RAND_FORCE
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   0  |   0  |   1  # Cons disabled
echo "Cons disabled"
dir_name=drag-rand-bonds-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE\ -DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   0  |   0  |   0  # Cons and bonds disabled
echo "Cons and bonds disabled"
dir_name=drag-rand-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   0  |   1  |   1  # Cons and rand disabled
echo "Cons and rand disabled"
dir_name=drag-bonds-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE\ -DDISABLE_RAND_FORCE\ -DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   0  |   1  |   0  # Cons, rand and bonds disabled
echo "Cons, rand and bonds disabled"
dir_name=drag-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE\ -DDISABLE_RAND_FORCE
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   1  |   0  |   1  # Cons and drag disabled
echo "Cons and drag disabled"
dir_name=rand-bonds-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE\ -DDISABLE_DRAG_FORCE\ -DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   1  |   0  |   0  # Cons, drag and bonds disabled
echo "Cons, drag and bonds disabled"
dir_name=rand-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE\ -DDISABLE_DRAG_FORCE
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   1  |   1  |   1  # Cons, drag and rand disabled
echo "Cons, drag and rand disabled"
dir_name=bonds-enabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE\ -DDISABLE_DRAG_FORCE\ -DDISABLE_RAND_FORCE\ -DBONDS
build_and_run $extra_flags
perform_analysis $dir_name

# CONS | DRAG | RAND | BOND
#   1  |   1  |   1  |   0  # All disabled
echo "All disabled"
dir_name=all-disabled
# mkdir -p ./$top_dir/$dir_name/
extra_flags=-DDISABLE_CONS_FORCE\ -DDISABLE_DRAG_FORCE\ -DDISABLE_RAND_FORCE
build_and_run $extra_flags
perform_analysis $dir_name
