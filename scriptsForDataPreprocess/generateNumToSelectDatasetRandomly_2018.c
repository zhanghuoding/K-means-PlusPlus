#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>

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

char dataPath_2017[GENERAL_SIZE]={'\0'};
char dataPath_2018[GENERAL_SIZE]={'\0'};
char randomNumFold[GENERAL_SIZE] = {'\0'};
char randomNumFile[GENERAL_SIZE] = {'\0'};
char randomNumFileOutput[GENERAL_SIZE] = {'\0'};

char buffer[BUFFER_SIZE]={'\0'};

unsigned long ranArray[TOTALDATANUM + 1]={0};

int main( int argc, char *argv[] )
{
	/*
	 *This program do not need any parameters.
	 */

	srand( (unsigned)time(NULL) );

	sprintf( dataPath_2017, "%s/trace_201708", dataPath );
	sprintf( dataPath_2018, "%s/alibaba_clusterdata_v2018", dataPath );

	/*
	 *first we will split dataset 2018 to 10 sets on the 1201019097.
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

	int currentIndex= 1 ;
	int i = 1;
	int m = 1;
	int j = 1;
	unsigned long ind = 0;
	unsigned long temp = -1;

	while( i <= TIMES )
	{
		currentIndex = 1;
		ranArray[0] = -2;
		j = 1;
		temp = -1;
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
		FILE *output = NULL;
		if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
		{
			perror( "Open data file" );
			exit( 1 );
		}
		//setbuf( output, NULL );

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

	/*
	 * the we will select 10 times and 190760 samples for each time.
	 * we also split the sub-dataset to 10 groups.
	 */

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
		ranArray[0] = -2;

		memset( randomNumFileOutput, 0, GENERAL_SIZE);
		sprintf( randomNumFileOutput, "%s", randomNumFile );
		remove( randomNumFileOutput );
		creat( randomNumFileOutput, 0755 );
	
		FILE *output = NULL;
		if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
		{
			perror( "Open data file" );
			exit( 1 );
		}

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
			sprintf( randomNumFile, "%s/set_%d", randomNumFold, i );
		
			currentIndex = 1;
			ranArray[0] = -2;
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
			FILE *output = NULL;
			if(( output = fopen( randomNumFileOutput, "a+" )) == NULL )
			{
				perror( "Open data file" );
				exit( 1 );
			}
			//setbuf( output, NULL );
	
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
	}

	return 0;
}
