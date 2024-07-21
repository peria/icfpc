#!/bin/bash

solver="solver/target/release/solver"
for i in {1..18};
do
    problem="spaceship${i}.txt"
    solution="spaceship${i}.txt"
    echo "Start solving ${problem}."
    cat problems/${problem} | \
      ./${solver} ${i} | \
      tee solutions/${solution} | \
      ../icfp/main.py
done
