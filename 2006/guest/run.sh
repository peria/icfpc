#!/bin/sh

UM_DIR=$(pwd)/../um

${UM_DIR}/um ${UM_DIR}/umix.um < input.txt > output.txt
fgrep '@999999' output.txt
