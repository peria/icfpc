#!/bin/zsh

UM_DIR=$(cd $(dirname $0)/../um; pwd)

#inputs=(input.txt downloader.adv)
inputs=(input.txt uploader.adv gc2.rml input2.txt)

cat ${inputs} | ${UM_DIR}/um ${UM_DIR}/umix.um | tee log.txt
