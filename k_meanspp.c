//################################################################
//#  author   :Bobo Wang                                         #
//#  time     :2019-08-09                                        #
//#  modify   :2019-08-14                                        #
//#  site     :Yunnan University                                 #
//#  e-mail   :wangbobochn@gmail.com                             #
//################################################################


#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include "my_c_string_deal_tools.h"

#define BUFFER_SIZE 1024
#define ARRAY_SIZE_ADDED 200
#define ELEMENT_LENGTH 48
#define GENERAL_SIZE 256
#define GENERAL_LONG_SIZE 512

typedef struct dataNode
{
	double value;//each sample's value.
	long int number;//The number of occurrences of this value.
	double minDis;//Distance from the nearest cluster center.
	double selectedProbability;//The probability of being selecting of this sample.
	int belongNumber;//This value belongs to the category.
}DataNode,*DataNodePtr;

typedef struct clusterCenterNode
{
	double value;//the value of current cluster center.
	int number;//the number of sample which belong to current cluster center.
}ClusterCenterNode, *ClusterCenterNodePtr;

char * logFilePath=".";//The path of log file.
char * logFileName="/exe-cluster-log.log";//The file name.
char logFile[GENERAL_SIZE]={'\0'};//path + file name of log file.
char logBuffer[GENERAL_SIZE]={'\0'};
char dataClusteredOutputFile[GENERAL_SIZE] = {'\0'};//The result of data clustering.
char clusterCenterOutputFile[GENERAL_SIZE] = {'\0'};//The result of cluster centers.
char dataRowBuffer[BUFFER_SIZE]={'\0'};

//pthread_t cp_thread;//线程句柄
char *splitChar=",";

DataNodePtr data = NULL;//Creating an array to store data.
DataNode auxiliary;//An auxiliary variable.
int dataArraySize=ARRAY_SIZE_ADDED;//Record the size of the array "data".
int dataArraySizeCurrent=-1;//Record the size of array "data" now,and point the last element in the array.
ClusterCenterNodePtr clusterCenter = NULL;
double infinityDouble = 0;
int dataFileColumnNumber = 0;
char *dataFilePath = NULL;
int columnNumberClusterOn = 0;
int clusterNumber=0;//Class number.
double totalSampleNumber = 0;//the line number of the data file.


void init();
char *getLocalTime();
void outputLog(char *);
void *expansionArray(void *,int);
int isExit(double,DataNodePtr);
double *k_means_pp( DataNodePtr, int, int);
double *k_means( DataNodePtr, int, ClusterCenterNodePtr, int);
void clusterSort( DataNodePtr, int, ClusterCenterNodePtr, int );
void printClustersArray( ClusterCenterNodePtr, int, char* );
void *myAllocateFun( void**, int, int, char* );

