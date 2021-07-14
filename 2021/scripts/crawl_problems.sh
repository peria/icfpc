#!/bin/sh

if [ $# -eq "0" ] ; then
    ids=`seq 1 132`
else
    ids=$@
fi

for id in $ids
do
    curl -o ${id}.problem.json https://poses.live/problems/${id}/download
done
