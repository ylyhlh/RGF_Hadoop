#!/usr/bin/env bash

set -e

dryrun=$1

# MR1 sets $mapred_map_tasks
# MR2/YARN sets $mapreduce_job_maps
nmappers=$mapred_map_tasks

# MR1 sets $mapreduce_job_submithost
# MR2/YARN sets $mapreduce_job_submithostname
submit_host=$mapreduce_job_submithost

# MR1 sets $mapred_output_dir
# MR2/YARN sets $mapreduce_output_fileoutputformat_outputdir
output_dir=$mapred_output_dir

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
echo 'Starting training' > /dev/stderr

# Prepare training data
TRAINFILE=train.dat
cat > $TRAINFILE

cut -f1 $TRAINFILE > $TRAINFILE.y
cut -f2- $TRAINFILE > $TRAINFILE.x
SPEEDTEST_LEN=100000
SPEEDTEST_NTIMES=100
rgfcmd="./speedTest --length $SPEEDTEST_LEN --times  $SPEEDTEST_NTIMES --master $submit_host --unique_id 1245 --total $nmappers --node_id $mapper"
#perl test/call_exe.pl ./bin/rgf train_test test/sample/msd_03 localhost 1233 1 0
echo $rgfcmd > /dev/stderr


$rgfcmd > /dev/stderr

