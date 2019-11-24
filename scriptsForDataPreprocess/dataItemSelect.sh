#!/bin/bash

#This shell file will help me to select the item I need in file bach_instance.csv.

dataPath="/mnt/shared/Datasets/bobowang/For_this_Experiment"
dataPath_2017=$dataPath"/trace_201708"
dataPath_2018=$dataPath"/alibaba_clusterdata_v2018"

#First deal with dataset "batch_instance.csv" at 2017.

ls -alh $dataPath_2017
#cp $dataPath_2017/batch_instance.csv $dataPath_2017/batch_instance-with_heads.csv
#head $dataPath_2017/batch_instance-with_heads.csv
#sed -i '1 istart_timestamp,end_timestamp,job_id,task_id,machineID,status,seq_no,total_seq_no,real_cpu_max,real_cpu_avg,real_mem_max,real_mem_avg' $dataPath_2017/batch_instance-with_heads.csv
#head $dataPath_2017/batch_instance-with_heads.csv
awk -F , 'END{print "lines="NR;}'  $dataPath_2017/batch_instance.csv
awk -F , 'END{print "lines="NR;}'  $dataPath_2017/batch_instance-with_heads.csv
#pip --version
#pip install csvkit
head $dataPath_2017/batch_instance-with_heads.csv | csvlook
docker container ls -a
docker image ls -a
