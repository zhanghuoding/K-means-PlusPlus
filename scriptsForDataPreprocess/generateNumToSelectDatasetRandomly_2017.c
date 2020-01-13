//################################################################
//#  author   :Bobo Wang                                         #
//#  time     :2019-11-20                                        #
//#  modify   :2019-12-11                                        #
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
#include <unistd.h>

#define MAX_THREAD_NUM 200
#define MOVE_THRESHOLD 1000//When the array has MOVE_THRESHOLD blank,it will sort memory.
#define NULL_ARRAY -2
#define NULL_RANDOM -1
#define RETURN_1 1
#define RETURN_2 2
#define RETURN_0 0
#define RETURN_P1 -1
#define RETURN_P2 -2

#define BUFFER_SIZE 1024
#define ARRAY_SIZE_ADDED 200
#define ELEMENT_LENGTH 48
#define GENERAL_SIZE 256
#define GENERAL_LONG_SIZE 512
#define TOTALDATANUM 1201019097
#define MAXNUM 1907064
#define INCREMENTAL_DATANUM 190760
#define SUBNUM 10
#define TIMES 10


char *  dataPath="/mnt/shared/Datasets/bobowang/For_this_Experiment";//path of the dataset.
//char *  dataPath=".";//path of the dataset.

int thread_count=0;//线程计数器
pthread_t threads[MAX_THREAD_NUM];
int threads_head_index=0;
int threads_tail_index=0;
pthread_mutex_t arr_mutex;

char dataPath_2017[GENERAL_SIZE]={'\0'};
char dataPath_2018[GENERAL_SIZE]={'\0'};

void child_thread(char*);
void arrangeArray( unsigned long *, int, unsigned long );

int main( int argc, char *argv[] )
{
	pthread_mutex_init(&arr_mutex,NULL);

	sprintf( dataPath_2017, "%s/trace_201708", dataPath );
	sprintf( dataPath_2018, "%s/alibaba_clusterdata_v2018", dataPath );

	if( argc >= 2 )
	{
		int argv_i = 1;
		while( argv_i < argc )
		{
			if( pthread_create(threads + (thread_count++), NULL, (void *)child_thread, (void *)argv[argv_i++] ) )
			{
				printf("Create thread Failed!\n");
				return RETURN_1;
			}
		}
		argv_i = 0;
		while( argv_i < thread_count )
			pthread_join( threads[ argv_i++ ], NULL );
	}
	else
	{
		printf("Please input atleast onr parameter!\n");
		return RETURN_1;
	}
	return RETURN_0;
}

