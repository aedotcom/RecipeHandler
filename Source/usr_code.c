//===========================================================================================================================
#define _CRT_SECURE_NO_DEPRECATE
//===========================================================================================================================

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include "iodefine.h"

//---------------------------------------------------------------------------------------

int MAX_BUFFER_SIZE = 50000; // 2017.08.20

//---------------------------------------------------------------------------------------
//#define MAX_GLOBAL_BUFFER_COUNT		10
#define MAX_GLOBAL_BUFFER_COUNT		64			// 2004.05.10
//------------------------------------------------------------------------------------------
char *GlobalBuffer[MAX_GLOBAL_BUFFER_COUNT]; // 2004.01.27
int  GlobalBufferCount[MAX_GLOBAL_BUFFER_COUNT]; // 2004.01.27
int  GlobalMakeCount[MAX_GLOBAL_BUFFER_COUNT]; // 2004.01.27
int  GlobalBufferIndex = 0; // 2004.01.27
//------------------------------------------------------------------------------------------
void Global_Buffer_Init() { // 2004.01.27
	int i;
	for ( i = 0 ; i < MAX_GLOBAL_BUFFER_COUNT ; i++ ) {
		GlobalBuffer[i] = NULL;
		GlobalBufferCount[i] = 0;
		GlobalMakeCount[i] = 0;
	}
}
//------------------------------------------------------------------------------------------
char *Global_Buffer_Get( int i ) { // 2004.01.27
	return GlobalBuffer[i];
}
//------------------------------------------------------------------------------------------
int Global_Buffer_IndexCount() { // 2004.01.27
	return GlobalBufferIndex;
}
//------------------------------------------------------------------------------------------
BOOL Global_Buffer_strinit() { // 2004.01.27
	GlobalBufferIndex = 0;
	if ( GlobalBufferCount[GlobalBufferIndex] == 0 ) {
//		GlobalBufferCount[GlobalBufferIndex] = 50000; // 2017.08.20
		GlobalBufferCount[GlobalBufferIndex] = MAX_BUFFER_SIZE; // 2017.08.20
		GlobalBuffer[GlobalBufferIndex] = calloc( GlobalBufferCount[GlobalBufferIndex] + 1 , sizeof( char ) );
		if ( GlobalBuffer[GlobalBufferIndex] == NULL ) {
			GlobalBufferCount[GlobalBufferIndex] = 0;
			return FALSE;
		}
	}
	strcpy( GlobalBuffer[GlobalBufferIndex] , "" );
	GlobalMakeCount[GlobalBufferIndex] = 0;
	return TRUE;
}
//------------------------------------------------------------------------------------------
/*
//
// 2017.08.20
//
BOOL Global_Buffer_strcat( char *data ) { // 2004.01.27
	int l;
	char TempBuffer[256];
	l = strlen( data );
	if ( ( GlobalMakeCount[GlobalBufferIndex] + l ) >= ( GlobalBufferCount[GlobalBufferIndex] - 50 ) ) {
		//
		sprintf( TempBuffer , "%c$B%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , GlobalBufferIndex , SEP_RIGHT_BLACKET );
		//----------------------------------
		strcat( GlobalBuffer[GlobalBufferIndex] , TempBuffer );
		//----------------------------------
		GlobalMakeCount[GlobalBufferIndex] += strlen( TempBuffer );
		//----------------------------------
		GlobalBufferIndex++;
		if ( GlobalBufferIndex >= MAX_GLOBAL_BUFFER_COUNT ) return FALSE;
		//----------------------------------
		if ( GlobalBufferCount[GlobalBufferIndex] == 0 ) {
			GlobalBufferCount[GlobalBufferIndex] = 50000;
			GlobalBuffer[GlobalBufferIndex] = calloc( GlobalBufferCount[GlobalBufferIndex] + 1 , sizeof( char ) );
			if ( GlobalBuffer[GlobalBufferIndex] == NULL ) {
				GlobalBufferCount[GlobalBufferIndex] = 0;
				return FALSE;
			}
		}
		//
		sprintf( GlobalBuffer[GlobalBufferIndex] , "%c$A%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , GlobalBufferIndex , SEP_RIGHT_BLACKET );
		GlobalMakeCount[GlobalBufferIndex] = strlen( GlobalBuffer[GlobalBufferIndex] );
	}
	GlobalMakeCount[GlobalBufferIndex] += l;
	strcat( GlobalBuffer[GlobalBufferIndex] , data );
	return TRUE;
}
//
*/
//
//
// 2017.08.20
//
BOOL Global_Buffer_strcat( char *data ) { // 2017.08.20
	int l , lt;
	char TempBuffer[256];
	//
	l = strlen( data );
	//
	if ( ( GlobalMakeCount[GlobalBufferIndex] + l ) >= ( GlobalBufferCount[GlobalBufferIndex] - 32 ) ) {
		//
		//----------------------------------
		//
		sprintf( TempBuffer , "%c$B%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , GlobalBufferIndex , SEP_RIGHT_BLACKET );
		//
		lt = strlen( TempBuffer );
		//
		memcpy( GlobalBuffer[GlobalBufferIndex] + GlobalMakeCount[GlobalBufferIndex] , TempBuffer , lt );
		//
		GlobalMakeCount[GlobalBufferIndex] += lt;
		//
		GlobalBuffer[GlobalBufferIndex][GlobalMakeCount[GlobalBufferIndex]] = 0;
		//
		//----------------------------------
		GlobalBufferIndex++;
		//
		if ( GlobalBufferIndex >= MAX_GLOBAL_BUFFER_COUNT ) return FALSE;
		//
		//----------------------------------
		if ( GlobalBufferCount[GlobalBufferIndex] == 0 ) {
			GlobalBufferCount[GlobalBufferIndex] = MAX_BUFFER_SIZE;
			GlobalBuffer[GlobalBufferIndex] = calloc( GlobalBufferCount[GlobalBufferIndex] + 1 , sizeof( char ) );
			if ( GlobalBuffer[GlobalBufferIndex] == NULL ) {
				GlobalBufferCount[GlobalBufferIndex] = 0;
				return FALSE;
			}
		}
		//
		//----------------------------------
		//
		sprintf( GlobalBuffer[GlobalBufferIndex] , "%c$A%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , GlobalBufferIndex , SEP_RIGHT_BLACKET );
		GlobalMakeCount[GlobalBufferIndex] = strlen( GlobalBuffer[GlobalBufferIndex] );
		//
		//----------------------------------
		//
	}
	//
	memcpy( GlobalBuffer[GlobalBufferIndex] + GlobalMakeCount[GlobalBufferIndex] , data , l );
	//
	GlobalMakeCount[GlobalBufferIndex] += l;
	//
	GlobalBuffer[GlobalBufferIndex][GlobalMakeCount[GlobalBufferIndex]] = 0;
	//
	return TRUE;
}
//
//