make clean
make clean-tinsel

# 1 BOX
s=3
max=40
# echo "Improved GALS One by one with new verlet DPD $s - $max" >> mega_results.csv
echo "Configuration time for volume lengths $s - $max" >> config_time.csv
make timed-improved-gals-obo-new-verlet
cd ./bin
while [[ $s -le $max ]]
do
    ./run $s
    if [[ $? -eq 0 ]] ; then
        s=$((s + 1))
    fi
    echo
    sleep 30s
done

cd ..
make clean
make clean-tinsel

# 1 BOX
s=41
max=150
# echo "Improved GALS One by one with new verlet DPD $s - $max" >> mega_results.csv
echo "Configuration time for volume lengths $s - $max" >> config_time.csv
make timed-improved-gals-obo-new-verlet-dram
cd ./bin
while [[ $s -le $max ]]
do
    ./run $s
    if [[ $? -eq 0 ]] ; then
        s=$((s + 1))
    fi
    echo
    sleep 30s
done

cd ..
make clean
make clean-tinsel
