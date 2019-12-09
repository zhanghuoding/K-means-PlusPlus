//################################################################
//#  author   :Bobo Wang                                         #
//#  time     :2019-11-20                                        #
//#  modify   :2019-12-09                                        #
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
#define SUB_TIMES 5


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
			if (pthread_create(threads + (thread_count++), NULL, (void *)child_thread, (void *)argv[argv_i++] ) != 0)
			{
				printf("Create thread Failed!\n");
				exit( 1 );
			}
			pthread_join( threads[ thread_count - 1 ], NULL );
		}
	}
	else
	{
		printf("Please input atleast onr parameter!\n");
		exit( 1 );
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
	
	unsigned long ranArray[TOTALDATANUM + 1]={0};

	/*
	 *This program do not need any parameters.
	 */

	srand( (unsigned)time(NULL) );

	/*
	 *firs  we will select 10 times and 190760 samples for each time.
	 * we also split the sub-dataset to 10 groups.
	 */

	int currentIndex= 1 ;
	int i = 1;
	int m = 1;
	int j = 1;
	unsigned long ind = 0;
	unsigned long temp = -1;
	
	FILE *output = NULL;

	/*
	 * If run this program with singal thread,this should be overwrited.
	m = 1;
	while( m <= SUBNUM )
	{
		sprintf( randomNumFold, "%s/randomNumFold/subDataset_%d", dataPath_2018, m );
		sprintf( randomNumFile, "%s/totalSamples_%d", randomNumFold, m );
		
		memset( buffer, 0, BUFFER_SIZE );
		sprintf( buffer, "mkdir -p %s", randomNumFold );
		if ( system( buffer ) )
		{
			perror( buffer );
			exit( 1 );
		}
	
		currentIndex= 1 ;
		i = 1;
		j = 1;
		ind = 0;
		temp = -1;

		memset( randomNumFileOutput, 0, GENERAL_SIZE);
		sprintf( randomNumFileOutput, "%s", randomNumFile );
		remove( randomNumFileOutput );
		creat( randomNumFileOutput, 0755 );
	
		if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
		{
			perror( "Open data file" );
			exit( 1 );
		}

		ranArray[0] = TOTALDATANUM;
		while( j <= INCREMENTAL_DATANUM )
		{
			//temp = rand() / ( RAND_MAX + 0.5 ) * MAXNUM;
			temp = rand() % TOTALDATANUM;
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
		memset( ranArray, 0, sizeof(unsigned long) * ( TOTALDATANUM + 1 ) );
		
		while( i <= TIMES )
		{
			if( m > 1 )
			{
				memset( buffer, 0, BUFFER_SIZE );
				sprintf( buffer, "cp %s/randomNumFold/subDataset_1/set_%d  %s/randomNumFold/subDataset_%d/", dataPath_2018, i, dataPath_2018, m );
				if( system( buffer ) )
				{
					perror( buffer );
					exit( 1 );
				}
				continue;
			}

			sprintf( randomNumFile, "%s/set_%d", randomNumFold, i );
		
			currentIndex = 1;
			j = 1;
			temp = -1;

			memset( randomNumFileOutput, 0, GENERAL_SIZE);
			sprintf( randomNumFileOutput, "%s", randomNumFile );
			remove( randomNumFileOutput );
			creat( randomNumFileOutput, 0755 );
		
			//memset( buffer, 0, BUFFER_SIZE );
			//sprintf( buffer, "echo \"\" > %s",randomNumFileOutput );
			//if ( system( buffer ) )
			//{
			//	perror( buffer );
			//	exit( 1 );
			//}
			if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
			{
				perror( "Open data file" );
				exit( 1 );
			}
			//setbuf( output, NULL );
	
			ranArray[0] = INCREMENTAL_DATANUM;
			while( j <= INCREMENTAL_DATANUM )
			{
				//temp = rand() / ( RAND_MAX + 0.5 ) * MAXNUM;
				temp = rand() % INCREMENTAL_DATANUM;
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
			memset( ranArray, 0, sizeof(unsigned long) * ( TOTALDATANUM + 1 ) );
			i += 1;
		}
		m++;
	}
	*/

	/*
	 *then we will split dataset 2018 to 10 sets on the 1201019097.
	 *this will be used to creat 10 models and the times for each process will be the contrast.
	 */
	
	sprintf( randomNumFold, "%s/randomNumFold/allSamples", dataPath_2018 );
	sprintf( randomNumFile, "%s/set_", randomNumFold );

	memset( buffer, 0, BUFFER_SIZE );
	sprintf( buffer, "mkdir -p %s", randomNumFold );
	if ( system( buffer ) )
	{
		perror( buffer );
		exit( 1 );
	}

	currentIndex= 1;
	i = 1;
	m = 1;
	j = 1;
	ind = 0;
	temp = -1;

	i = fileNum;
	//while( i <= TIMES )
	//while( i <= SUB_TIMES )
	while( i == fileNum )
	{
		if( fileNum > 0 & fileNum <= SUB_TIMES )
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
				exit( 1 );
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
					exit( 1 );
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
			//	exit( 1 );
			//}
		}
		if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
		{
			perror( "Open data file" );
			exit( 1 );
		}
		//setbuf( output, NULL );

		temp = -1;
		ranArray[0] = TOTALDATANUM;
		while( j <= TOTALDATANUM )
		{
			//temp = rand() / ( RAND_MAX + 0.5 ) * MAXNUM;
			temp = rand() % TOTALDATANUM;
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
		memset( ranArray, 0, sizeof(unsigned long) * ( TOTALDATANUM + 1 ) );
		i += 1;
	}

	return 0;
}
