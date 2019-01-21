#!/bin/zsh

BASE_DIR=$(cd $(dirname $0); pwd)
UM_DIR=${BASE_DIR}/um

users=("bbarker" "ftd" "gardener" "guest" "hmonk" "howie" "ohmega")

for user in "${users[@]}"
do
  cd ${BASE_DIR}/${user}
  ./run.sh
done
