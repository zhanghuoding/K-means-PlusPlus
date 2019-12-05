	----------------------------------------------------------------
	#  author   :Bobo Wang                                         #
	#  time     :2019-08-09                                        #
	#  modify   :2019-12-04                                        #
	#  site     :Yunnan University                                 #
	#  e-mail   :wangbobochn@gmail.com                             #
	----------------------------------------------------------------

Welcom!

	1. This program is implemented in C programming language and tested on Linux platform.

	2. This program implements k-means++ clustering operation for any colnums in *.csv file, but
	   each execution can only target at one column, and cannot cluster two or more columns at 
	   the same time.

	3. After downloading the necessary files (include "k_meanspp.c","my_c_string_deal_tools.h" and
	   "makefile"), run the "$make" command in the terminal to compile the executable file named 
	   "k_meanspp".

	4. Program oprtation requires three parameters, they are the path of data,column number to 
	   perform clustering operation and cluster number respectively.
	   
	5. For this program, input file must be the format of CSV, and the file cannot contain table
	   header, that is to say, all lines of the file content must be all digital data, can not
	   contain words, letters, Chinese characters and every non-numeric characters other than
	   "," and ".".

	6. If there are any problems with the program, you can use the GDB tool to debug it directly.

	7. The program outputs three files, one is the log file "exe-cluster-log.log"; one is the data
	   file after the clustering with CSV format, named "clustered_column_*_based_"+ original
	   file name,the "*" represents the column number of this operation, and a new column will 
	   be added in the copy file of original file and the new column located after grouped column,
	   and this column indicates the clustering number of this sample. The last file 
	   "cluster_center_column_*_based-"+ original file name, the "*" in the file name also represents 
	   the column number which the poperation based on, the file content recoeds the clustering 
	   number after the clustering operation, the cluster center value and the number of samples 
	   classified into this clustering.

	8. You can execute the program as follows:
	   $./k_meanspp  ./exampleData.csv 2 15
	   This means that the second column of the file "example.csv" will be grouped into 15 
	   categories.
	   In this README.md file, all use format CSV file viewer named csvlook and the file processing
	   tool named head under Linux to view the file contents, like the following:
	   $head ./clustered_column_1_based_exampleData.csv | csvlook

	   Forexample, original data file "exampleData.csv" like the following:
	   
	   | 30 | 0.87 | 0.04 |               0.008… |               0.007… |
	   | 30 | 1.00 | 0.04 |               0.008… |               0.008… |
	   | 29 | 0.86 | 0.03 |               0.009… |               0.008… |
	   | 30 | 0.93 | 0.04 |               0.010… |               0.009… |
	   | 30 | 0.80 | 0.05 |               0.050… |               0.049… |
	   | 29 | 0.92 | 0.03 |               0.007… |               0.007… |
	   | 30 | 0.75 | 0.03 |               0.006… |               0.006… |
	   | 31 | 0.89 | 0.04 |               0.006… |               0.006… |
	   | 30 | 1.02 | 0.05 |               0.031… |               0.030… |
	   
	   Executing command : $./k_meanspp  ./exampleData.csv 1 20
	   We got a new file "clustered_column_1_based_exampleData.csv" like the following:
	   The second column is the cluster class number of the first column data,with a total 
	   of 20 classes.
	   
	   | 30 | 2 | 0.87 | 0.04 |               0.008… |               0.007… |
	   | 30 | 2 | 1.00 | 0.04 |               0.008… |               0.008… |
	   | 29 | 2 | 0.86 | 0.03 |               0.009… |               0.008… |
	   | 30 | 2 | 0.93 | 0.04 |               0.010… |               0.009… |
	   | 30 | 2 | 0.80 | 0.05 |               0.050… |               0.049… |
	   | 29 | 2 | 0.92 | 0.03 |               0.007… |               0.007… |
	   | 30 | 2 | 0.75 | 0.03 |               0.006… |               0.006… |
	   | 31 | 2 | 0.89 | 0.04 |               0.006… |               0.006… |
	   | 30 | 2 | 1.02 | 0.05 |               0.031… |               0.030… |
	   
	   Executing command : $./k_meanspp  ./exampleData.csv 3 20
	   We got a new file "clustered_column_3_based_exampleData.csv" like the following:
	   The forth column is the cluster class number of the third column data,with a total 
	   of 20 classes.
	   
	   | 30 | 0.87 | 0.04 | True |               0.008… |               0.007… |
	   | 30 | 1.00 | 0.04 | True |               0.008… |               0.008… |
	   | 29 | 0.86 | 0.03 | True |               0.009… |               0.008… |
	   | 30 | 0.93 | 0.04 | True |               0.010… |               0.009… |
	   | 30 | 0.80 | 0.05 | True |               0.050… |               0.049… |
	   | 29 | 0.92 | 0.03 | True |               0.007… |               0.007… |
	   | 30 | 0.75 | 0.03 | True |               0.006… |               0.006… |
	   | 31 | 0.89 | 0.04 | True |               0.006… |               0.006… |
	   | 30 | 1.02 | 0.05 | True |               0.031… |               0.030… |
	   
	   Executing command : $./k_meanspp  ./exampleData.csv 5 20
	   We got a new file "clustered_column_5_based_exampleData.csv" like the following:
	   The sixth column is the cluster class number of the fifth column data,with a total 
	   of 20 classes.
	   
	   | 30 | 0.87 | 0.04 |               0.008… |               0.007… |  3 |
	   | 30 | 1.00 | 0.04 |               0.008… |               0.008… |  4 |
	   | 29 | 0.86 | 0.03 |               0.009… |               0.008… |  4 |
	   | 30 | 0.93 | 0.04 |               0.010… |               0.009… |  4 |
	   | 30 | 0.80 | 0.05 |               0.050… |               0.049… | 15 |
	   | 29 | 0.92 | 0.03 |               0.007… |               0.007… |  3 |
	   | 30 | 0.75 | 0.03 |               0.006… |               0.006… |  3 |
	   | 31 | 0.89 | 0.04 |               0.006… |               0.006… |  3 |
	   | 30 | 1.02 | 0.05 |               0.031… |               0.030… | 11 |
	   
	   Executing command : $./k_meanspp  ./exampleData.csv 3 20
	   so we also got a new file "cluster_center_column_3_based_exampleData.csv" like the following:
	   This file records the number of the 20 clusters,the cluster center value,and the 
	   number os samples belonging to the cluster.
	   
	   Cluster No. | Center Value | Sample Number |
	   | ----------- | ------------ | ------------- |
	   |           1 |       0.066… |       494,625 |
	   |           2 |       0.175… |       246,765 |
	   |           3 |       0.292… |       171,009 |
	   |           4 |       0.432… |       118,115 |
	   |           5 |       0.570… |       118,858 |
	   |           6 |       0.676… |       129,510 |
	   |           7 |       0.778… |       176,826 |
	   |           8 |       0.863… |       146,889 |
	   |           9 |       0.941… |        78,933 |

