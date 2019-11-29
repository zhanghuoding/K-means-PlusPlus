#/bin/bash

dataPath="/mnt/shared/Datasets/bobowang/For_this_Experiment"
dataPath_2017=$dataPath"/trace_201708"
dataPath_2018=$dataPath"/alibaba_clusterdata_v2018"

randomNumSubFold_1="$dataPath_2018/subDataset_"
randomNumSubFold_2="/randomNumFold"

totalDataNum=1201019097

maxNum=1907064

subNum=10

times=10

currentIndex=-1
m=0
i=0
while [ $m -le $subNum ]
do
#first extract sub dataset, it have 10 folds.
	m=`expr $m + 1`
	randomNumFold="${randomNumSubFold_1}${m}${randomNumSubFold_2}"
	mkdir -p $randomNumFold
	randomNumFile="${randomNumFold}/set_"
	randomNumFileInt="${randomNumFold}/intDataset"		#intDataset saved the sub-dataset 1907064 in 1201019097.

	currentIndex=-1
	ranArray[0]=-2
	j=1
	temp=-1
	echo "" > $randomNumFileInt
	while [ $j -le $maxNum ]
	do
		temp=` head -200 /dev/urandom | cksum | cut -d " " -f1 `
		temp=`expr $temp % $totalDataNum`

		for n in ${ranArray[*]}
		do
			if [ $temp == $n ]
			then
				temp=-1
				break
			fi
		done

		if [[ $temp == -1 ]]
		then
			continue
		else
			currentIndex=`expr $currentIndex + 1`
			ranArray[$currentIndex]=$temp
			echo $temp >> $randomNumFileInt
			temp=-1
			j=`expr $j + 1`
		fi
	done
	unset ranArray

#Then we will split sub-dataset to 10 set.

	while [ $i -le $times ]
	do
		i=`expr $i + 1`
	
		currentIndex=-1
		ranArray[0]=-2
		j=1
		temp=-1
		echo "" > $randomNumFile$i
	
		while [ $j -le $maxNum ]
		do
			temp=` head -200 /dev/urandom | cksum | cut -d " " -f1 `
			temp=`expr $temp % $maxNum`
	
			for n in ${ranArray[*]}
			do
				if [ $temp == $n ]
				then
					temp=-1
					break
				fi
			done
	
			if [[ $temp == -1 ]]
			then
				continue
			else
				currentIndex=`expr $currentIndex + 1`
				ranArray[$currentIndex]=$temp
				echo $temp >> $randomNumFile$i
				temp=-1
				j=`expr $j + 1`
			fi
		done
		unset ranArray
	done
done
