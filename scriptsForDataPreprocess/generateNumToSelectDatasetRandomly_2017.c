//################################################################
//#  author   :Bobo Wang                                         #
//#  time     :2019-11-20                                        #
//#  modify   :2019-12-10                                        #
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

char dataPath_2017[GENERAL_SIZE]={'\0'};
char dataPath_2018[GENERAL_SIZE]={'\0'};

void child_thread(char*);

int main( int argc, char *argv[] )
{
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
				return 1;
			}
		}
		argv_i = 0;
		while( argv_i < thread_count )
			pthread_join( threads[ argv_i++ ], NULL );
	}
	else
	{
		printf("Please input atleast onr parameter!\n");
		return 1;
	}
	return 0;
}

void child_thread(char * fn)
{
	char randomNumFold[GENERAL_SIZE] = {'\0'};
	char randomNumFile[GENERAL_SIZE] = {'\0'};
	char randomNumFileOutput[GENERAL_SIZE] = {'\0'};
	int fileNum = atoi( (char*)fn );

	char buffer[BUFFER_SIZE]={'\0'};

	unsigned long *ranArray = NULL;
	if ( ! ( ranArray = malloc( sizeof(unsigned long) * ( MAXNUM + 1 ) ) ) )
	{
		memset( buffer, 0, BUFFER_SIZE );
		sprintf( buffer, "Allocate memery in thread %d\n", fileNum );
		perror( buffer );
		return 2;
	}

	/*
	 *This program need one or zero parameters.
	 */

	srand( (unsigned)time(NULL) );

	sprintf( randomNumFold, "%s/randomNumFold", dataPath_2017 );
	sprintf( randomNumFile, "%s/set_", randomNumFold );

	memset( buffer, 0, BUFFER_SIZE );
	sprintf( buffer, "mkdir -p %s", randomNumFold );
	if ( system( buffer ) )
	{
		perror( buffer );
		return 1;
	}

	int currentIndex= 1;
	int i = 1;
	int m = 1;
	int j = 1;
	unsigned long ind = 0;
	unsigned long temp = -1;

	FILE *output = NULL;

	i = fileNum;
	//while( i <= TIMES )
	while( i == fileNum )
	{
		if( fileNum > 0 && fileNum <= TIMES )
		{
			i = fileNum;
			memset( randomNumFileOutput, 0, GENERAL_SIZE);
			sprintf( randomNumFileOutput, "%s%d", randomNumFile, fileNum );
			if( access( randomNumFileOutput, F_OK ) || access( randomNumFileOutput, R_OK ) )
			{//if file does not exists or un-readable.
				fileNum = -1;
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
				return 1;
			}
			char readNum[ELEMENT_LENGTH]={'\0'};
			while( !feof(fdata) )
			{
				memset( readNum, 0, ELEMENT_LENGTH );
				fgets( readNum, ELEMENT_LENGTH - 1, fdata);
				if( strlen( readNum ) == 0 )
				{//If this lines is empty, break the loop.
					break;
				}
				ranArray[currentIndex++] = atoi( readNum );
			}
			fclose( fdata );
			fileNum = -1;
			if( currentIndex >= 5 )
			{
				currentIndex -= 2;
				j = currentIndex;
				remove( randomNumFileOutput );
				creat( randomNumFileOutput, 0755 );
				if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
				{
					perror( "Open data file" );
					return 1;
				}
				int t = 1;
				while( t < currentIndex )
				{
					fprintf( output, "%d\n", ranArray[t++] );
				}
				fclose( output );
			}
			else
			{
				currentIndex= 1;
				//#############################
				goto goon;
				//#############################
				continue;
			}
		}
		else
		{
			goon:
			currentIndex = 1;
			j = 1;
			memset( randomNumFileOutput, 0, GENERAL_SIZE);
			sprintf( randomNumFileOutput, "%s%d", randomNumFile, i );
			remove( randomNumFileOutput );
			creat( randomNumFileOutput, 0755 );
		
			//memset( buffer, 0, BUFFER_SIZE );
			//sprintf( buffer, "echo \"\" > %s",randomNumFileOutput );
			//if ( system( buffer ) )
			//{
			//	perror( buffer );
			//	return 1;
			//}
		
		}
		if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
		{
			perror( "Open data file" );
			return 1;
		}
		//setbuf( output, NULL );

		temp = -1;
		ranArray[0] = MAXNUM;
		while( j <= MAXNUM )
		{
			//temp = rand() / ( RAND_MAX + 0.5 ) * MAXNUM;
			temp = rand() % MAXNUM;
			//printf("RAND_MAX = %d, \tand temp = %d \n", RAND_MAX, temp );

			ind = 0;
			for( ind = 0; ind < currentIndex; ind++ )
			{
				if( ! ( temp ^  ranArray[ind] ) )
				{
					temp = -1;
					break;
				}
				
			}
			if( temp ^ -1 )
			{
				ranArray[currentIndex++] = temp;
				fprintf( output, "%d\n", temp );
				temp = -1;
				j++;
			}
		}
		fclose( output );
		memset( ranArray, 0, sizeof(unsigned long) * ( MAXNUM + 1 ) );
		i += 1;
	}

	return 0;
}
