#!/bin/sh

ROOT_DIR=$(realpath $(dirname $(dirname $0)))
# TODO: Use arg params
SOLVER=${ROOT_DIR}/src/solver
MODEL_DIR=${ROOT_DIR}/contest/lightning/problemsL
TRACE_DIR=${ROOT_DIR}/contest/lightning/bestTracesL

cd ${MODEL_DIR}
models=$(ls *.mdl)

cd ${TRACE_DIR}
for model in ${models}
do
  id=${model:0:5}
  trace=${id}.nbt
  time ${SOLVER} --dst=${MODEL_DIR}/${model} --trace=${TRACE_DIR}/${trace}
done
