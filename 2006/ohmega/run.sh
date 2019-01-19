#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

inputs=(input1.txt mult.2d input2.txt)

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
