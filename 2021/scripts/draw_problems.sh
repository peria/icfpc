#!/bin/sh

SCRIPT_DIR=$(cd $(dirname $0); pwd)
ROOT_DIR=$(dirname ${SCRIPT_DIR})
PROBLEM_DIR=${ROOT_DIR}/data/problems
WWW_DIR=${ROOT_DIR}/www
PROBLEM_IMG_DIR=${WWW_DIR}/static/images/problems

ids=`seq 1 132`
cd ${WWW_DIR}
for id in $ids
do
    json=${PROBLEM_DIR}/${id}.problem.json
    png=${PROBLEM_IMG_DIR}/${id}.problem.png
    ./visualize.py ${json} -o ${png}
done
