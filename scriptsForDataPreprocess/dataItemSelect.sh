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
#head $dataPath_2017/batch_instance-with_heads.csv
#awk -F , 'END{print "lines="NR;}'  $dataPath_2017/batch_instance.csv
#awk -F , 'END{print "lines="NR;}'  $dataPath_2017/batch_instance-with_heads.csv
#pip --version
#pip install csvkit
#head $dataPath_2017/batch_instance-with_heads.csv | csvlook


#We will select the item from file "batch_instance.csv".
#head $dataPath_2017/batch_instance-with_heads.csv | csvlook > $dataPath_2017/temp-batch_instance_heads_look
echo "" > $dataPath_2017/batch_instance_just_selected.csv
awk -F , 'BEGIN{ i = 20 } { if( i > 0 && $1 > 0 && $2 > $1 && $6 == "Terminated" && $9 > 0 && $10 > 0 && $11 > 0 && $12 > 0 ) { i -= 1; print $1","$2","$9","$10","$11","$12 ; } }' ${dataPath_2017}/batch_instance.csv
#awk -F , '{ if( $1 > 0 && $2 > $1 && $6 == "Terminated" && $9 > 0 && $10 > 0 && $11 > 0 && $12 > 0 ) { print $1","$2","$9","$10","$11","$12  >> "'${dataPath_2017}'/batch_instance_just_selected.csv"; } }' ${dataPath_2017}/batch_instance.csv
#awk -F , 'BEGIN{ OFS=","} { if( $1 > 0 && $2 > $1 && $6 == "Terminated" && $9 > 0 && $10 > 0 && $11 > 0 && $12 > 0 ) { print $1,$2,$9,$10,$11,$12  >> "'${dataPath_2017}'/batch_instance_just_selected.csv"; } }' ${dataPath_2017}/batch_instance.csv

#head -300 "$dataPath_2017/batch_instance_just_selected.csv" | csvlook > "$dataPath_2017/temp-batch_instance_selected_look"
head -30 "$dataPath_2017/batch_instance_just_selected.csv" > ./temp
head -300 "$dataPath_2017/batch_instance_just_selected.csv" | csvlook
