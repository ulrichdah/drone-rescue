count=30
for i in $(seq $count); do
    echo '----' >> /home/docker/drone-rescue/sim/controller/src/argos/results/relay_4040_20_belief.txt
    argos3 -c search_example.argos
    git checkout ../sample_maps/fake0.txt
done