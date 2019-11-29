#/bin/bash

dataPath="/mnt/shared/Datasets/bobowang/For_this_Experiment"
dataPath_2017=$dataPath"/trace_201708"
dataPath_2018=$dataPath"/alibaba_clusterdata_v2018"

randomNumFold="$dataPath_2017/randomNumFold"
randomNumFile="${randomNumFold}/set_"

mkdir -p $randomNumFold

maxNum=1907064

times=10

currentIndex=-1
i=0
while [ $i -le $times ]
do
	i=`expr $i + 1`

	currentIndex=-1
	ranArray[1]=-2
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
