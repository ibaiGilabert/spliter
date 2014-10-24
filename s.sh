#!/bin/sh

jobID=$`/usr/local/sge/bin/linux-x64/qsub spliter/run.sh a`

jobID=`echo $jobID | awk 'match($0,/[0-9]+/){print substr($0, RSTART, RLENGTH)}'`

echo "JOB ID: $jobID"

