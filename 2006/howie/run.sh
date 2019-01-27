#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

inputs=(input.txt)
#inputs=(input.txt power.adv usb.adv display.adv progress.adv)

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