int main( int argc, char *argv[] )
{
	/*
	 *The first parameter is the name of this propram.
	 *The second parameter is the name and path of the data file.
	 *The third parameter indicate which column we should change.
	 *The forth parameter indicate how many classes we need.
	 */
        if( argc != 4 )
         {
		perror("Too few parameters, you should set 4 parameters.");

/*
		printf("\n");
	 	printf("The first parameter is the name of this propram.\n");
	 	printf("The second parameter is the name and path of the data file.\n");
		printf("The third parameter indicate which column we should change.\n");
	 	printf("The forth parameter indicate how many classes we need.\n");
*/
		printf("Usage: k_meanspp fileName columnNumber clusterAmount\n");

		exit(1);
        }

	dataFilePath = argv[1];
	columnNumberClusterOn = atoi(argv[2]);
	clusterNumber = atoi(argv[3]);

	infinityDouble = 1.0 / 0.0;
        //initialize
        init();
	//Start classify data.
	k_means_pp( data, dataArraySizeCurrent, clusterNumber );

	char *writeBuffer[BUFFER_SIZE];
	sprintf( dataClusteredOutputFile, "./clustered_column_%d_based-%s", columnNumberClusterOn, dataFilePath + lastIndexOf( dataFilePath, "/" ) + 1 );
	sprintf( clusterCenterOutputFile, "./cluster_center_column_%d-based_%s", columnNumberClusterOn, dataFilePath + lastIndexOf( dataFilePath, "/" ) + 1 );
	outputLog("Writing output file, this may take some time, please be patient...");

	FILE *dataClusteredOutput=NULL;
	remove(dataClusteredOutputFile);
	creat(dataClusteredOutputFile,0755);
	if(( dataClusteredOutput = fopen( dataClusteredOutputFile, "a+"))==NULL)
	{
		perror("Open dataClusteredOutputFile file");
		exit(1);
	}
        //Read original data file.
	int col= columnNumberClusterOn ;
	int splitIndex=-1;
	double value=0;
	int currentSampleNumber = 0;
	double progress = 0;
	char valueChar[ELEMENT_LENGTH];
	int valueIndexIfThisValueExits=-1;
	int preApp = -1;
	int currentApp = -1;
	int splitStrLen = strlen( splitChar );
	int bufferLen = 0;
	int sstl = 0;
        FILE * fdata=NULL;
        if((fdata=fopen(dataFilePath,"r"))==NULL)
        {
            perror("Open data file");
            exit(1);
        }
	printf("\033[?25l");
        while(!feof(fdata))
        {
		if( ! ( currentSampleNumber % 200 ) )
			printf("\r\033[kWriting output file, this may take some time, \033[;34;1m%5.2f%%\033[0m complete currently, please be patient...   ", 100 * ( currentSampleNumber / totalSampleNumber ) );
		currentSampleNumber += 1;
		splitIndex=-1;
		value=0;
		valueIndexIfThisValueExits=-1;
		sstl = 0;

            	memset(dataRowBuffer,0, BUFFER_SIZE );
            	fgets(dataRowBuffer, BUFFER_SIZE - 1,fdata);
		rtrimnr(dataRowBuffer);//delete the '\n' or '\r' at the tail of dataRowBuffer.
		bufferLen = strlen( dataRowBuffer );
	    	if( bufferLen == 0 )
			continue;//If this line is empty,continue next loop.

	    	memset(valueChar,0,sizeof(char) * ELEMENT_LENGTH);
		preApp = nthAppearingIndexOf( dataRowBuffer, splitChar, col - 1 );
		currentApp = nthAppearingIndexOf( dataRowBuffer, splitChar, col );
		if( preApp != currentApp && preApp != -1 && currentApp != -1 )
		//This is not the first column or the last column.
			strncpy( valueChar, dataRowBuffer + preApp + splitStrLen, currentApp - preApp - splitStrLen );
		else if( preApp == -1 && currentApp > 0 )
		//This is the first column.
			strncpy( valueChar, dataRowBuffer, currentApp );
		else if( preApp != -1 && currentApp == -1)
		//This is the last column.
			strncpy( valueChar, dataRowBuffer + preApp + splitStrLen, bufferLen - preApp - splitStrLen );
		else if( preApp == currentApp && preApp == -1 && col == 1 )
		//This file have only one column.
			strncpy( valueChar, dataRowBuffer, bufferLen );

		value=atof(valueChar);
		sprintf( writeBuffer, "%s%d",splitChar, data[isExit( value, data )].belongNumber );
		if( currentApp != -1 )
			insertAtPtr( dataRowBuffer, BUFFER_SIZE, dataRowBuffer + currentApp, writeBuffer );
		else
			insertAtPtr( dataRowBuffer, BUFFER_SIZE, dataRowBuffer + bufferLen, writeBuffer );
		sstl = strlen( dataRowBuffer );
		dataRowBuffer[sstl] = '\n';
		dataRowBuffer[sstl + 1] = '\0';
		fputs(dataRowBuffer,dataClusteredOutput);
	}
	fclose(dataClusteredOutput);
	printf("\033[?25h\n");


	FILE *clusterCenterOutput=NULL;
	remove(clusterCenterOutputFile);
	creat(clusterCenterOutputFile,0755);
	if(( clusterCenterOutput = fopen( clusterCenterOutputFile, "a+"))==NULL)
	{
		perror("Open clusterCenterOutputFile file");
		exit(1);
	}
	sprintf( writeBuffer, "Cluster No.,Center Value,Sample Number\n" );
	fputs(writeBuffer,clusterCenterOutput);
	int i = 0;
	for( i = 1; i <= clusterNumber; i++ )
	{
		sprintf( writeBuffer, "%d,%f,%d\n", i, clusterCenter[i].value, clusterCenter[i].number );
		fputs(writeBuffer,clusterCenterOutput);
	}
	fclose(clusterCenterOutput);

	return 0;
}

