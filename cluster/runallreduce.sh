#!/usr/bin/env bash

set -e

dryrun=$1

# MR1 sets $mapred_map_tasks
# MR2/YARN sets $mapreduce_job_maps
nmappers=$mapreduce_job_maps

# MR1 sets $mapreduce_job_submithost
# MR2/YARN sets $mapreduce_job_submithostname
submit_host=$mapreduce_job_submithostname

# MR1 sets $mapred_output_dir
# MR2/YARN sets $mapreduce_output_fileoutputformat_outputdir
output_dir=$mapreduce_output_fileoutputformat_outputdir

set -u

# This works on both MR1 and MR2/YARN
mapper=`printenv mapred_task_id | cut -d "_" -f 5`
mapred_job_id=`echo "$mapred_job_id" | tr -d 'job_'`

# debug
echo $mapper > /dev/stderr
echo $nmappers > /dev/stderr
echo $output_dir > /dev/stderr
echo $submit_host > /dev/stderr

echo 'Starting training' > /dev/stderr

# Prepare training data
TRAINFILE=train.dat
cat > $TRAINFILE

cut -f1 $TRAINFILE > $TRAINFILE.y
cut -f2- $TRAINFILE > $TRAINFILE.x

# Feed into allreduce_test
#"./vw -b 24 --total $mapred_map_tasks --node $mapper --unique_id $mapred_job_id --cache_file temp.cache --passes 1 --regularization=1 --adaptive --exact_adaptive_norm -d /dev/stdin -f tempmodel --span_server $mapreduce_job_submithost --loss_function=logistic"

artestcmd="./artest $submit_host $nmappers $mapper --job count_file --file $TRAINFILE.y"
echo $artestcmd > /dev/stderr
#$artestcmd