void child_thread(char * fn)
{
	char randomNumFold[GENERAL_SIZE] = {'\0'};
	char randomNumFile[GENERAL_SIZE] = {'\0'};
	char randomNumFileOutput[GENERAL_SIZE] = {'\0'};
	int fileNum = atoi( (char*)fn );

	char buffer[BUFFER_SIZE]={'\0'};

	unsigned long *ranArray = NULL;
	if ( ! ( ranArray = malloc( sizeof(unsigned long) * ( MAXNUM ) ) ) )
	{
		memset( buffer, 0, BUFFER_SIZE );
		sprintf( buffer, "Allocate memery in thread %d\n", fileNum );
		perror( buffer );
		return RETURN_2;
	}
	int ss = 0;
	for( ss = 0; ss < MAXNUM; ss++ )
		ranArray[ ss ] = ss;
	unsigned long arraySize = MAXNUM;

	/*
	 *This program need one or zero parameters.
	 */

	srand( (unsigned)time(NULL) );

	sprintf( randomNumFold, "%s/randomNumFold", dataPath_2017 );
	sprintf( randomNumFile, "%s/set_", randomNumFold );

	memset( buffer, 0, BUFFER_SIZE );
	sprintf( buffer, "mkdir -p %s", randomNumFold );
	pthread_mutex_lock(&arr_mutex);
	if ( system( buffer ) )
	{
		perror( buffer );
		pthread_mutex_unlock(&arr_mutex);
		return RETURN_1;
	}
	pthread_mutex_unlock(&arr_mutex);

	int i = 1;
	int j = 0;
	unsigned long ind = 0;
	unsigned long temp = NULL_RANDOM;

	FILE *output = NULL;

	i = fileNum;
	//while( i <= TIMES )
	while( i == fileNum )
	{
		//if( fileNum > 0 && fileNum <= TIMES )
		if( fileNum > 0 )
		{
			i = fileNum;
			memset( randomNumFileOutput, 0, GENERAL_SIZE);
			sprintf( randomNumFileOutput, "%s%d", randomNumFile, fileNum );
			if( access( randomNumFileOutput, F_OK ) || access( randomNumFileOutput, R_OK ) )
			{//if file does not exists or un-readable.
				fileNum = NULL_RANDOM;
				//#############################
				goto goon;
				//#############################
				continue;
			}
			FILE *fdata = NULL;
			if(( fdata = fopen( randomNumFileOutput, "r")) == NULL )
			{
				memset( buffer, 0, BUFFER_SIZE );
				sprintf( buffer, "Open file %s", randomNumFileOutput );
				perror( buffer );
				return RETURN_1;
			}
			char tempFile[GENERAL_SIZE]={'\0'};
			sprintf( tempFile, "%s.temp", randomNumFileOutput );
			FILE *tempData = NULL;
			remove( tempFile );
			creat( tempFile, 0755 );
			if(( tempData = fopen( tempFile, "a+" )) == NULL )
			{
				perror( "Open data file" );
				return RETURN_1;
			}
			char readNum[ELEMENT_LENGTH]={'\0'};
			while( !feof(fdata) )
			{
				memset( readNum, 0, ELEMENT_LENGTH );
				fgets( readNum, ELEMENT_LENGTH - 1, fdata);
				if( strlen( readNum ) == 0 )
				{//If this lines is empty, break the loop.
					continue;
				}
				temp = atoi( readNum );
				for( ss = 0; ss < MAXNUM; ss++ )
				{
					if( ! ( ranArray[ ss ] ^ temp ) )
					{
						ranArray[ ss ] = NULL_ARRAY;
						fprintf( tempData, "%d\n", temp );
						j++;
					}
				}
				temp = NULL_RANDOM;
			}
			fclose( fdata );
			fclose( tempData );
			memset( buffer, 0, BUFFER_SIZE );
			sprintf( buffer, "mv %s %s", tempFile, randomNumFileOutput );
			pthread_mutex_lock(&arr_mutex);
			if ( system( buffer ) )
			{
				perror( buffer );
				pthread_mutex_unlock(&arr_mutex);
				return RETURN_1;
			}
			pthread_mutex_unlock(&arr_mutex);
			fileNum = NULL_RANDOM;
			if( j >= MOVE_THRESHOLD )
			{
				arrangeArray( ranArray, arraySize, NULL_ARRAY );
				arraySize -= j;
				j = 0;
			}
		}
		else
		{
			goon:
			j = 0;
			memset( randomNumFileOutput, 0, GENERAL_SIZE);
			sprintf( randomNumFileOutput, "%s%d", randomNumFile, i );
			remove( randomNumFileOutput );
			creat( randomNumFileOutput, 0755 );
		
			//memset( buffer, 0, BUFFER_SIZE );
			//sprintf( buffer, "echo \"\" > %s",randomNumFileOutput );
			//pthread_mutex_lock(&arr_mutex);
			//if ( system( buffer ) )
			//{
			//	perror( buffer );
			//	pthread_mutex_unlock(&arr_mutex);
			//	return RETURN_1;
			//}
			//pthread_mutex_unlock(&arr_mutex);
		
		}
		if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
		{
			perror( "Open data file" );
			return RETURN_1;
		}
		//setbuf( output, NULL );

		temp = NULL_RANDOM;
		//ranArray[0] = MAXNUM;
		arraySize -= j;
		j = 0;
		while( j < arraySize )
		{
			//temp = rand() / ( RAND_MAX + 0.5 ) * MAXNUM;
			temp = rand() % arraySize;
			//printf("RAND_MAX = %d, \tand temp = %d \n", RAND_MAX, temp );

			if( ranArray[ temp ] ^ NULL_ARRAY )
			{
				fprintf( output, "%d\n", ranArray[ temp ] );
				ranArray[ temp ] = NULL_ARRAY;
				j++;
				if( j >= MOVE_THRESHOLD )
				{
					arrangeArray( ranArray, arraySize, NULL_ARRAY );
					arraySize -= j;
					j = 0;
				}
			}
			temp = NULL_RANDOM;
		}
		fclose( output );
		//memset( ranArray, 0, sizeof(unsigned long) * ( MAXNUM ) );
		i += 1;
	}

	return RETURN_0;
}

void arrangeArray( unsigned long *array, int totalLength, unsigned long null_array )
{
	unsigned long ind = 0;
	int count = 0;
	for( ind = 0; ind < totalLength; ind++ )
	{
		if( ! ( array[ ind ] ^ null_array ) )
		{
			count++;
		}
		else if( count )
		{
			array[ ind - count ] = array[ ind ];
		}
	}
}
