#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

inputs=(input1.txt
        000.black input_rep.txt
        010.black input2.txt)

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
