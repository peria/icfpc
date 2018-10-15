#!/bin/sh

ROOT_DIR=$(realpath $(dirname $(dirname $0)))
# TODO: Use arg params
SOLVER=${ROOT_DIR}/src/solver
LIGHTNING_DIR=${ROOT_DIR}/contest/lightning
TMP_DIR=/tmp
MODEL_DIR=${LIGHTNING_DIR}/problemsL
TMP_TRACE_DIR=${TMP_DIR}/trace

cd ${MODEL_DIR}
models=$(ls *.mdl)

if [ ! -d ${TMP_TRACE_DIR} ]; then
  mkdir -p ${TMP_TRACE_DIR}
fi

cd ${TRACE_DIR}
for model in ${models}
do
  id=${model:0:5}
  trace=${id}.nbt
  ${SOLVER} --label=${id} \
    --dst=${MODEL_DIR}/${model} \
    --trace=${TMP_TRACE_DIR}/${id}.nbt \
    --info=${TMP_TRACE_DIR}/${id}.info
done
