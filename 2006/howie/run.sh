#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

#inputs=(input.txt)
inputs=(input.txt display.adv power.adv usb.adv progress.adv jumper.adv)

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
