#!/bin/bash

#This shell file will help me to select the item I need in file batch_instance.csv.

currentPath=`pwd`
dataPath="/mnt/shared/Datasets/bobowang/For_this_Experiment"
dataPath_2017=$dataPath"/trace_201708"
dataPath_2018=$dataPath"/alibaba_clusterdata_v2018"
clusterNum=6
columnNum=5

#First deal with dataset "batch_instance.csv" at 2018.

#ls -alh $dataPath_2018
#cp ${dataPath_2018}/batch_instance.csv ${dataPath_2018}/batch_instance-with_heads.csv
#head ${dataPath_2018}/batch_instance-with_heads.csv
#sed -i '1 istart_timestamp,end_timestamp,job_id,task_id,machineID,status,seq_no,total_seq_no,real_cpu_max,real_cpu_avg,real_mem_max,real_mem_avg' ${dataPath_2018}/batch_instance-with_heads.csv
#echo "============================================="
#head ${dataPath_2018}/batch_instance-with_heads.csv

#2019-12-04-002
awk -F , 'END{print "batch_instance.csv_2018-lines="NR;}'  ${dataPath_2018}/batch_instance.csv
awk -F , 'END{print "lines="NR;}'  ${dataPath_2018}/batch_instance-with_heads.csv

#We will select the item from file "batch_instance.csv", and then calculate duration for each instance.
rm -f ${dataPath_2018}/batch_instance-effective.csv
awk -F , 'BEGIN{ OFS=","} { if( $6 > 0 && $7 > $6 && $5 == "Terminated" && $11 > 0 && $12 > 0 && $13 > 0 && $14 > 0 ) { print $7-$6,$11,$12,$13,$14  >> "'${dataPath_2018}'/batch_instance-effective.csv"; } }' ${dataPath_2018}/batch_instance.csv
awk -F , 'END{print "batch_instance-effective.csv_2018-lines="NR;}'  ${dataPath_2018}/batch_instance-effective.csv

##2019-12-05-002
#cp ${dataPath_2018}/batch_instance-effective.csv ${dataPath_2018}/batch_instance-effective.csv.backup
#mkdir -p ${dataPath_2018}/midstFile
#make
#i=1
#j=1
#while [ $i -le $columnNum ]
#do
#	$currentPath/k_meanspp ${dataPath_2018}/batch_instance-effective.csv.backup ${j} $clusterNum
#	rm -f ${dataPath_2018}/batch_instance-effective.csv.backup
#	cp ${dataPath_2018}/clustered_column_${j}_based-batch_instance-effective.csv.backup ${dataPath_2018}/batch_instance-effective.csv.backup
#	mv ${dataPath_2018}/clustered_column_${j}_based-batch_instance-effective.csv.backup ${dataPath_2018}/midstFile/
#	head -300 ${dataPath_2018}/midstFile/clustered_column_${j}_based-batch_instance-effective.csv.backup >  ${dataPath_2018}/midstFile/clustered_column_${j}_based-batch_instance-effective.csv.backup-head-300.csv
#	i=`expr $i + 1`
#	j=`expr ${j} + 2`
#done
#awk -F , 'BEGIN{ OFS=","} { print $2,$4,$6,$8,$10  >> "'${dataPath_2018}'/batch_instance-usable.csv"; } END{ print "Printed total lines = "NR}' ${dataPath_2018}/batch_instance-effective.csv.backup
#rm -f ${dataPath_2018}/batch_instance-effective.csv.backup








#Doing for every time.
chmod 777 -R $dataPath_2018
