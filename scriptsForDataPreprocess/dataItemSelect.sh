#!/bin/bash

#This shell file will help me to select the item I need in file bach_instance.csv.

dataPath="/mnt/shared/Datasets/bobowang/For_this_Experiment"
dataPath_2017=$dataPath"/trace_201708"
dataPath_2018=$dataPath"/alibaba_clusterdata_v2018"

#First deal with dataset "batch_instance.csv" at 2017.

ls -alh $dataPath_2017
#cp $dataPath_2017/batch_instance.csv $dataPath_2017/batch_instance-with_heads.csv
