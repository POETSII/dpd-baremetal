make clean
make clean-tinsel

boxes_x=2
boxes_y=2

s=3
max=80
make timed-gals-vesicle-fastest
cd ./bin
while [[ $s -le $max ]]
do
    ./run $s --boxes-x $boxes_x --boxes-y $boxes_y
    if [[ $? -eq 0 ]] ; then
        echo >> ../vesicle-config-time.csv
        s=$((s + 1))
    fi
    echo "Finished"
    sleep 15s
done

cd ..
make clean
make clean-tinsel

s=$((max + 1))
max=125
make timed-gals-vesicle-fastest-dram
cd ./bin
while [[ $s -le $max ]]
do
    ./run $s --boxes-x $boxes_x --boxes-y $boxes_y
    if [[ $? -eq 0 ]] ; then
        echo >> ../vesicle-config-time.csv
        s=$((s + 1))
    fi
    echo "Finished"
    sleep 15s
done

cd ..
make clean
make clean-tinsel