void init()
{//Read and statistic data file.
        //Initialize the path of log file.
        char *lp=logFile;
        strncpy(logFile,logFilePath,strlen(logFilePath));
        lp+=strlen(logFilePath);
        strncpy(lp,logFileName,strlen(logFileName));
	remove(logFile);
	creat(logFile,0755);

	//initialize auxiliary struct.
	auxiliary.value = 0;
	auxiliary.number = 0;
	auxiliary.minDis = infinityDouble;
	auxiliary.selectedProbability = 0;
	auxiliary.belongNumber = 0;

	data = ( DataNodePtr )myAllocateFun( (void**)&data, dataArraySize, sizeof( DataNode ), "data array" );
	int ss = dataArraySizeCurrent;
	while( ++ss < dataArraySize )
		data[ss] = auxiliary;

	int col= columnNumberClusterOn ;
	int splitIndex=-1;
	double value=0;
	char valueChar[ELEMENT_LENGTH];
	int valueIndexIfThisValueExits=-1;

	int preApp = -1;
	int currentApp = -1;
	int splitStrLen = strlen( splitChar );
	int bufferLen = 0;
	
	if( clusterNumber < 1 )
	{
		outputLog("Sorry, please specify an appropriate cluster number (cluster number must more than 0)!");
		printf("Sorry, please specify an appropriate cluster number (cluster number must more than 0)!\n");

		exit(1);
	}

        //Read data file.
        FILE * fdata=NULL;
        if((fdata=fopen(dataFilePath,"r"))==NULL)
        {
            perror("Open data file");
            exit(1);
        }
	int tempIndex = -1;
	char *tempPtr = NULL;
	while( !feof(fdata) )
	{
            	memset(dataRowBuffer,0,BUFFER_SIZE);
            	fgets(dataRowBuffer,BUFFER_SIZE - 1,fdata);
		bufferLen = strlen( dataRowBuffer );
	    	if( bufferLen == 0 )
			continue;//If this line is empty,continue next loop.
		tempIndex = -1;
		tempPtr = dataRowBuffer;
		while( ( tempIndex = indexOf( tempPtr, splitChar ) ) != -1 )
		{
			dataFileColumnNumber += 1;
			tempPtr = tempPtr + tempIndex + splitStrLen;
		}
		tempIndex = -1;
		tempPtr = NULL;
		dataFileColumnNumber += 1;
		break;
	}
	rewind(fdata);
	if( !dataFileColumnNumber )
	{
		outputLog("Sorry, the file you specified contains 0 columns!");
		printf("Sorry, the file you specified contains 0 columns!\n");
		fclose(fdata);
		exit(1);
	}

	if( columnNumberClusterOn < 1 || columnNumberClusterOn > dataFileColumnNumber )
	{
		outputLog("Please input a correct column number to operation!");
		printf("Please input a correct column number to operation!\n");
		fclose(fdata);
		exit(1);
	}

        while(!feof(fdata))
        {
		splitIndex=-1;
		value=0;
		valueIndexIfThisValueExits=-1;
		totalSampleNumber += 1;

            	memset(dataRowBuffer,0, BUFFER_SIZE );
            	fgets(dataRowBuffer, BUFFER_SIZE - 1,fdata);
		bufferLen = strlen( dataRowBuffer );
	    	if( bufferLen == 0 )
			continue;//If this line is empty,continue next loop.

	    	memset(valueChar,0,sizeof(char) * ELEMENT_LENGTH);
		preApp = nthAppearingIndexOf( dataRowBuffer, splitChar, col - 1 );
		currentApp = nthAppearingIndexOf( dataRowBuffer, splitChar, col );
		if( preApp != currentApp && preApp != -1 && currentApp != -1 )
		//This is not the first column or the last column.
			strncpy( valueChar, dataRowBuffer + preApp + splitStrLen, currentApp - preApp - splitStrLen );
		else if( preApp == -1 && currentApp > 0 )
		//This is the first column.
			strncpy( valueChar, dataRowBuffer, currentApp );
		else if( preApp != -1 && currentApp == -1)
		//This is or the last column.
			strncpy( valueChar, dataRowBuffer + preApp + splitStrLen, bufferLen - preApp - splitStrLen );
		else if( preApp == currentApp && preApp == -1 && col == 1 )
		//This file have only one column.
			strncpy( valueChar, dataRowBuffer, bufferLen );

		value=atof(valueChar);
		if( ( valueIndexIfThisValueExits = isExit( value, data ) ) == -1 )
		{//This value dose not exit.
			if( dataArraySizeCurrent + 1 >= dataArraySize )
				data = ( DataNodePtr )myAllocateFun( (void**)&data, dataArraySize += ARRAY_SIZE_ADDED, sizeof( DataNode ), "data array" );
			data[++dataArraySizeCurrent].value = value;
			//The distance domain is set to infinity to facilitate the calculation of the 
			//minimum distance from the cluster center when determining the cluster center.
			data[dataArraySizeCurrent].number = 1;
		}else
		{//This value exits.
			data[valueIndexIfThisValueExits].number += 1;
		}
	}

        fclose(fdata);

	if( dataArraySizeCurrent < 0 )
	{
		outputLog("Please input a non-empty file!");
		printf("Please input a non-empty file!\n");
		exit(1);
	}

	if( dataArraySizeCurrent < clusterNumber )
	{
		outputLog("Sorry, please specify an appropriate cluster number (cluster number should not be less than the number of samples)!");
		printf("Sorry, please specify an appropriate cluster number (cluster number should not be less than the number of samples)!\n");

		exit(1);
	}

	outputLog("Finished read data file.");

}

