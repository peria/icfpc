#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

inputs=(input.txt ) # usb.adv display.adv

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
