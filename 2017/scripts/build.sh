#!/bin/zsh

rootdir="$(cd $(dirname "$0")/..; pwd)"
outdir="$rootdir/out/Default"
srcdir="$rootdir/src"

declare -a cmake_opts
cmake_opts+=("-GNinja")
cmake_opts+=("-DCMAKE_C_COMPILER=clang")
cmake_opts+=("-DCMAKE_CXX_COMPILER=clang++")

mkdir -p $outdir
cd $outdir
cmake "${cmake_opts[@]}" $srcdir
ninja "$@"
