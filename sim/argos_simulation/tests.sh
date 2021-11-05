## declare an array for the test suffixes
declare -a test_types=("4040_10_" "4040_15_" "4040_20_" "3030_10_" "3030_15_" "3030_20_" "2020_10_" "2020_15_" "2020_20_")
nb_tests=30

for test_type in "${test_types[@]}"
    echo "========= TEST $test_type ========="
    for i in $(seq $nb_tests); do
        echo '----' >> /home/docker/drone-rescue/sim/controller/src/argos/results/relay_"$test_type"random.txt
        argos3 -c search_example.argos
        git checkout ../sample_maps/fake0.txt
        nlines=`wc --lines < ../controller/src/argos/results/target_"$test_type"random.txt`
        if [ $nlines -gt $i ]
        then
            delete_start=$((i + 1))
            sed -i "$delete_start,$nlines d" ../controller/src/argos/results/target_"$test_type"random.txt
        fi
        echo "EXPERIMENT $i done!\n"
    done
    echo "RANDOM DONE!\n"

    sed -i '11s/0/1/' config/search_example/parameters.bzz
    sed -i '13s/random/belief/' ../controller/src/argos/buzz_controller_drone_rescue_sim.cpp
    sed -i '14s/random/belief/' ../controller/src/argos/buzz_controller_drone_rescue_sim.cpp
    cd ../controller/build/ && make clean && make && make install && cd ../../argos_simulation && rm search_example.bdb search_example.bo target_drift_example.bdb target_drift_example.bo && bzzc search_example.bzz && bzzc target_drift_example.bzz
    for i in $(seq $nb_tests); do
        echo '----' >> /home/docker/drone-rescue/sim/controller/src/argos/results/relay_"$test_type"belief.txt
        argos3 -c search_example.argos
        git checkout ../sample_maps/fake0.txt
        nlines=`wc --lines < ../controller/src/argos/results/target_"$test_type"belief.txt`
        if [ $nlines -gt $i ]
        then
            delete_start=$((i + 1))
            sed -i "$delete_start,$nlines d" ../controller/src/argos/results/target_"$test_type"belief.txt
        fi
        echo "EXPERIMENT $i done!\n"
    done
    echo "BELIEF DONE!\n"

    sed -i '11s/1/0/' config/search_example/parameters.bzz
    sed -i '13s/belief/random/' ../controller/src/argos/buzz_controller_drone_rescue_sim.cpp
    sed -i '14s/belief/random/' ../controller/src/argos/buzz_controller_drone_rescue_sim.cpp
    cd ../controller/build/ && make clean && make && make install && cd ../../argos_simulation && rm search_example.bdb search_example.bo target_drift_example.bdb target_drift_example.bo && bzzc search_example.bzz && bzzc target_drift_example.bzz
done 