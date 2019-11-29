#!/bin/bash

#This shell file will help me to select the item I need in file batch_instance.csv.

dataPath="/mnt/shared/Datasets/bobowang/For_this_Experiment"
dataPath_2017=$dataPath"/trace_201708"
dataPath_2018=$dataPath"/alibaba_clusterdata_v2018"

#First deal with dataset "batch_instance.csv" at 2017.

#ls -alh $dataPath_2017
#cp $dataPath_2017/batch_instance.csv $dataPath_2017/batch_instance-with_heads.csv
#head $dataPath_2017/batch_instance-with_heads.csv
#sed -i '1 istart_timestamp,end_timestamp,job_id,task_id,machineID,status,seq_no,total_seq_no,real_cpu_max,real_cpu_avg,real_mem_max,real_mem_avg' $dataPath_2017/batch_instance-with_heads.csv
#echo "============================================="
#head $dataPath_2017/batch_instance-with_heads.csv
awk -F , 'END{print "batch_instance.csv-lines="NR;}'  $dataPath_2017/batch_instance.csv
#awk -F , 'END{print "lines="NR;}'  $dataPath_2017/batch_instance-with_heads.csv

#We will select the item from file "batch_instance.csv" and then calculate duration for each instance.
echo "" > $dataPath_2017/batch_instance-effective.csv
awk -F , 'BEGIN{ OFS=","} { if( $1 > 0 && $2 > $1 && $6 == "Terminated" && $9 > 0 && $10 > 0 && $11 > 0 && $12 > 0 ) { print $2-$1,$9,$10,$11,$12  >> "'${dataPath_2017}'/batch_instance-effective.csv"; } }' ${dataPath_2017}/batch_instance.csv
awk -F , 'END{print "batch_instance-effective.csv-lines="NR;}'  $dataPath_2017/batch_instance-effective.csv