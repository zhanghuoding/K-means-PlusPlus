#!/bin/bash

#This is a awk script.

cat ./container_event.csv | awk -F , 'BEGIN{
	for(i=1;i<=4;)
	{
		getline < "./container_event.csv";
		if($1)
		{
			temp_array[$4]=NR;
			i++
		}
	}
	close("./containter.csv");
}
{
	if($1 && ($4 in temp_array))
	{
		print $0;
	}
}' | csvlook