char *getLocalTime()
{
	time_t timep;
	time(&timep);
	return ctime( &timep );
}


void outputLog(char *buffer)
{
	char writeBuffer[GENERAL_LONG_SIZE] = {'\0'};
	sprintf(writeBuffer,"%s\t%s\n",buffer,getLocalTime());

	//组织创建日志路径的语句
	char logPathBuffer[GENERAL_SIZE]={'\0'};
	sprintf(logPathBuffer,"mkdir -p %s",logFilePath);

	FILE *log=NULL;
	if( (access( logFile, 0 )) == -1 )
	{//日志文件不存在则新建
        	system(logPathBuffer);//创建日志文件路径
		creat(logFile,0755);
	}else
	{//日志文件没有读写权限时删除并新建
		if( (access( logFile, 6 )) == -1 )
		{
			remove(logFile);
            		creat(logFile,0755);
        	}
	}

	if((log=fopen(logFile,"a+"))==NULL)
	{
		perror("Open log file");
		exit(1);
	}

	fputs(writeBuffer,log);

	fclose(log);
}

int isExit(double val,DataNodePtr array)
{//If value exits,return index in the array; else,return -1.
	int no=-1;
	int i=0;
	if(dataArraySizeCurrent < 0 )
		return no;//If array is empty,return -1 directly.
	for(i=0;i <= dataArraySizeCurrent;i++)
	{
		if( fabs( data[i].value - val ) <= 1e-6 )//If value exits,return index.
		{
			no = i;
			break;
		}
	}
	if( i > dataArraySizeCurrent )
		no = -1;
	return no;
}
double *k_means_pp( DataNodePtr dataArray, int dataArrayLength, int cluNum )
{//This function will classify all of the data value into "cluNum" classes.
 //Using K-means++ algorithm.
 //This function return a array of cluster centers.
 
	outputLog("Entering the function named k_means_pp( starting algorithm k-means++ ...).");

	cluNum += 1;

	int clusterIndexCurrent=0;//This pointer will point the last element.
	clusterCenter = ( ClusterCenterNodePtr )myAllocateFun( (void**)&clusterCenter, cluNum, sizeof( ClusterCenterNode ), "cluster center array" );

	int j = 0;
	for( j=0; j < cluNum; j++ )
	{
		clusterCenter[j].value = 0;
		clusterCenter[j].number = 0;
	}

	//Select the first cluster center.
	srand( (unsigned)time(NULL) );
	double r = rand();
	int p = (int)r % ( dataArrayLength + 1 );
	clusterCenter[++clusterIndexCurrent].value = dataArray[p].value;
	dataArray[p].belongNumber = clusterIndexCurrent;

	//Then select the remaining cluster centers.
	//Firstly comput the mindis between each sample and all clusters centers.
	outputLog("Selecting cluster center for first time(first time means classify for first time)...");

	int ind = 0;
	double tempDis=0;
	int dataInd = 0;
	double sumOfSquares = 0;
	double *proSum = NULL;
	proSum = ( double* )myAllocateFun( (void**)&proSum, dataArrayLength + 1, sizeof( double ), "probability sum array" );
	double proSumTemp = 0;
	while( clusterIndexCurrent < cluNum - 1 )
	{
		ind = 1;
		tempDis = 0;
		dataInd = 0;
		sumOfSquares = 0;
		proSumTemp = 0;

		//Computing the minimum distance between each sample and all current clustering centers.
		for(ind = 1; ind <= clusterIndexCurrent; ind++)
		{
			for( dataInd = 0; dataInd <= dataArrayLength; dataInd++ )
			{
				tempDis = fabs( clusterCenter[ind].value - dataArray[dataInd].value );
				dataArray[dataInd].minDis = tempDis < dataArray[dataInd].minDis ? tempDis : dataArray[dataInd].minDis;
			}
		}
		//Then comput the probability of each sample for being selected as the next cluster center.
		for( dataInd = 0; dataInd <= dataArrayLength; dataInd++ )
			sumOfSquares += pow( dataArray[dataInd].minDis, 2 );
		r = rand();
		while( r > 1 )
			r /= 7;
		for( dataInd = 0; dataInd <= dataArrayLength; dataInd++ )
		{
			dataArray[dataInd].selectedProbability = pow( dataArray[dataInd].minDis, 2 ) / sumOfSquares;
			if( ( r - proSumTemp ) > 1e-6 && ( ( r - ( proSumTemp += dataArray[dataInd].selectedProbability )) < 0 ) || ( fabs( r - proSumTemp) <= 1e-6 ) )
			{
				clusterCenter[++clusterIndexCurrent].value = dataArray[dataInd].value;
				dataArray[dataInd].belongNumber = clusterIndexCurrent;
				break;
			}
			proSum[dataInd] = proSumTemp;
		}
	}

	clusterSort( dataArray, dataArrayLength, clusterCenter, cluNum );

	char *title = "Cluster ( first time )";
	printClustersArray( clusterCenter, cluNum, title );

	outputLog("All cluster centers for the first round have been identified.");

	free( proSum );
	proSum = NULL;

	return k_means( dataArray, dataArrayLength, clusterCenter, cluNum );

}
double *k_means( DataNodePtr dataArray, int dataArrayLength, ClusterCenterNodePtr clusterCenter, int cluNum )
{//This function will cluste sample from array "dataArray" into "cluNum" clusters.
 //The parameter "dataArrayLength" indicated data array's last element index,and the length of dataArray is "dataArrayLength + 1".
 //The parameter "clusterCenter" is a array which contain all cluster's center,and parameter "cluNum" indicated the number of clusters.
 //This function return a array of cluster centers.
	outputLog("Starting k_means algorithm...");

	int tempIndex = 0;
	int ind = 1;
	double tempDis=0;
	int dataInd = 0;
	int clusterIndexCurrent=-1;//This pointer will point the last element.
	double *cluCenSum = NULL;
	int *cluCenNum = NULL;
	cluCenSum = ( double* )myAllocateFun( (void**)&cluCenSum, cluNum, sizeof( double ), "cluster center sum array" );
	for( tempIndex=0; tempIndex < cluNum; tempIndex++ )
		cluCenSum[tempIndex] = 0;

	cluCenNum = ( int* )myAllocateFun( (void**)&cluCenNum, cluNum, sizeof( int ), "cluster center number array" );
	for( tempIndex=0; tempIndex < cluNum; tempIndex++ )
		cluCenNum[tempIndex] = 0;

	short flag = 0;
	int loopTimeCurrent = 0;
	double currentMeansForEachClusterCenter = 0;
	do
	{
		//Display the prompt message.
		printf("\r\033[kExecuting the %dth loop of K-means++ algorithm, please wait...", ++loopTimeCurrent);
		
		ind = 1;
		tempDis = 0;
		dataInd = 0;
		flag = 0;

		for( dataInd = 0; dataInd <= dataArrayLength; dataInd++ )
		{
			dataArray[dataInd].minDis = infinityDouble;
		}
		
		//Computing the minimum distance between each sample and all current clustering centers.
		for(ind = 1; ind < cluNum; ind++)
		{
			for( dataInd = 0; dataInd <= dataArrayLength; dataInd++ )
			{
				tempDis = fabs( clusterCenter[ind].value - dataArray[dataInd].value );
				if( tempDis < dataArray[dataInd].minDis )
				{
					dataArray[dataInd].minDis = tempDis;
					dataArray[dataInd].belongNumber = ind;
				}
			}
		}

		//Recalculate the center of mass of the cluster.
		for( tempIndex=0; tempIndex < cluNum; tempIndex++ )
			cluCenSum[tempIndex] = 0;
		for( tempIndex=0; tempIndex < cluNum; tempIndex++ )
			cluCenNum[tempIndex] = 0;
		for( dataInd = 0; dataInd <= dataArrayLength; dataInd++ )
		{
			cluCenSum[dataArray[dataInd].belongNumber] += dataArray[dataInd].value * dataArray[dataInd].number;
			cluCenNum[dataArray[dataInd].belongNumber] += dataArray[dataInd].number;
		}
		for( ind = 1; ind < cluNum; ind++ )
		{
			currentMeansForEachClusterCenter = 0;
			currentMeansForEachClusterCenter = cluCenSum[ind] / cluCenNum[ind];
			clusterCenter[ind].number = cluCenNum[ind];
			if( ! fabs( clusterCenter[ind].value - currentMeansForEachClusterCenter ) <= 1e-6 )
			{
				clusterCenter[ind].value = currentMeansForEachClusterCenter;
				flag = 1;
			}
		}
	}while( flag );
	printf("\n");

	clusterSort( dataArray, dataArrayLength, clusterCenter, cluNum );

	char *title = " Cluster ( finally )  ";
	printClustersArray( clusterCenter, cluNum, title );

	outputLog("The algorithm of k_means has been executed successfully!");

	return clusterCenter;

}
void printClustersArray( ClusterCenterNodePtr clustrerCenter, int cluNum, char *title )
{
	int ga=0;
	printf("\n\u250f\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2513\n");
	printf("\u2503                 %s                 \u2503\n",title);
	printf("\u2520\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2528\n");
	printf("\u2503 Cluster No.\t\u2503\tCenter value\t\u2503 Sampale nember \u2503\n");
	printf("\u2520\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u254b\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u254b\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2528\n");
	while( ++ga < cluNum )
		printf("\u2503 \t%d\t\u2503\t%f\t\u2503\t%d\t \u2503\n", ga, clusterCenter[ga].value, clusterCenter[ga].number );
	printf("\u2517\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u251b\n");
}

