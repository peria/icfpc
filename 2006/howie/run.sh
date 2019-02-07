#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

inputs1=(input.txt downloader.adv)
inputs2=(input.txt uploader.adv gc2.rml input2.txt)
inputs3=(input3.txt race_car.adv pousse_game.adv package_robot.adv robber.adv finite_state.adv codex.adv)

cat ${inputs1} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
cat ${inputs2} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee -a log.txt
cat ${inputs3} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee -a log.txt
