//################################################################
//#  author   :Bobo Wang                                         #
//#  time     :2018-01-09                                        #
//#  modify   :2019-08-14                                        #
//#  site     :Yunnan University                                 #
//#  e-mail   :wangbobochn@gmail.com                             #
//################################################################


#ifndef MY_C_STRING_DEAL_TOOLS
#define MY_C_STRING_DEAL_TOOLS

#include <string.h>
#include <stdio.h>

void replaceFirst(char *str1,char *str2,char *str3);
void replace(char *str1,char *str2,char *str3);
void substring(char *dest,char *src,int start,int end);
char charAt(char *src,int index);
int indexOf(char *str1,char *str2);
int nthAppearingIndexOf(char *str1,char *str2,int n);
int lastIndexOf(char *str1,char *str2);
void ltrim(char *str);
void rtrim(char *str);
void trim(char *str);
void rtrimnr(char *str);
int nthAppearingIndexOf(char *,char *,int);
char *insertAtPtr(char *str1, int size_str1, char *ptr, char *str2);
char *insertAtIndex(char *str1, int size_str1, int index, char *str2);


/*Replace the first occurrence of str2 in str1 with str3*/
void replaceFirst(char *str1,char *str2,char *str3)
{
	char str4[strlen(str1)+1];
	char *p;
	strcpy(str4,str1);
	if((p=strstr(str1,str2))!=NULL)/*p指向str2在str1中第一次出现的位置*/
	{
		while(str1!=p&&str1!=NULL)/*将str1指针移动到p的位置*/
		{
			str1++;
		}
		str1[0]='\0';
		strcat(str1,str3);3,
		strcat(str1,strstr(str4,str2)+strlen(str2));
	}
}
/*Replace all of str2 in str1 with str3*/
void replace(char *str1,char *str2,char *str3)
{
	while(strstr(str1,str2)!=NULL)
	{
		replaceFirst(str1,str2,str3);
	}
}
/*Getting the substring which start from index "start" and end at index "end - 1" in src and save it to dest (the record rule for the index is start from 0).*/
void substring(char *dest,char *src,int start,int end)
{
	char *p=src;
	int i=start;
	if(start>strlen(src))return;
	if(end>strlen(src))
		end=strlen(src);
	while(i<end)
	{	
		dest[i-start]=src[i];
		i++;
	}
	dest[i-start]='\0';
	return;
}
/*Getting the character at "index" in src (the record rule for the index is start fiom 0)*/
char charAt(char *src,int index)
{
	char *p=src;
	int i=0;
	if(index<0||index>strlen(src))
		return 0;
	while(i<index)i++;
	return p[i];
}
/*Returning the position of str2 appearing for the first time in str1,using index format,if it dose not appwar,return -1*/
int indexOf(char *str1,char *str2)
{
	char *p=str1;
	int i=0;
	p=strstr(str1,str2);
	if(p==NULL)
		return -1;
	else{
		while(str1!=p)
		{
			str1++;
			i++;
		}
	}
	return i;
}
/*Returning the position of str2 appearing for the last time in str1,using index format,if it dose not appwar,return -1*/
int lastIndexOf(char *str1,char *str2)
{
	char *p=str1;
	int i=0,len=strlen(str2);
	p=strstr(str1,str2);
	if(p==NULL)return -1;
	while(p!=NULL)
	{
		for(;str1!=p;str1++)i++;
		p=p+len;
		p=strstr(p,str2);
	}
	return i;
}
/*Delete all blank characters in front of the first non-blank character on the left of str,including spaces and horizontal tabs.*/
void ltrim(char *str)
{
	int i=0,j,len=strlen(str);
	while(str[i]!='\0')
	{
		if(str[i]!=32&&str[i]!=9)
			break;/*32:space,9:horizontal tab*/
		i++;
	}
	if(i!=0)
	for(j=0;j<=len-i;j++)
	{	
		str[j]=str[j+i];
	}
}
/*Delete all blank characters after of the last non-blank character on the right of str,including spaces and horizontal tabs.*/
void rtrim(char *str)
{
	char *p=str;
	int i=strlen(str)-1;
	while(i>=0)
	{
		if(p[i]!=32&&p[i]!=9)break;
		i--;
	}
	str[++i]='\0';
}
/*Delete all blank characters on both sides of a,indluding spaces and horizontal tabs.*/
void trim(char *str)
{
	ltrim(str);
	rtrim(str);
}
/*Delete the char '\n' or '\r' at the tail of the string.*/
void rtrimnr(char *str)
{
	if( !str )
		return;
	int len = strlen(str);
	char *pp = str;
	while( --len )
	{
		if( pp[len] == '\n' || pp[len] == '\r' )
			pp[len] = '\0';
		else
			break;
	}
	return;
}
/*Return the index that str2 appearing in str1 for the nth time.*/
int nthAppearingIndexOf(char *str1,char *str2,int n)
{//If the total time is equals n,then return the index for nth time appearing.
 //If the total time is less than n,the function will return -1.
 //If parameter n is 0, return -1.
	if( !n )
		return -1;
	if( !str1 || !str2 )
		return -1;
	int length1 = strlen( str1 );
	int length2 = strlen( str2 );
	if( length1 < 1 )
		return -1;
	int index = 0;
	int co = n;
	int times = 0;

	char *pt = str1;
	int ptlen = length1 - ( pt - str1 );
	while( co-- )
	{
		if( ptlen < length2 )
			break;
		index = 0;
		if( ( index = indexOf( pt, str2 ) ) == -1 )
			break;

		times++;
		pt += index + length2;
		ptlen = length1 - ( pt - str1 );
	}
	if( times < n )
		return -1;
	else
		return pt - length2 - str1;
}
/*Inserting str2 into str1 at the poointer ptr in str1.*/
char *insertAtPtr(char *str1, int size_str1, char *ptr, char *str2)
{//If the original array str1 pointed is enough to store new data,the str2 will be inserted directly
 //and return the original space header address str1.
 //If the original array space is not ecough to store new data, a new space will be created
 //and the new space header address will be returned after inserting the string.
 //If extend array str1 failed, return NULL.
	if( !str1 || !ptr || !str2 )
		return NULL;

	int len1 = strlen( str1 );
	int len2 = strlen( str2 );
	int len3 = strlen(ptr);	
	if( ptr < str1 || ptr >= str1 + size_str1 - 1 )
		return NULL;

	int pi = ptr - str1;
	char *pt = str1;
	if( size_str1 - 4 < len1 + len2 )
		if( ! ( pt = (char*)realloc( str1, sizeof(char) * ( len1 + len2 + 4 ) ) ) )
			return NULL;//extend array failed
	str1 = pt;
	ptr = str1 + pi;
	char *temp = (char*)calloc( len3 + 1, sizeof(char) );
	if( !temp )
		return NULL;
	strncpy( temp, ptr, len3 );
	strncpy( ptr, str2, len2 );
	ptr += len2;
	strncpy( ptr, temp, len3 );
	ptr += len3;
	*ptr = '\0';
	
	return str1;
}
/*Inserting str2 into str1 at the position index in str1.*/
char *insertAtIndex(char *str1, int size_str1, int index, char *str2)
{//The function of this function is the same as function "char *insertAtPtr(char*, char*, char*).
	return insertAtPtr( str1, size_str1, str1 + index, str2 );
}

#endif //MY_C_STRING_DEAL_TOOLS
