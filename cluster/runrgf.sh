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
TRAINFILE=train.dat
cat > $TRAINFILE

cut -f1 $TRAINFILE > $TRAINFILE.y
cut -f2- $TRAINFILE > $TRAINFILE.x

hadoop fs -copyToLocal /user/hl1283/RGF_Hadoop/test/sample/cts.test.x ./test.x
hadoop fs -copyToLocal /user/hl1283/RGF_Hadoop/test/sample/cts.test.y ./test.y

rgfcmd="perl ./call_exe.pl ./rgf train_test ./long $submit_host 1245 $nmappers $mapper"
#perl test/call_exe.pl ./bin/rgf train_test test/sample/msd_03 localhost 1233 1 0
echo $rgfcmd > /dev/stderr

if [ "$mapper" == '000000' ]
then
  $rgfcmd > /dev/stderr #> mapperout 2>&1
  if [ $? -ne 0 ] 
  then
    exit 1
  fi 
  hadoop fs -put train.evaluation $output_dir/train.evaluation
  #hadoop fs -put mapperout $output_dir/mapperout
else
  $rgfcmd > /dev/stderr
fi