void clusterSort( DataNodePtr dataArray, int dataArrayLength, ClusterCenterNodePtr clusterCenter, int cluNum )
{//Sorting the clusters in ascending order by cluster center value.
 //Using selection sort algorithm.

	int te = 0;
	int re = 0;
	int max = 0;
	//ClusterCenterNode tempNode;
	//clusterCenter[0] = clusterCenter[1];
	for( te = cluNum - 1; te > 1 ; te-- )
	{
		max = 1;
		for( re = te; re > 1; re-- )
			if( clusterCenter[ max ].value < clusterCenter[ re ].value )
				max = re;
		//exchange
		if( te != max )
		{
			clusterCenter[ 0 ] = clusterCenter[ te ];
			clusterCenter[ te ] = clusterCenter[ max ];
			clusterCenter[ max ] = clusterCenter[ 0 ];
		}

		for( re = 0; re <= dataArrayLength; re++ )
		{
			if( dataArray[ re ].belongNumber == te )
				dataArray[ re ].belongNumber == max;
			if( dataArray[ re ].belongNumber == max )
				dataArray[ re ].belongNumber == te;
		}
	}
	clusterCenter[ 0 ].value = 0;
	clusterCenter[ 0 ].number = 0;
}
void *myAllocateFun( void** address, int number, int elementSize, char* tips)
{//This function applies for a space of "number" data units and all of data units is "elementSize" 
 //bytes and return the address fo the new space.
 //If "*address" equals "NULL", function "calloc" will calling to allocate the new space.
 //If "number" or "elementSize" equals 0, the original storage space will be release, and the original
 //pointer also will be set to "NULL",and this function will return null value.
 	if( !address )
		return NULL;

	if( number * elementSize == 0 )
	{
		if( *address )
			free( *address );
		*address = NULL;
		return NULL;
	}

	int tem = 10;
	void *ptr = NULL;

	if ( ! *address )
	{
		tem = 10;
		while(tem--)
		{
			if( ! ( ptr = calloc( number, elementSize ) ) )
			{
				memset(logBuffer,0,sizeof(logBuffer));
				sprintf( logBuffer,"Allocate %s failed.", tips );
				outputLog(logBuffer);
		
			}else
			{
				memset(logBuffer,0,sizeof(logBuffer));
				sprintf( logBuffer,"Allocate %s successfully.", tips );
				outputLog(logBuffer);
				break;
			}
		}
		if( ! ptr )
		{
			perror("Allocate:");
			exit(1);
		}
		*address = ptr;
		return *address;
	}

	ptr = *address;
	tem = 10;	
	while(tem--)
	{
		if( ! ( ptr = realloc( ptr, number * elementSize ) ) )
		{
			memset(logBuffer,0,sizeof(logBuffer));
			sprintf( logBuffer,"Reallocate %s failed.", tips );
			outputLog(logBuffer);
	
		}else
		{
			memset(logBuffer,0,sizeof(logBuffer));
			sprintf( logBuffer,"Reallocate %s successfully.", tips );
			outputLog(logBuffer);
			break;
		}
	}
	if( ! ptr )
	{
		perror("Reallocate:");
		exit(1);
	}
	*address = ptr;
	return *address;
}


