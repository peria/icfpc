#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

inputs=(input1.txt arith.adv input2.txt xml.adv input3.txt)

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
