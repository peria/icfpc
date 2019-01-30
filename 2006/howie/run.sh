#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

inputs=(input.txt)
#inputs=(input.txt battery.adv rs232.adv)
#inputs=(input.txt downloader.adv)

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
