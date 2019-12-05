//################################################################
//#  author   :Bobo Wang                                         #
//#  time     :2019-11-20                                        #
//#  modify   :2019-12-05                                        #
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
int fileNum = 0;

char buffer[BUFFER_SIZE]={'\0'};

unsigned long ranArray[MAXNUM + 1]={0};

int main( int argc, char *argv[] )
{
	/*
	 *This program need one or zero parameters.
	 */
	if( argc == 2 )
	{
		fileNum = atoi( argv[1] );
	}

	srand( (unsigned)time(NULL) );

	sprintf( dataPath_2017, "%s/trace_201708", dataPath );
	sprintf( dataPath_2018, "%s/alibaba_clusterdata_v2018", dataPath );
	sprintf( randomNumFold, "%s/randomNumFold", dataPath_2017 );
	sprintf( randomNumFile, "%s/set_", randomNumFold );

	memset( buffer, 0, BUFFER_SIZE );
	sprintf( buffer, "mkdir -p %s", randomNumFold );
	if ( system( buffer ) )
	{
		perror( buffer );
		exit( 1 );
	}

	int currentIndex= 1;
	int i = 1;
	int m = 1;
	int j = 1;
	unsigned long ind = 0;
	unsigned long temp = -1;

	FILE *output = NULL;

	while( i <= TIMES )
	{
		if( fileNum > 0 && fileNum <= TIMES )
		{
			i = fileNum;
			memset( randomNumFileOutput, 0, GENERAL_SIZE);
			sprintf( randomNumFileOutput, "%s%d", randomNumFile, fileNum );
			if( access( randomNumFileOutput, F_OK ) || access( randomNumFileOutput, R_OK ) )
			{//if file does not exists or un-readable.
				fileNum = -1;
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
				currentIndex -= 4;
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
				continue;
			}
		}
		else
		{
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
