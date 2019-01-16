#!/bin/zsh

UM_DIR=$(pwd)/um
USERS=(ftd guest hmonk)
for user in ${USERS}
do
    ${UM_DIR}/um ${UM_DIR}/umix.um < ${user}/input.txt > ${user}/log.txt
    fgrep '@999999' ${user}/log.txt
done
