#!/bin/zsh

BASE_DIR=$(cd $(dirname $0); pwd)

users=("bbarker" "ftd" "gardener" "guest" "hmonk" "howie" "ohmega")

for user in "${users[@]}"
do
  logfile=${BASE_DIR}/${user}/log.txt
  if [ -e ${logfile} ]
  then
    grep '@999999' ${logfile} | sed -e "s/^[^A-Z]\+\\s*//"
  fi
done
