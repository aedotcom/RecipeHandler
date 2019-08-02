//===========================================================================================================================
#define _CRT_SECURE_NO_DEPRECATE
//===========================================================================================================================

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <time.h>

#include <Kutlstr.h>
#include <Kutlanal.h>
#include <Kutltime.h>
#include <Kutlfile.h>

#include "k:\h\FiletoSMIntf.h" // 2015.07.20

#include "cimseqnc.h"
#include "iodefine.h"


extern int MAX_BUFFER_SIZE; // 2017.08.20

//#define	MAX_TUNE_COUNT		128 // 2006.04.13
//#define	MAX_TUNE_COUNT		1024 // 2006.04.13
#define	MAX_TUNE_COUNT			2048 // 2007.06.08
//
#define	MAX_TUNE_FILE_COUNT		256 // 2010.08.20
//
#define	MAX_UPFILT_COUNT		512 // 2007.06.11
//
#define	MAX_TUNE_SAVE_COUNT		1024 // 2013.03.28
//
#define	MAX_EVTRECV_COUNT	64
//
#define	MAX_STEP_USE_COUNT		1024 // 2019.03.13
//
//------------------------------------------------------------------------------------------
BOOL APPEND_FILE_STEP_CUT_WITH_MAIN = FALSE; // 2006.04.14
BOOL UPLOAD_FILTERING = FALSE; // 2006.04.14
BOOL STEP0_FILE_MIXING = FALSE; // 2006.04.14
BOOL MULTI_FILE_MIXING = FALSE; // 2007.04.06
//
int  FILE_READ_MODE = 1; // Auto , File , ShMem 2015.07.20
BOOL FILE_SHMEM_INIT = FALSE; // 2015.07.20
//
BOOL TUNE_DATA_SAVING = FALSE; // 2013.03.28
int	 TUNE_DATA_SAVE_IO = -1; // 2013.04.28
//
int  TUNE_DATA_FIXING = 0; // 2018.12.07

//
int  INVALID_FILE_OPERATION = -1; // 2013.10.29
int  INVALID_FILE_ALARM = 0; // 2014.01.10
//
int  INVALID_MESSAGE_OPERATION = 0; // 2015.04.27
int  INVALID_MESSAGE_ALARM = 0; // 2015.04.27

int  TIME_MESSAGE_OPERATION = 0; // 2015.04.27
int  TIME_MESSAGE_ALARM = 0; // 2015.04.27
//------------------------------------------------------------------------------------------
int  PRE_RUN_FUNCTION = -1; // 2006.10.31
//
//char PRE_RUN_FUNCTION_STR[256]; // 2006.10.31 // 2008.04.04
char *PRE_RUN_FUNCTION_STR = NULL; // 2006.10.31 // 2008.04.04
//
int  POST_RUN_FUNCTION = -1; // 2006.11.14
//char POST_RUN_FUNCTION_STR[256]; // 2006.11.14 // 2008.04.04
char *POST_RUN_FUNCTION_STR = NULL; // 2006.11.14 // 2008.04.04
//------------------------------------------------------------------------------------------
char *UPLOAD_FIL_STR = NULL; // 2006.04.14
//
BOOL SINGLE_MODE = FALSE; // 2006.04.28
//
int RECALL_INDEX = 0; // 2006.04.28
//
BOOL MAIN_STATUS;
BOOL TIME_FINISH; // 2006.03.27
int  FUNCTION_ADDRESS[3] = { -1 , -1 , -1 };
int  FUNCTION_FILE_ADDRESS = -1;
//
int  FUNCTION_TIME_GATEWAY_ADDRESS = -1; // 2017.04.01
int  RUNIO_ADDRESS = -1; // 2017.04.01
//
//char OPTION[63+1]; // 2008.04.04
char *OPTION = NULL; // 2008.04.04
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
int  TUNE_STEP_START_INDEX[MAX_STEP_USE_COUNT];
int  TUNE_STEP_END_INDEX[MAX_STEP_USE_COUNT];
//
int	 TUNE_COUNT = 0;
int  TUNE_STEP[MAX_TUNE_COUNT];
int  TUNE_MIDX[MAX_TUNE_COUNT]; // 2007.06.01
int  TUNE_MODE[MAX_TUNE_COUNT]; // 2007.06.07
char *TUNE_NAME[MAX_TUNE_COUNT]; // 2007.06.08
char *TUNE_DATA[MAX_TUNE_COUNT]; // 2007.06.08
//------------------------------------------------------------------------------------------
int  TUNE_FILE_MODE[MAX_TUNE_FILE_COUNT]; // 2010.08.20
int  TUNE_FILE_LOT[MAX_TUNE_FILE_COUNT]; // 2010.08.20
int  TUNE_FILE_SLOT[MAX_TUNE_FILE_COUNT]; // 2010.08.20
char *TUNE_FILE_NAME[MAX_TUNE_FILE_COUNT]; // 2010.08.20
//------------------------------------------------------------------------------------------
int	 UPFILT_COUNT = 0; // 2007.06.11
int  UPFILT_STEP[MAX_UPFILT_COUNT]; // 2007.06.11
int  UPFILT_MODE[MAX_UPFILT_COUNT]; // 2013.03.28
char *UPFILT_NAME[MAX_UPFILT_COUNT]; // 2007.06.11
char *UPFILT_DATA[MAX_UPFILT_COUNT]; // 2007.06.11
//------------------------------------------------------------------------------------------
int	 TUNE_SAVE_COUNT = 0; // 2013.03.28
int  TUNE_SAVE_MODE[MAX_TUNE_SAVE_COUNT]; // 2013.03.28
int  TUNE_SAVE_TINDEX[MAX_TUNE_SAVE_COUNT]; // 2013.03.28
//------------------------------------------------------------------------------------------

IO_Name_String_Map IO_STR_Map_Table[] = {
	""	
};
//------------------------------------------------------------------------------------------
//char Program_Param_Read_Buffer[512+1]; // 2004.10.07 // 2010.09.02
char Program_Param_Read_Buffer[1024+1]; // 2004.10.07 // 2010.09.02
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
int  Event_Parameter_Received_Tag = 0; // 2004.10.07
int  Event_Parameter_Received_Count = 0; // 2006.03.22
//char Event_Parameter_Data[MAX_EVTRECV_COUNT][512+1]; // 2006.03.22 // 2008.04.04
char *Event_Parameter_Data[MAX_EVTRECV_COUNT]; // 2006.03.22 // 2008.04.04
int  Event_Parameter_Mode[MAX_EVTRECV_COUNT]; // 2010.03.10
//------------------------------------------------------------------------------------------
CRITICAL_SECTION Event_Parameter_CR; // 2004.10.07
//
CRITICAL_SECTION Event_Tunning_CR; // 2019.03.13
//---------------------------------------------------------------------------------------

//char	FILE_ID[63+1]; // 2008.04.04
char	*FILE_ID = NULL; // 2008.04.04
//char	RECIPE_ID[63+1]; // 2008.04.04
char	*RECIPE_ID = NULL; // 2008.04.04
int		MAX_STEP_COUNT;
int		TIMER_IO = -1;
int		TIMER_MODE = 0;
//------------------------------------------------------------------------------------------
void LotData_Change_to( BOOL commode , char *data ) { // 2007.10.07
	int i = 0;
	while ( TRUE ) {
		if ( data[i] == 0 ) break;
		if ( commode ) {
			if      ( data[i] == ' ' ) data[i] = 29;
			else if ( data[i] == ':' ) data[i] = 28;
		}
		else {
			if      ( data[i] == 29 ) data[i] = ' ';
			else if ( data[i] == 28 ) data[i] = ':';
		}
		i++;
	}
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//char FUNCTION_FILE_STRING[10][256]; // 2008.04.04
char *FUNCTION_FILE_STRING[10]; // 2008.04.04
int  FUNCTION_FILE_INDEX = 0;
//------------------------------------------------------------------------------------------
BOOL Function_String_Send_Check( char *data ) {
	int i;
	if ( FUNCTION_FILE_INDEX >= 10 ) return TRUE;
	for ( i = 0 ; i < FUNCTION_FILE_INDEX ; i++ ) {
		if ( STRCMP_L( data , STR_MEM_GET_STR( FUNCTION_FILE_STRING[i] ) ) ) return FALSE;
	}
//	strncpy( FUNCTION_FILE_STRING[FUNCTION_FILE_INDEX] , data , 255 ); // 2008.04.04
	STR_MEM_MAKE_COPY( &(FUNCTION_FILE_STRING[FUNCTION_FILE_INDEX]) , data ); // 2008.04.04
	FUNCTION_FILE_INDEX++;
	return TRUE;
}
//------------------------------------------------------------------------------------------
void Function_String_Send_Reset() {	FUNCTION_FILE_INDEX = 0; }
//------------------------------------------------------------------------------------------
//
void REMAPPING_IO_NAME( char *Target , char *Source ) {
	int i , len;
	while (*Source) {
		if ( *Source == '$' ) {
			if ( OPTION != NULL ) {
				len = strlen( OPTION );
				for ( i = 0 ; i < len ; i++ ) {
					*Target = OPTION[i];
					*Target++;
				}
			}
		}
		else {
			*Target = *Source;
			*Target++;
		}
		*Source++;
	}
	*Target = 0;
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
/*
// 2008.05.30
void Optimize_Float_Buffer( char *strbuffer ) {
	int i , l;
	l = strlen( strbuffer );
	for ( i = 0 ; i < l ; i++ ) {
		if ( strbuffer[i] == '.' ) break;
	}
	if ( i == l ) return;
	for ( i = l - 1 ; i >= 0 ; i-- ) {
		if      ( strbuffer[i] == '0' ) {
			strbuffer[i] = 0;
		}
		else if ( strbuffer[i] == '.' ) {
			strbuffer[i] = 0;
			return;
		}
		else {
			return;
		}
	}
}
*/

void Optimize_FloatDouble_Buffer( char *strbuffer , float fdata , double ddata , BOOL doublemode ) { // 2008.05.30
	int i , l;

	// 6 : 1
	// 5 : 2
	// 4 : 3
	// 3 : 4
	// 2 : 5
	// 1 : 6
	// 0 : 7
	if ( doublemode ) {
		sprintf( strbuffer , "%f" , ddata );
	}
	else {
		if      ( fdata < 1 ) {
			sprintf( strbuffer , "%.7f" , fdata );
		}
		else if ( fdata < 10 ) {
			sprintf( strbuffer , "%.6f" , fdata );
		}
		else if ( fdata < 100 ) {
			sprintf( strbuffer , "%.5f" , fdata );
		}
		else if ( fdata < 1000 ) {
			sprintf( strbuffer , "%.4f" , fdata );
		}
		else if ( fdata < 10000 ) {
			sprintf( strbuffer , "%.3f" , fdata );
		}
		else if ( fdata < 100000 ) {
			sprintf( strbuffer , "%.2f" , fdata );
		}
		else if ( fdata < 1000000 ) {
			sprintf( strbuffer , "%.1f" , fdata );
		}
		else {
			sprintf( strbuffer , "%f" , fdata );
		}
	}
	//
	l = strlen( strbuffer );
	for ( i = 0 ; i < l ; i++ ) {
		if ( strbuffer[i] == '.' ) break;
	}
	if ( i == l ) return;
	for ( i = l - 1 ; i >= 0 ; i-- ) {
		if      ( strbuffer[i] == '0' ) {
			strbuffer[i] = 0;
		}
		else if ( strbuffer[i] == '.' ) {
			strbuffer[i] = 0;
			return;
		}
		else {
			return;
		}
	}
}


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void Find_Tune_Data_Remap_XCR() { // 2010.09.07
	int i , j , z;
//	char Buffer[1024]; // 2019.03.13
	char *Buffer; // 2019.03.13

	for ( i = 0 ; i < TUNE_COUNT - 1 ; i++ ) {
		for ( j = i + 1 ; j < TUNE_COUNT; j++ ) {
			if ( TUNE_STEP[i] > TUNE_STEP[j] ) {
				//
				z = TUNE_MIDX[i];
				TUNE_MIDX[i] = TUNE_MIDX[j];
				TUNE_MIDX[j] = z;
				//
				z = TUNE_MODE[i];
				TUNE_MODE[i] = TUNE_MODE[j];
				TUNE_MODE[j] = z;
				//
				z = TUNE_STEP[i];
				TUNE_STEP[i] = TUNE_STEP[j];
				TUNE_STEP[j] = z;
				//
//				strcpy( Buffer , TUNE_NAME[i] ); // 2019.03.13
//				strcpy( TUNE_NAME[i] , TUNE_NAME[j] ); // 2019.03.13
//				strcpy( TUNE_NAME[j] , Buffer ); // 2019.03.13
//				strcpy( Buffer , TUNE_DATA[i] ); // 2019.03.13
//				strcpy( TUNE_DATA[i] , TUNE_DATA[j] ); // 2019.03.13
//				strcpy( TUNE_DATA[j] , Buffer ); // 2019.03.13
				//
				// 2019.03.13
				//
				Buffer = TUNE_NAME[i];
				TUNE_NAME[i] = TUNE_NAME[j];
				TUNE_NAME[j] = Buffer;
				//
				Buffer = TUNE_DATA[i];
				TUNE_DATA[i] = TUNE_DATA[j];
				TUNE_DATA[j] = Buffer;
				//
			}
		}
	}
	for ( i = 0 ; i < MAX_STEP_USE_COUNT; i++ ) TUNE_STEP_START_INDEX[i] = -1;
	for ( i = 0 ; i < TUNE_COUNT ; i++ ) {
		if ( TUNE_STEP_START_INDEX[TUNE_STEP[i]] == -1 ) {
			TUNE_STEP_START_INDEX[TUNE_STEP[i]] = i;
		}
		TUNE_STEP_END_INDEX[TUNE_STEP[i]] = i;
	}
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
void TUNE_FILE_INIT() { // 2010.08.20
	int i;
	for ( i = 0 ; i < MAX_TUNE_FILE_COUNT ; i++ ) {
		TUNE_FILE_MODE[i] = -99;
		TUNE_FILE_LOT[i] = 0;
		TUNE_FILE_SLOT[i] = 0;
		TUNE_FILE_NAME[i] = NULL;
	}
}






void TUNE_FILE_DELETE_OCR( int lotid ) { // 2010.08.20
	int i;

//printf( "TUNE_FILE_DELETE [S] [%d]\n" , lotid );
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------

	for ( i = 0 ; i < MAX_TUNE_FILE_COUNT ; i++ ) {
		if ( TUNE_FILE_MODE[i] == -99 ) continue;
		if ( TUNE_FILE_LOT[i] != lotid ) continue;
		TUNE_FILE_MODE[i] = -99;

//printf( "TUNE_FILE_DELETE [D] [%d]\n" , lotid );
	}
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
}

void TUNE_FILE_ADD_OCR( int lotid , int mode , int slot , char *name ) { // 2010.08.20
	int i;
//printf( "TUNE_FILE_ADD [S] [lotid=%d][mode=%d][slot=%d][name=%s]\n" , lotid , mode , slot , name );
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	for ( i = 0 ; i < MAX_TUNE_FILE_COUNT ; i++ ) {
		if ( TUNE_FILE_MODE[i] != -99 ) continue;
		TUNE_FILE_MODE[i] = mode;
		TUNE_FILE_LOT[i] = lotid;
		TUNE_FILE_SLOT[i] = slot;
		STR_MEM_MAKE_COPY2( &(TUNE_FILE_NAME[i]) , name );
//printf( "TUNE_FILE_ADD [I%d] [lotid=%d][mode=%d][slot=%d][name=%s]\n" , i , lotid , mode , slot , name );
		//-------------------------------------------------------------------
		//
		LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
		//
		//-------------------------------------------------------------------
		return;
	}
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
}

//void TUNE_FILE_SET( int lotid , int mode , int slot ) { // 2010.08.20 // 2019.03.13
void TUNE_FILE_SET_SUB_XCR( int lotid , int mode , int slot ) { // 2010.08.20 // 2019.03.13
	HFILE hFile;
	char Buffer[256];
	char Buffer1[256];
	char Buffer2[256];
	char chr_return[256];
	int  ReadCnt;
	BOOL FileEnd;
	int  doact = -1; // 2010.09.07
	FILE *fpt; // 2010.09.07
	//
	int i;
//printf( "TUNE_FILE_SET [S] [lotid=%d][mode=%d][slot=%d]\n" , lotid , mode , slot );
	for ( i = 0 ; i < MAX_TUNE_FILE_COUNT ; i++ ) {
		//-----------------------------------------------------------------------------
		if ( TUNE_FILE_MODE[i] == -99 ) continue;
		if ( TUNE_FILE_LOT[i] != lotid ) continue;
		if ( TUNE_FILE_MODE[i] != -1 ) {
			//
			if ( ( ( TUNE_FILE_MODE[i] / 10 ) > 0 ) && ( ( TUNE_FILE_MODE[i] % 10 ) == 0 ) ) {
				if ( ( TUNE_FILE_MODE[i] / 10 ) != ( mode / 10 ) ) continue;
			}
			else {
				if ( TUNE_FILE_MODE[i] != mode ) continue;
			}
			//
		}
		if      ( TUNE_FILE_SLOT[i] >= 0 ) {
			if ( TUNE_FILE_SLOT[i] != slot ) continue;
		}
		//-----------------------------------------------------------------------------
//printf( "TUNE_FILE_SET [F] [lotid=%d][mode=%d][slot=%d][File=%s]\n" , lotid , mode , slot , TUNE_FILE_NAME[i] );
		//-----------------------------------------------------------------------------
		hFile = _lopen( TUNE_FILE_NAME[i] , OF_READ );
		if ( hFile == -1 ) return;
		//-----------------------------------------------------------------------------
		FileEnd = TRUE;
		//
		while( FileEnd ) {
			//=================================================================================
			FileEnd = H_Get_Line_String_From_File( hFile , Buffer , &ReadCnt );
			//=================================================================================
			if ( ReadCnt <= 2 ) continue;
			//=================================================================================
			if ( !Get_Data_From_String( Buffer , chr_return , 0 ) ) break;
			//=================================================================================
			if ( STRCMP_L( chr_return , "F" ) ) continue;
			//=================================================================================
			if ( atoi( chr_return ) == 0 ) continue;
			//=================================================================================
			if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) break;
			//=================================================================================
			if ( TUNE_COUNT >= MAX_TUNE_COUNT ) break;
			//=================================================================================
			if ( TUNE_NAME[ TUNE_COUNT ] == NULL ) {
				TUNE_NAME[TUNE_COUNT] = calloc( 127 + 1 , sizeof( char ) );
				if ( TUNE_NAME[ TUNE_COUNT ] == NULL ) break;
			}
			//
			if ( TUNE_DATA[ TUNE_COUNT ] == NULL ) {
				TUNE_DATA[TUNE_COUNT] = calloc( 127 + 1 , sizeof( char ) );
				if ( TUNE_DATA[ TUNE_COUNT ] == NULL ) break;
			}
			//=================================================================================
			STR_SEPERATE_CHAR( chr_return  , ':' , Buffer1 , Buffer2 , 255 );
			TUNE_STEP[TUNE_COUNT] = atoi( Buffer1 ) + 1;
			//
			if ( TUNE_STEP[TUNE_COUNT] < 0 ) continue; // 2019.03.13
			if ( TUNE_STEP[TUNE_COUNT] >= MAX_STEP_USE_COUNT ) continue; // 2019.03.13
			//
			//=================================================================================
			STR_SEPERATE_CHAR( Buffer2  , ':' , Buffer1 , chr_return , 255 );
			strncpy( TUNE_NAME[TUNE_COUNT] , Buffer1 , 127 );
			TUNE_NAME[TUNE_COUNT][127] = 0; // 2019.03.13
			//=================================================================================
			STR_SEPERATE_CHAR( chr_return  , ':' , Buffer1 , Buffer2 , 255 );
			strncpy( TUNE_DATA[TUNE_COUNT] , Buffer1 , 127 );
			TUNE_DATA[TUNE_COUNT][127] = 0; // 2019.03.13
			//============================================================================
			TUNE_MIDX[TUNE_COUNT] = -1;
			//============================================================================
//			TUNE_MODE[TUNE_COUNT] = 1; // 2013.03.28
			TUNE_MODE[TUNE_COUNT] = 2; // 2013.03.28
			//============================================================================
//printf( "TUNE_FILE_SET [A] [%d] [STEP=%d][NAME=%s][DATA=%s]\n" , TUNE_COUNT , TUNE_STEP[TUNE_COUNT] , TUNE_NAME[TUNE_COUNT] , TUNE_DATA[ TUNE_COUNT ] );

			TUNE_COUNT++;
			//
			//
			//
			doact = i; // 2010.09.07
			//=================================================================================
		}
		//
		_lclose( hFile );
		//
		break;
	}
	//
	if ( doact != -1 ) {
		//
		Find_Tune_Data_Remap_XCR(); // 2010.09.07
		//
		fpt = fopen( TUNE_FILE_NAME[doact] , "a" );
		if ( fpt != NULL ) {
			fprintf( fpt , "ADAPT\n" );
			fclose( fpt );
		}
	}
	//
}

void TUNE_FILE_SET_OCR( int lotid , int mode , int slot ) { // 2019.03.13
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	TUNE_FILE_SET_SUB_XCR( lotid , mode , slot );
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
}
//------------------------------------------------------------------------------------------

BOOL File_Save_Tune_Data_Setting_XCR( int mode , int tindex ) { // 2013.03.28
	//
	int i;
	//
	if ( mode == -99 ) {
		TUNE_SAVE_COUNT = 0;
		return TRUE;
	}
	//
	if ( !TUNE_DATA_SAVING ) return TRUE;
	//
	for ( i = 0 ; i < TUNE_SAVE_COUNT ; i++ ) {
		if ( ( TUNE_SAVE_MODE[i] == mode ) && ( TUNE_SAVE_TINDEX[i] == tindex ) ) return TRUE;
	}
	//
	if ( TUNE_SAVE_COUNT >= MAX_TUNE_SAVE_COUNT ) {
		_IO_CIM_PRINTF( "File_Save_Tune_Data_Setting Error - Too Many Data Set\n" );
		return FALSE;
	}
	//
	TUNE_SAVE_MODE[TUNE_SAVE_COUNT] = mode; // 2013.03.28
	TUNE_SAVE_TINDEX[TUNE_SAVE_COUNT] = tindex; // 2013.03.28
	//
	TUNE_SAVE_COUNT++;
	//
	return TRUE;
}





//int CHECK_COUNT;




//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
int Find_Tune_And_Redefine_CharInteger_XCR( int MIndex , char *name , int step , char *cc_return , int *ii_return , BOOL intmode ) {
	int i;
	//
	if ( TUNE_COUNT <= 0 ) return 0;
	if ( TUNE_STEP_START_INDEX[step] == -1 ) return 0;
	for ( i = TUNE_STEP_START_INDEX[step] ; i <= TUNE_STEP_END_INDEX[step] ; i++ ) {
		if ( ( TUNE_MIDX[i] == -1 ) || ( TUNE_MIDX[i] == MIndex ) ) { // 2007.06.01
			if ( STRCMP_L( name , TUNE_NAME[i] ) ) {
				if ( strlen( TUNE_DATA[i] ) > 0 ) {
					//
					if ( STRCMP_L( TUNE_DATA[i] , "$<REMOVE>$" ) ) { // 2011.05.18
						//
						if ( ( TUNE_MODE[i] == 1 ) || ( TUNE_MODE[i] == -1 ) ) { // 2013.03.28
							File_Save_Tune_Data_Setting_XCR( -1 , i );
							TUNE_MODE[i] = -1;
						}
						else {
							TUNE_MODE[i] = 0;
						}
						//
						return -1;
					}
					else {
						//============================================================
						if ( ( TUNE_MODE[i] == 1 ) || ( TUNE_MODE[i] == -1 ) ) { // 2013.03.28
							File_Save_Tune_Data_Setting_XCR( 0 , i );
							TUNE_MODE[i] = -1;
						}
						else {
							TUNE_MODE[i] = 0; // 2007.06.07
						}
						//============================================================
						if ( intmode ) {
							*ii_return = atoi( TUNE_DATA[i] );
						}
						else {
							*cc_return = (char) atoi( TUNE_DATA[i] );
						}




//CHECK_COUNT++;




						return 1;
					}
					//
				}
				return 0;
			}
		}
	}
	return 0;
}
//

int Find_Tune_And_Redefine_CharInteger_OCR( int MIndex , char *name , int step , char *cc_return , int *ii_return , BOOL intmode ) {
	int Res;
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	Res = Find_Tune_And_Redefine_CharInteger_XCR( MIndex , name , step , cc_return , ii_return , intmode );
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	return Res;
}

int Find_Tune_And_Redefine_FloatDouble_XCR( int MIndex , char *name , int step , float *ff_return , double *dd_return , BOOL doublemode ) {
	int i;
	//
	if ( TUNE_COUNT <= 0 ) return 0;
	if ( TUNE_STEP_START_INDEX[step] == -1 ) return 0;
	for ( i = TUNE_STEP_START_INDEX[step] ; i <= TUNE_STEP_END_INDEX[step] ; i++ ) {
		if ( ( TUNE_MIDX[i] == -1 ) || ( TUNE_MIDX[i] == MIndex ) ) { // 2007.06.01
			if ( STRCMP_L( name , TUNE_NAME[i] ) ) {
				if ( strlen( TUNE_DATA[i] ) > 0 ) {
					//
					if ( STRCMP_L( TUNE_DATA[i] , "$<REMOVE>$" ) ) { // 2011.05.18
						//
						if ( ( TUNE_MODE[i] == 1 ) || ( TUNE_MODE[i] == -1 ) ) { // 2013.03.28
							File_Save_Tune_Data_Setting_XCR( -1 , i );
							TUNE_MODE[i] = -1;
						}
						else {
							TUNE_MODE[i] = 0;
						}
						//
						return -1;
					}
					else {
						//============================================================
						if ( ( TUNE_MODE[i] == 1 ) || ( TUNE_MODE[i] == -1 ) ) { // 2013.03.28
							File_Save_Tune_Data_Setting_XCR( 0 , i );
							TUNE_MODE[i] = -1;
						}
						else {
							TUNE_MODE[i] = 0; // 2007.06.07
						}
						//============================================================
						if ( doublemode ) { // 2008.05.30
							*dd_return = atof( TUNE_DATA[i] );
						}
						else {
							*ff_return = (float) atof( TUNE_DATA[i] );
						}




//CHECK_COUNT++;




						return 1;
					}
				}
				return 0;
			}
		}
	}
	return 0;
}
//


int Find_Tune_And_Redefine_FloatDouble_OCR( int MIndex , char *name , int step , float *ff_return , double *dd_return , BOOL doublemode ) {
	int Res;
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	Res = Find_Tune_And_Redefine_FloatDouble_XCR( MIndex , name , step , ff_return , dd_return , doublemode );
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	return Res;
}

int Find_Tune_And_Redefine_String_XCR( int MIndex , char *name , int step , char *ss_return ) {
	int i;
	//
	if ( TUNE_COUNT <= 0 ) return 0;
	if ( TUNE_STEP_START_INDEX[step] == -1 ) return 0;
	for ( i = TUNE_STEP_START_INDEX[step] ; i <= TUNE_STEP_END_INDEX[step] ; i++ ) {
		if ( ( TUNE_MIDX[i] == -1 ) || ( TUNE_MIDX[i] == MIndex ) ) { // 2007.06.01
			if ( STRCMP_L( name , TUNE_NAME[i] ) ) {
				//
				if ( STRCMP_L( TUNE_DATA[i] , "$<REMOVE>$" ) ) { // 2011.05.18
					//
					if ( ( TUNE_MODE[i] == 1 ) || ( TUNE_MODE[i] == -1 ) ) { // 2013.03.28
						File_Save_Tune_Data_Setting_XCR( -1 , i );
						TUNE_MODE[i] = -1;
					}
					else {
						TUNE_MODE[i] = 0;
					}
					//
					return -1;
				}
				else {
					//============================================================
					if ( ( TUNE_MODE[i] == 1 ) || ( TUNE_MODE[i] == -1 ) ) { // 2013.03.28
						File_Save_Tune_Data_Setting_XCR( 0 , i );
						TUNE_MODE[i] = -1;
					}
					else {
						TUNE_MODE[i] = 0; // 2007.06.07
					}
					//============================================================
					strcpy( ss_return , TUNE_DATA[i] );




//CHECK_COUNT++;




					return 1;
				}
			}
		}
	}
	return 0;
}
//

int Find_Tune_And_Redefine_String_OCR( int MIndex , char *name , int step , char *ss_return ) {
	int Res;
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	Res = Find_Tune_And_Redefine_String_XCR( MIndex , name , step , ss_return );
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	return Res;
}



BOOL Find_Tune_And_Append_XCR( int MIndex , int step , char *SendBuffer ) { // 2007.06.07
	int i;
	//
	if ( TUNE_COUNT <= 0 ) return TRUE;
	if ( TUNE_STEP_START_INDEX[step] == -1 ) return TRUE;
	for ( i = TUNE_STEP_START_INDEX[step] ; i <= TUNE_STEP_END_INDEX[step] ; i++ ) {
		if ( ( TUNE_MIDX[i] == -1 ) || ( TUNE_MIDX[i] == MIndex ) ) {
//			if ( TUNE_MODE[i] != 0 ) { // 2013.03.28
			if ( TUNE_MODE[i] > 0 ) { // 2013.03.28
				//
				if ( TUNE_MODE[i] == 1 ) { // 2013.03.28
					File_Save_Tune_Data_Setting_XCR( 1 , i );
					TUNE_MODE[i] = -1;
				}
				else {
					TUNE_MODE[i] = 0; // 2007.06.07
				}
				//
				sprintf( SendBuffer , "%c%s%c%s%c" , SEP_LEFT_BLACKET , TUNE_NAME[i] , SEP_NAME_DATA , TUNE_DATA[i] , SEP_RIGHT_BLACKET );
				if ( !Global_Buffer_strcat( SendBuffer ) ) return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL Find_Tune_And_Append_OCR( int MIndex , int step , char *SendBuffer ) { // 2007.06.07
	BOOL Res;
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	Res = Find_Tune_And_Append_XCR( MIndex , step , SendBuffer );
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	return Res;
}




//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
BOOL SETUP_FILE( char *Filename ) {
	FILE *fpt;
	char Buffer[ 63+1 ];
	char chr_return[ 63+1 ];
	char chr_return2[ 63+1 ];
	int  ReadCnt , Line;
	BOOL FileEnd = TRUE;

	for ( Line = 0 ; Line < 3 ; Line ++ ) {
//		if ( ( fpt = fopen( Filename , "r+t" ) ) != NULL ) break; // 2005.03.22
		if ( ( fpt = fopen( Filename , "r" ) ) != NULL ) break; // 2005.03.22
//		_sleep(100); // 2013.03.28
		Sleep(100); // 2013.03.28
	}
	if ( Line == 3 ) {
//		_IO_CIM_PRINTF( "File Open Error - [%s]\n" , Filename );							// 2009.06.19
		_IO_CIM_PRINTF( "File Open Error - [%s] - [%ld]\n" , Filename, GetLastError() );	// 2009.06.19
		return FALSE;
	}
	for ( Line = 1 ; FileEnd ; Line++ ) {
		FileEnd = Get_Line_String_From_File( fpt , Buffer , &ReadCnt );
		if ( ReadCnt > 0 ) {
			if ( !Get_Data_From_String( Buffer , chr_return , 0 ) ) {
				_IO_CIM_PRINTF( "Unknown Error 1 - [%s]-[%d]\n" , Filename , Line );
				fclose(fpt); // 2013.10.29
				return FALSE;
			}
			if ( STRCMP_L( chr_return , "FILE_ID" ) ) {
				if ( ReadCnt > 1 ) {
//					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , FILE_ID ) ) { // 2008.04.04
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) { // 2008.04.04
						_IO_CIM_PRINTF( "FILE ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					STR_MEM_MAKE_COPY( &FILE_ID , chr_return2 ); // 2008.04.04
				}
			}
			else if ( STRCMP_L( chr_return , "RECIPE_ID" ) ) {
				if ( ReadCnt > 1 ) {
//					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , RECIPE_ID ) ) { // 2008.04.04
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) { // 2008.04.04
						_IO_CIM_PRINTF( "RECIPE ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					STR_MEM_MAKE_COPY( &RECIPE_ID , chr_return2 ); // 2008.04.04
				}
			}
			else if ( STRCMP_L( chr_return , "FUNCTION" ) ) { 
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "FUNCTION ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					//------------------------------------------------------------
					SINGLE_MODE = TRUE; // 2006.04.28
					//------------------------------------------------------------
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					FUNCTION_ADDRESS[0] = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( FUNCTION_ADDRESS[0] == -1 ) {
						_IO_CIM_PRINTF( "FUNCTION1 Can not Find => Will Go Success Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
				}
			}
			else if ( STRCMP_L( chr_return , "FUNCTION1" ) || STRCMP_L( chr_return , "PROCESS_IN" ) ) { 
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "FUNCTION ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					//------------------------------------------------------------
					SINGLE_MODE = FALSE; // 2006.04.28
					//------------------------------------------------------------
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					FUNCTION_ADDRESS[0] = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( FUNCTION_ADDRESS[0] == -1 ) {
						_IO_CIM_PRINTF( "FUNCTION1 Can not Find => Will Go Success Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
				}
			}
			else if ( STRCMP_L( chr_return , "FUNCTION2" ) || STRCMP_L( chr_return , "PROCESS_OUT" ) ) {
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "FUNCTION ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					//------------------------------------------------------------
					SINGLE_MODE = FALSE; // 2006.04.28
					//------------------------------------------------------------
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					FUNCTION_ADDRESS[1] = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( FUNCTION_ADDRESS[1] == -1 ) {
						_IO_CIM_PRINTF( "FUNCTION2 Can not Find => Will Go Success Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
				}
			}
			else if ( STRCMP_L( chr_return , "FUNCTION3" ) || STRCMP_L( chr_return , "MAINTENANCE" ) ) {
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "FUNCTION ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					//------------------------------------------------------------
					SINGLE_MODE = FALSE; // 2006.04.28
					//------------------------------------------------------------
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					FUNCTION_ADDRESS[2] = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( FUNCTION_ADDRESS[2] == -1 ) {
						_IO_CIM_PRINTF( "FUNCTION3 Can not Find => Will Go Success Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
				}
			}
			else if ( STRCMP_L( chr_return , "FUNCTION4" ) || STRCMP_L( chr_return , "FILE_INCLUDE" ) ) {
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "INCLUDE_FILE ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					FUNCTION_FILE_ADDRESS = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( FUNCTION_FILE_ADDRESS == -1 ) {
						_IO_CIM_PRINTF( "FUNCTION4 Can not Find => Will Go Unused Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
				}
			}
			else if ( STRCMP_L( chr_return , "FUNCTION_TIME" ) || STRCMP_L( chr_return , "FUNCTION_TIME_GATEWAY" ) ) { // 2017.04.01
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "FUNCTION ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					FUNCTION_TIME_GATEWAY_ADDRESS = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( FUNCTION_TIME_GATEWAY_ADDRESS == -1 ) {
						_IO_CIM_PRINTF( "FUNCTION_TIME Can not Find => Will Go Success Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
				}
			}
			else if ( STRCMP_L( chr_return , "RUNIO" ) ) { // 2017.04.01
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "RUNIO Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					RUNIO_ADDRESS = _FIND_FROM_STRING( _K_D_IO , chr_return );
				}
			}
			else if ( STRCMP_L( chr_return , "TIMER" ) ) {
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "TIMER IO Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					TIMER_IO = _FIND_FROM_STRING( _K_D_IO , chr_return );
					if ( TIMER_IO == -1 ) {
//						_IO_CIM_PRINTF( "TIMER IO NAME Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return ); // 2006.07.19
					}
					if ( ReadCnt > 2 ) {
						if ( !Get_Data_From_String( Buffer , chr_return , 2 ) ) {
							_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
							fclose(fpt); // 2013.10.29
							return FALSE;
						}
						if      ( STRCMP_L( chr_return , "TIME_UP"         ) ) TIMER_MODE = 0;
						else if ( STRCMP_L( chr_return , "TIME_DOWN"       ) ) TIMER_MODE = 1;
						else if ( STRCMP_L( chr_return , "PERCENT_UP"      ) ) TIMER_MODE = 2;
						else if ( STRCMP_L( chr_return , "PERCENT_DOWN"    ) ) TIMER_MODE = 3;
						else if ( STRCMP_L( chr_return , "TIME_UP(E)"      ) ) TIMER_MODE = 4;
						else if ( STRCMP_L( chr_return , "TIME_DOWN(E)"    ) ) TIMER_MODE = 5;
						else if ( STRCMP_L( chr_return , "PERCENT_UP(E)"   ) ) TIMER_MODE = 6;
						else if ( STRCMP_L( chr_return , "PERCENT_DOWN(E)" ) ) TIMER_MODE = 7;
						else                                                   TIMER_MODE = 0;
					}
				}
			}
			else if ( STRCMP_L( chr_return , "MAX_STEP" ) ) {
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_INT( Buffer , 1 , chr_return , 1 , MAX_STEP_USE_COUNT - 1 , &MAX_STEP_COUNT ) ) {
						_IO_CIM_PRINTF( "MAX_STEP Data Error - [%s]-[%d] -%s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
				}
			}
			else if ( STRCMP_L( chr_return , "APPEND_FILE_STEP_CUT" ) ) { // 2006.04.14
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					if ( STRCMP_L( chr_return , "ON" ) ) {
						APPEND_FILE_STEP_CUT_WITH_MAIN = TRUE;
					}
					else {
						APPEND_FILE_STEP_CUT_WITH_MAIN = FALSE;
					}
				}
			}
			else if ( STRCMP_L( chr_return , "UPLOAD_FILTERING" ) ) { // 2006.04.14
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					if ( STRCMP_L( chr_return , "ON" ) ) {
						UPLOAD_FILTERING = TRUE;
					}
					else {
						UPLOAD_FILTERING = FALSE;
					}
				}
			}
			else if ( STRCMP_L( chr_return , "STEP0_FILE_MIXING" ) ) { // 2006.04.14
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					if ( STRCMP_L( chr_return , "ON" ) ) {
						STEP0_FILE_MIXING = TRUE;
					}
					else {
						STEP0_FILE_MIXING = FALSE;
					}
				}
			}
			else if ( STRCMP_L( chr_return , "MULTI_FILE_MIXING" ) ) { // 2007.04.06
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					if ( STRCMP_L( chr_return , "ON" ) ) {
						MULTI_FILE_MIXING = TRUE;
					}
					else {
						MULTI_FILE_MIXING = FALSE;
					}
				}
			}
			else if ( STRCMP_L( chr_return , "TUNE_DATA_SAVING" ) ) { // 2013.03.28
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					if ( STRCMP_L( chr_return , "ON" ) ) {
						TUNE_DATA_SAVING = TRUE;
					}
					else {
						TUNE_DATA_SAVING = FALSE;
					}
					//
					if ( ReadCnt > 2 ) { // 2013.04.28
						if ( !Get_Data_From_String_for_STRING( Buffer , 2 , chr_return , 63 , chr_return2 ) ) {
							_IO_CIM_PRINTF( "FUNCTION ID Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
							fclose(fpt); // 2013.10.29
							return FALSE;
						}
						//------------------------------------------------------------
						REMAPPING_IO_NAME( chr_return , chr_return2 );
						TUNE_DATA_SAVE_IO = _FIND_FROM_STRING( _K_D_IO , chr_return );
						if ( TUNE_DATA_SAVE_IO == -1 ) {
							_IO_CIM_PRINTF( "TUNE_DATA_SAVING Can not Find IO [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						}
					}
					else {
						TUNE_DATA_SAVE_IO = -1;
					}
				}
			}
			else if ( STRCMP_L( chr_return , "PRE_RUN_FUNCTION" ) ) { // 2006.10.31
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "PRE_RUN_FUNCTION Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					PRE_RUN_FUNCTION = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( PRE_RUN_FUNCTION == -1 ) {
						_IO_CIM_PRINTF( "PRE_RUN_FUNCTION Can not Find => Will Go Success Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
					else {
//						strcpy( PRE_RUN_FUNCTION_STR , "" ); // 2008.04.04
						STR_MEM_MAKE_COPY( &PRE_RUN_FUNCTION_STR , "" ); // 2008.04.04
						if ( ReadCnt > 2 ) {
							if ( !Get_Data_From_String_for_STRING( Buffer , 2 , chr_return , 255 , chr_return2 ) ) {
								_IO_CIM_PRINTF( "PRE_RUN_FUNCTION Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
								fclose(fpt); // 2013.10.29
								return FALSE;
							}
//							REMAPPING_IO_NAME( PRE_RUN_FUNCTION_STR , chr_return2 ); // 2008.04.04
							REMAPPING_IO_NAME( chr_return , chr_return2 ); // 2008.04.04
							STR_MEM_MAKE_COPY( &PRE_RUN_FUNCTION_STR , chr_return ); // 2008.04.04
						}
					}
				}
			}
			else if ( STRCMP_L( chr_return , "POST_RUN_FUNCTION" ) ) { // 2006.11.14
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_STRING( Buffer , 1 , chr_return , 63 , chr_return2 ) ) {
						_IO_CIM_PRINTF( "POST_RUN_FUNCTION Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
						fclose(fpt); // 2013.10.29
						return FALSE;
					}
					REMAPPING_IO_NAME( chr_return , chr_return2 );
					POST_RUN_FUNCTION = _FIND_FROM_STRING( _K_F_IO , chr_return );
					if ( POST_RUN_FUNCTION == -1 ) {
						_IO_CIM_PRINTF( "POST_RUN_FUNCTION Can not Find => Will Go Success Status- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
					}
					else {
//						strcpy( POST_RUN_FUNCTION_STR , "" ); // 2008.04.04
						STR_MEM_MAKE_COPY( &POST_RUN_FUNCTION_STR , "" ); // 2008.04.04
						if ( ReadCnt > 2 ) {
							if ( !Get_Data_From_String_for_STRING( Buffer , 2 , chr_return , 255 , chr_return2 ) ) {
								_IO_CIM_PRINTF( "POST_RUN_FUNCTION Error - [%s]-[%d] - %s\n" , Filename , Line , chr_return );
								fclose(fpt); // 2013.10.29
								return FALSE;
							}
//							REMAPPING_IO_NAME( POST_RUN_FUNCTION_STR , chr_return2 ); // 2008.04.04
							REMAPPING_IO_NAME( chr_return , chr_return2 ); // 2008.04.04
							STR_MEM_MAKE_COPY( &POST_RUN_FUNCTION_STR , chr_return ); // 2008.04.04
						}
					}
				}
			}
			else if ( STRCMP_L( chr_return , "INVALID_FILE_OPERATION" ) ) { // 2013.10.29
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt);
						return FALSE;
					}
					//
					if      ( STRCMP_L( chr_return , "SUCCESS" ) ) {
						INVALID_FILE_OPERATION = SYS_SUCCESS;
					}
					else if ( STRCMP_L( chr_return , "ERROR" ) ) {
						INVALID_FILE_OPERATION = SYS_ERROR;
					}
					else if ( STRCMP_L( chr_return , "ABORT" ) ) {
						INVALID_FILE_OPERATION = SYS_ABORTED;
					}
					else {
						INVALID_FILE_OPERATION = -1;
					}
					//
					// 2014.01.10
					//
					if ( ReadCnt > 2 ) {
						if ( !Get_Data_From_String( Buffer , chr_return , 2 ) ) {
							_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
							fclose(fpt);
							return FALSE;
						}
						//
						INVALID_FILE_ALARM = atoi( chr_return );
						//
						if ( INVALID_FILE_ALARM > 9999 ) INVALID_FILE_ALARM = 0;
						//
					}
					//
				}
			}
			else if ( STRCMP_L( chr_return , "INVALID_MESSAGE_OPERATION" ) ) { // 2015.04.27
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt);
						return FALSE;
					}
					//
					if      ( STRCMP_L( chr_return , "TIME" ) ) {
						INVALID_MESSAGE_OPERATION = 0;
					}
					else if ( STRCMP_L( chr_return , "ERROR" ) ) {
						INVALID_MESSAGE_OPERATION = 1;
					}
					else if ( STRCMP_L( chr_return , "ABORT" ) ) {
						INVALID_MESSAGE_OPERATION = 2;
					}
					else {
						INVALID_MESSAGE_OPERATION = 0;
					}
					//
					// 2014.01.10
					//
					if ( ReadCnt > 2 ) {
						if ( !Get_Data_From_String( Buffer , chr_return , 2 ) ) {
							_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
							fclose(fpt);
							return FALSE;
						}
						//
						INVALID_MESSAGE_ALARM = atoi( chr_return );
						//
						if ( INVALID_MESSAGE_ALARM > 9999 ) INVALID_MESSAGE_ALARM = 0;
						//
					}
					//
				}
			}
			else if ( STRCMP_L( chr_return , "TIME_MESSAGE_OPERATION" ) ) { // 2015.04.27
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt);
						return FALSE;
					}
					//
					if      ( STRCMP_L( chr_return , "TIME" ) ) {
						TIME_MESSAGE_OPERATION = 0;
					}
					else if ( STRCMP_L( chr_return , "ERROR" ) ) {
						TIME_MESSAGE_OPERATION = 1;
					}
					else if ( STRCMP_L( chr_return , "ABORT" ) ) {
						TIME_MESSAGE_OPERATION = 2;
					}
					else {
						TIME_MESSAGE_OPERATION = 0;
					}
					//
					// 2014.01.10
					//
					if ( ReadCnt > 2 ) {
						if ( !Get_Data_From_String( Buffer , chr_return , 2 ) ) {
							_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
							fclose(fpt);
							return FALSE;
						}
						//
						TIME_MESSAGE_ALARM = atoi( chr_return );
						//
						if ( TIME_MESSAGE_ALARM > 9999 ) TIME_MESSAGE_ALARM = 0;
						//
					}
					//
				}
			}
			else if ( STRCMP_L( chr_return , "FILE_READ_MODE" ) ) { // 2015.07.20
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String( Buffer , chr_return , 1 ) ) {
						_IO_CIM_PRINTF( "Unknown Error 2 - [%s]-[%d]\n" , Filename , Line );
						fclose(fpt);
						return FALSE;
					}
					//
					if      ( STRCMP_L( chr_return , "FILE" ) ) {
						FILE_READ_MODE = 1;
					}
					else if ( STRCMP_L( chr_return , "SHMEM" ) ) {
						FILE_READ_MODE = 2;
					}
					else if ( STRCMP_L( chr_return , "AUTO" ) ) {
						FILE_READ_MODE = 0;
					}
					else {
						FILE_READ_MODE = 0;
					}
					//
				}
			}
			else if ( STRCMP_L( chr_return , "MAX_BUFFER_SIZE" ) ) { // 2017.08.20
				if ( ReadCnt > 1 ) {
					if ( !Get_Data_From_String_for_INT( Buffer , 1 , chr_return , 10000 , 1000000 , &MAX_BUFFER_SIZE ) ) {
						MAX_BUFFER_SIZE = 50000;
					}
					else {
						if ( MAX_BUFFER_SIZE > 999000 ) MAX_BUFFER_SIZE = 999000;
					}
				}
			}
			else {
				_IO_CIM_PRINTF( "Unknown Parameter Error- [%s]-[%d] - %s\n" , Filename , Line , chr_return );
//				return FALSE; // 2013.10.29
			}
		}
	}
	fclose(fpt);
	return TRUE;
}
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
BOOL SETUP_MAKE_FILE( char *Filename , char *SendFormat ) {
	HFILE hFile;
	int wbyte , l;
	if ( Filename == NULL ) return FALSE;

//	hFile = _lopen( Filename , OF_READ ); if ( hFile == -1 ) return FALSE;		// 2009.06.19
	//
	hFile = _lopen( Filename , OF_READ );	// 2009.06.19
	//
//	if ( hFile == -1 ) { // 2015.07.17
	if ( hFile <= -1 ) { // 2015.07.17
		_IO_CIM_PRINTF( "Recipe File Open Error [%s]-[%d]\n", Filename, GetLastError() );
		return FALSE;	
	}

	l = strlen( SendFormat );
	//
	wbyte = _lread( hFile , SendFormat + l , ( 50000 - l ) );
	//
	SendFormat[wbyte+l] = 0x00;
	_lclose( hFile );
	return TRUE;
}
//
void FileNameGenerate_From_String( char *Filename , char *ReadName , char *buffer ) { // 2008.06.12
	int i , l;
	//----------------------------------------------------------------
	if ( ( ReadName[0] != '%' ) || ( ReadName[1] != 's' ) ) return;
	if ( ( ReadName[2] != '/' ) && ( ReadName[2] != '\\' ) ) return;
	//----------------------------------------------------------------
	l = strlen( Filename );
	for ( i = l - 1 ; i >= 0 ; i-- ) {
		if ( ( Filename[i] == '/' ) || ( Filename[i] == '\\' ) ) break;
	}
	//----------------------------------------------------------------
	if ( i < 0 ) {
		strcpy( buffer , ReadName + 3 );
		strcpy( ReadName , buffer );
	}
	else {
		strcpy( buffer , ReadName + 2 );
		strcpy( ReadName , Filename );		ReadName[i] = 0;
		strcat( ReadName , buffer );
	}
}
//------------------------------------------------------------------------------------------
void _UTIL_Cut_Space_Data( char *source ) {
	int i , l;
	//
	l = strlen( source );
	//
	for ( i = ( l - 1 ) ; i >= 0 ; i-- ) {
		if ( ( source[i] == 9 ) || ( source[i] == ' ' ) ) {
			source[i] = 0;
		}
		else {
			break;
		}
	}
}
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
int SETUP_MESSAGE( int LoopMode , char *Filename , int Type_Mode , int WfrID , int CassChamber , int Finger , int PutTime , char *NextPM , int Mindex , char *lotstr , int upstepcnt ) {
	HFILE hFile;
	HFILE hFileM;
	HFILE hFileS;
	HFILE hFileD;
	int ml , mffind = FALSE , fres , tres;
//	char mffilename[256]; // 2015.05.05
//	char SendBuffer[256]; // 2015.05.05
	char mffilename[512]; // 2015.05.05
	char SendBuffer[1024]; // 2015.05.05
	char FloatBuffer[256];
	char *FileBuffer;
	BOOL StepEndFind;
//	int i,kill,Step_Count,Step_CountSub,Max_Step_Count;// 2007.10.25
	int i,kill,Step_Count,Step_CountSub;// 2007.10.25
	
	char  cc_return;
	int   ii_return;
	float ff_return;
	double dd_return; // 2008.05.30
	char  str_return[1025];
	char  name_return[1025];
	char  type_return;

	int   FileName_ShMem; // 2015.07.20
	int   FileName_Pnt; // 2015.07.20

	if ( Filename == NULL ) return 1;
	//
	// 2015.07.02
	//
	FileName_ShMem = 0;
	FileName_Pnt = 0;
	//
	if ( Filename[0] == '+' ) {
		if ( ( Filename[1] == '\\' ) || ( Filename[1] == '/' ) ) {
			FileName_ShMem = 1;
			FileName_Pnt = 2;
		}
	}
	//
	//
//	LotData_Change_to( FALSE , Filename ); // 2012.08.23 // 2015.07.20
	LotData_Change_to( FALSE , Filename + FileName_Pnt ); // 2012.08.23 // 2015.07.20
	//
//	if ( strcmp( Filename , "?" ) == 0 ) { // 2003.05.22
//	if ( Filename[0] == '?' ) { // 2003.05.22 // 2007.03.07 // 2015.07.20
	if ( ( Filename[0] == '?' ) || ( Filename[FileName_Pnt] == '?' ) ) { // 2003.05.22 // 2007.03.07 // 2015.07.20
		if ( LoopMode != 1 ) { // 2006.04.14
			// 2007.04.06 (Move Here)
			if ( Mindex >= 0 ) { // 2005.10.21
				sprintf( SendBuffer , "%c$X%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Mindex , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 2;
			}
			//
//			sprintf( SendBuffer , "%c$I%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , WfrID , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) return 3; // 2007.04.06
			//---- 2002.02.20
			ii_return = 0;
			kill = strlen( Filename );
			for ( i = kill - 1 ; i >= 0 ; i-- ) {
				if ( Filename[i] == '?' ) {
					break;
				}
				else {
					ii_return = 1;
				}
			}
			i++;
			//--------------------------
			if ( ii_return == 0 ) { // 2007.03.07
				sprintf( SendBuffer , "%c$R%c*%c"  , SEP_LEFT_BLACKET , SEP_NAME_DATA , SEP_RIGHT_BLACKET );				if ( !Global_Buffer_strcat( SendBuffer ) ) return 4;
			}
			else {
				sprintf( SendBuffer , "%c$R%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Filename + i , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 5;
			}
			//
			if ( LoopMode == 0 ) { // 2007.04.06
				sprintf( SendBuffer , "%c$I%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , WfrID , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) return 6; // 2007.04.06
				//
				sprintf( SendBuffer , "%c$M%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Type_Mode , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 7;
				sprintf( SendBuffer , "%c$C%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , CassChamber , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 8;
				sprintf( SendBuffer , "%c$F%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Finger , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) return 9;
				sprintf( SendBuffer , "%c$P%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , PutTime , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) return 10;
				if ( strlen( NextPM ) > 0 ) { // 2002.07.16
					sprintf( SendBuffer , "%c$N%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , NextPM , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 11;
				}
	/*
	// 2007.04.06
				if ( Mindex >= 0 ) { // 2005.10.21
					sprintf( SendBuffer , "%c$X%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Mindex , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 12;
				}
	*/
				if ( strlen( lotstr ) > 0 ) { // 2006.04.06
					sprintf( SendBuffer , "%c$L%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , lotstr , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 13;
				}
			}
		}
		return 0; // 2003.11.05 // Bug Update
	}
	else {
		if ( LoopMode != 1 ) { // 2006.04.14
			// 2007.04.06 (Move Here)
			if ( Mindex >= 0 ) { // 2005.10.21
				sprintf( SendBuffer , "%c$X%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Mindex , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 14;
			}
			//
//			sprintf( SendBuffer , "%c$I%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , WfrID , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) { return 15; } // 2007.04.06
			/*
			// 2012.07.05
			//---- 2002.02.20
			kill = strlen( Filename );
			for ( i = kill - 1 ; i >= 0 ; i-- ) {
				if ( Filename[i] == '$' ) break;
			}
			i++;
			//--------------------------
			sprintf( SendBuffer , "%c$R%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Filename + i , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) { return 16; }
			*/
//			sprintf( SendBuffer , "%c$R%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Filename , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) { return 16; } // 2012.07.05 // 2015.07.20
			sprintf( SendBuffer , "%c$R%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Filename + FileName_Pnt , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) { return 16; } // 2012.07.05 // 2015.07.20
			//
			if ( LoopMode == 0 ) { // 2007.04.06
				sprintf( SendBuffer , "%c$I%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , WfrID , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) { return 17; } // 2007.04.06
				//
				sprintf( SendBuffer , "%c$M%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Type_Mode , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) { return 18; }
				sprintf( SendBuffer , "%c$C%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , CassChamber , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) { return 19; }
				sprintf( SendBuffer , "%c$F%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Finger , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) { return 20; }
				sprintf( SendBuffer , "%c$P%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , PutTime , SEP_RIGHT_BLACKET );		if ( !Global_Buffer_strcat( SendBuffer ) ) { return 21; }
				if ( strlen( NextPM ) > 0 ) { // 2002.07.16
					sprintf( SendBuffer , "%c$N%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , NextPM , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 22;
				}
	/*
	// 2007.04.06
				if ( Mindex >= 0 ) { // 2005.10.21
					sprintf( SendBuffer , "%c$X%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Mindex , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 23;
				}
	*/
				if ( strlen( lotstr ) > 0 ) { // 2006.04.06
					sprintf( SendBuffer , "%c$L%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , lotstr , SEP_RIGHT_BLACKET );	if ( !Global_Buffer_strcat( SendBuffer ) ) return 24;
				}
			}
		}
		//
		// 2015.07.20
		//
		if ( FileName_ShMem == 1 ) {
			//
			if ( !FILE_SHMEM_INIT ) {
				//
				i = _FILE2SM_INTERFACE_INITIALIZING( GET_SYSTEM_NAME() );	// 0:OK 1:AlreadyInit 2:CreateMutexFail 3:WaitObjectFail 4:OpenFileFail 5:CreateFileFail 6:MultiMutexFail
				//
				if ( i != 0 ) {
					//
					_IO_CIM_PRINTF( "_FILE2SM_INTERFACE_INITIALIZING Error2 (%d) - FILE=%s\n" , i , Filename );
					//
				}
				else {
					FILE_SHMEM_INIT = TRUE;
				}
			}
			//
			read_mode_file_set( 2 ); // 2015.07.20
			//
		}
		else {
			//
			read_mode_file_set( FILE_READ_MODE );
			//
		}
		//
		for ( i = 0 ; i < 3 ; i++ ) {
			//
//			hFile = _lopen( Filename , OF_READ ); // 2015.07.17
//			hFile = read_open_file( Filename ); // 2015.07.17 // 2015.07.20
			hFile = read_open_file( Filename + FileName_Pnt ); // 2015.07.17 // 2015.07.20
			//
//			if ( hFile != -1 ) break; // 2015.07.17
			if ( hFile >= 0 ) break; // 2015.07.17
			//
//			_sleep(100); // 2013.03.28
			Sleep(100); // 2013.03.28
		}
//		if ( hFile == -1 ) return 25; // 2015.07.17
		if ( hFile < 0 ) return 25; // 2015.07.17

	}
	// INDICATOR
	if ( !read_form_name_check( hFile , STR_MEM_GET_STR( FILE_ID )   ) ) { read_close_file( hFile ); return 26; }
	if ( !read_form_name_check( hFile , STR_MEM_GET_STR( RECIPE_ID ) ) ) { read_close_file( hFile ); return 27; }
//	Max_Step_Count = MAX_STEP_COUNT; // 2007.10.25
	// HEADER
	if ( !read_form_name_check( hFile , "__&&HEADER_BEGIN&&__" ) ) { read_close_file( hFile ); return 28; }
	if ( !read_form_until_Matched( hFile , "NumberOfSteps"        , INT_FORMAT    , &cc_return , &Step_Count , &ff_return , str_return ) ) { read_close_file( hFile ); return 29; }
	//========================================================================================================
	// 2006.04.14
	//========================================================================================================
	if ( ( LoopMode != 0 ) && APPEND_FILE_STEP_CUT_WITH_MAIN ) {
		if ( Step_Count > upstepcnt ) {
			Step_Count = upstepcnt;
		}
	}
	//========================================================================================================
	if ( !read_form_name_until( hFile , "__&&HEADER_END&&__" ) ) { read_close_file( hFile ); return 30; }

	sprintf( SendBuffer , "%c$T%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , Step_Count , SEP_RIGHT_BLACKET );
	if ( !Global_Buffer_strcat( SendBuffer ) ) {
		read_close_file( hFile );
		return 31;
	}

	sprintf( SendBuffer , "%c$S%c0%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , SEP_RIGHT_BLACKET );
	if ( !Global_Buffer_strcat( SendBuffer ) ) {
		read_close_file( hFile );
		return 32;
	}

	hFileD = -1;

	for ( ml = 0 ; ml < 2 ; ml++ ) { // 2006.04.14

		if ( !read_form( hFile , "__&&STEP0_BEGIN&&__"   , NAME_FORMAT   , &cc_return , &ii_return           , &ff_return , str_return        ) ) {
			read_close_file( hFile );
			if ( hFileD != -1 ) read_close_file( hFileD );
			return 33;
		}

		//StepRead
		StepEndFind = FALSE;
//		kill = 0; // 2016.04.11
		while( !StepEndFind ) {
//			kill++; // 2016.04.11
			if ( !read_return_form2( hFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
				read_close_file( hFile );
				if ( hFileD != -1 ) read_close_file( hFileD );
				return 34;
			}
			switch ( type_return ) {
				case NAME_FORMAT :	if ( strcmp( name_return , "__&&STEP0_END&&__" ) == 0 ) StepEndFind = TRUE;
									break;
				case CHAR_FORMAT :
				case INT_FORMAT	 :
									if ( LoopMode != 1 ) tres = Find_Tune_And_Redefine_CharInteger_OCR( Mindex , name_return , 0 , &cc_return , &ii_return , type_return == INT_FORMAT );
									else                 tres = 0;
									//
									if ( tres != -1 ) { // 2011.05.18
										sprintf( SendBuffer , "%c%s%c%d%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , ( type_return == INT_FORMAT ) ? ii_return : cc_return , SEP_RIGHT_BLACKET );
										if ( !Global_Buffer_strcat( SendBuffer ) ) {
											read_close_file( hFile );
											if ( hFileD >= 0 ) read_close_file( hFileD );
											return 35;
										}
									}
									break;
				case FLOAT_FORMAT:
				case DOUBLE_FORMAT:
									if ( LoopMode != 1 ) tres = Find_Tune_And_Redefine_FloatDouble_OCR( Mindex , name_return , 0 , &ff_return , &dd_return , type_return == DOUBLE_FORMAT );
									else                 tres = 0;
									//
									if ( tres != -1 ) { // 2011.05.18
										Optimize_FloatDouble_Buffer( FloatBuffer , ff_return , dd_return , type_return == DOUBLE_FORMAT );
										sprintf( SendBuffer , "%c%s%c%s%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , FloatBuffer , SEP_RIGHT_BLACKET );
										if ( !Global_Buffer_strcat( SendBuffer ) ) {
											read_close_file( hFile );
											if ( hFileD >= 0 ) read_close_file( hFileD );
											return 36;
										}
									}
									break;
				case STRING_FORMAT:	if ( LoopMode != 1 ) tres = Find_Tune_And_Redefine_String_OCR( Mindex , name_return , 0 , str_return );
									else                 tres = 0;
									//
									if ( tres != -1 ) { // 2011.05.18
										sprintf( SendBuffer , "%c%s%c%s%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , str_return , SEP_RIGHT_BLACKET );
										if ( !Global_Buffer_strcat( SendBuffer ) ) {
											read_close_file( hFile );
											if ( hFileD >= 0 ) read_close_file( hFileD );
											return 37;
										}
									}
									break;
				case FILENAME_FORMAT:
									//
									_UTIL_Cut_Space_Data( str_return ); // 2010.11.02
									if ( STRCMP_L( "" , str_return ) ) break; // 2010.11.02
									//
									if ( ( LoopMode == 0 ) && ( ml == 0 ) && STEP0_FILE_MIXING ) { // 2006.04.14
										//
//										FileNameGenerate_From_String( Filename , str_return , SendBuffer ); // 2008.06.12 // 2015.07.20
										FileNameGenerate_From_String( Filename + FileName_Pnt , str_return , SendBuffer ); // 2008.06.12 // 2015.07.20
										//
//										if ( !STRCMP_L( Filename , str_return ) ) { // 2015.07.20
										if ( !STRCMP_L( Filename + FileName_Pnt , str_return ) ) { // 2015.07.20
											if ( strlen( str_return ) > 0 ) { // 2007.10.04
												if ( str_return[0] != '?' ) { // 2007.10.04
													mffind = TRUE;
													strcpy( mffilename , str_return );
												}
											}
										}
									}
									else {
										sprintf( SendBuffer , "%c$F%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , name_return , SEP_RIGHT_BLACKET );
										if ( !Global_Buffer_strcat( SendBuffer ) ) {
											read_close_file( hFile );
											if ( hFileD >= 0 ) read_close_file( hFileD );
											return 38;
										}
										if ( ( LoopMode != 1 ) && ( str_return[0] != 0 ) ) { // 2006.04.14
											//
//											FileNameGenerate_From_String( Filename , str_return , SendBuffer ); // 2008.06.12 // 2015.07.20
											FileNameGenerate_From_String( Filename + FileName_Pnt , str_return , SendBuffer ); // 2008.06.12 // 2015.07.20
											//
//											if ( !STRCMP_L( Filename , str_return ) ) { // 2006.04.14 // 2015.07.20
											if ( !STRCMP_L( Filename + FileName_Pnt , str_return ) ) { // 2006.04.14 // 2015.07.20
												if ( FUNCTION_FILE_ADDRESS >= 0 ) {
													FileBuffer = calloc( 50000 + 1 , sizeof( char ) );
													if ( FileBuffer == NULL ) {
														// _IO_CIM_PRINTF( "RcpHandler Sub Buffer 2 Init Error - Less Memory\n" );		// 2009.06.19
														_IO_CIM_PRINTF( "RcpHandler Sub Buffer 2 Init Error - Less Memory [%d]\n", GetLastError() );	// 2009.06.19
														read_close_file( hFile );
														if ( hFileD >= 0 ) read_close_file( hFileD );
														return 39;
													}
													if ( Function_String_Send_Check( str_return ) ) {
														sprintf( FileBuffer , "RCPFileSave1.00%4d%s" , strlen( str_return ) , str_return );
														if ( !SETUP_MAKE_FILE( str_return , FileBuffer ) ) {
															free( FileBuffer );
															read_close_file( hFile );
															if ( hFileD >= 0 ) read_close_file( hFileD );
															return 40;
														}
														if ( _dRUN_FUNCTION( FUNCTION_FILE_ADDRESS , FileBuffer ) == SYS_ABORTED ) {
															_IO_CIM_PRINTF( "RcpHandler Sub File Setting Error - Return SYS_ABORTED\n" );
														}
													}
													free( FileBuffer );
													sprintf( SendBuffer , "%c%s%c%s%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , str_return , SEP_RIGHT_BLACKET );
													if ( !Global_Buffer_strcat( SendBuffer ) ) {
														read_close_file( hFile );
														if ( hFileD >= 0 ) read_close_file( hFileD );
														return 41;
													}
												}
												else {
//													if ( !SETUP_MESSAGE( 1 , str_return , Type_Mode , WfrID , CassChamber , Finger , PutTime , "" , 0 , "" , Step_Count ) ) { // 2007.06.01
													fres = SETUP_MESSAGE( 1 , str_return , Type_Mode , WfrID , CassChamber , Finger , PutTime , "" , Mindex , "" , Step_Count );
													if ( fres != 0 ) { // 2007.06.01
														read_close_file( hFile );
														if ( hFileD >= 0 ) read_close_file( hFileD );
														//
//														_IO_CIM_PRINTF( "RcpHandler Sub File Reading Error 1 - [%s][%d]\n" , str_return , fres ); // 2015.02.13
														_IO_CIM_PRINTF( "RcpHandler Sub File Reading Error 1 - [%s][%d] - [%ld]\n" , str_return , fres , GetLastError() ); // 2015.02.13
//														return 42;
														return 500 + fres;
													}
												}
											}
										}
										sprintf( SendBuffer , "%c$f%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , name_return , SEP_RIGHT_BLACKET );
										if ( !Global_Buffer_strcat( SendBuffer ) ) {
											read_close_file( hFile );
											if ( hFileD >= 0 ) read_close_file( hFileD );
											return 43;
										}
									}
									break;
				//==============================================================================================
				case NAME_X_FORMAT :
				case CHAR_X_FORMAT :
				case INT_X_FORMAT :
				case FLOAT_X_FORMAT :
				case DOUBLE_X_FORMAT :
				case STRING_X_FORMAT :
				case FILENAME_X_FORMAT :
									break;
				//==============================================================================================
			}
//			if ( kill >= 1000 )	{ // 2016.04.11
//				read_close_file( hFile );
//				if ( hFileD >= 0 ) read_close_file( hFileD );
//				return 44;
//			}
		}
		if ( ml == 0 ) { // 2006.04.14
			if ( mffind ) {
				hFileD = hFile;
				hFileM = hFile;
				//------------------------------------------------------------------------------------------------
				//
//				hFile = _lopen( mffilename , OF_READ ); // 2015.07.17
				hFile = read_open_file( mffilename ); // 2015.07.17
				//
//				if ( hFile == -1 ) { // 2015.07.17
				if ( hFile < 0 ) { // 2015.07.17

//					if ( hFileD != -1 ) _lclose( hFileD ); // 2015.07.17
					if ( hFileD >= 0 ) read_close_file( hFileD ); // 2015.07.17

					return 45;
				}
				if ( !read_form_until_Matched( hFile , "NumberOfSteps" , INT_FORMAT , &cc_return , &Step_CountSub , &ff_return , str_return ) ) {
//					_lclose( hFile ); // 2015.07.17
					read_close_file( hFile ); // 2015.07.17

//					if ( hFileD != -1 ) _lclose( hFileD ); // 2015.07.17
					if ( hFileD >= 0 ) read_close_file( hFileD ); // 2015.07.17

					return 46;
				}
				if ( !read_form_name_until( hFile , "__&&HEADER_END&&__" ) ) {
//					_lclose( hFile ); // 2015.07.17
					read_close_file( hFile ); // 2015.07.17

//					if ( hFileD != -1 ) _lclose( hFileD ); // 2015.07.17
					if ( hFileD >= 0 ) read_close_file( hFileD ); // 2015.07.17

					return 47;
				}
			}
			else {
				break;
			}
		}
		else {
			hFileD = hFile;
			hFileS = hFile;
			hFile = hFileM;
		}
	}
	//========================================================================================================================================
	// 2007.06.07
	//========================================================================================================================================
	if ( LoopMode != 1 ) {
		if ( !Find_Tune_And_Append_OCR( Mindex , 0 , SendBuffer ) ) {
			read_close_file( hFile );
			if ( hFileD >= 0 ) read_close_file( hFileD );
			return 48;
		}
	}
	//========================================================================================================================================
	sprintf( SendBuffer , "%c$E%c0%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , SEP_RIGHT_BLACKET );
	if ( !Global_Buffer_strcat( SendBuffer ) ) {
		read_close_file( hFile );
		if ( hFileD >= 0 ) read_close_file( hFileD );
		return 49;
	}

	// STEP

//	for ( i = 0 ; ( i < Step_Count ) && ( i < Max_Step_Count ) ; i++ ) { // 2007.10.25
	for ( i = 0 ; ( i < Step_Count ) && ( i < MAX_STEP_COUNT ) ; i++ ) { // 2007.10.25

		sprintf( SendBuffer , "%c$S%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , i+1 , SEP_RIGHT_BLACKET );

		if ( !Global_Buffer_strcat( SendBuffer ) ) {
			read_close_file( hFile );
			if ( hFileD >= 0 ) read_close_file( hFileD );
			return 50;
		}

		for ( ml = 0 ; ml < 2 ; ml++ ) { // 2006.04.14

			if ( !read_form( hFile , "__&&STEP_BEGIN&&__"   , NAME_FORMAT   , &cc_return , &ii_return , &ff_return , str_return ) ) {
				read_close_file( hFile );
				if ( hFileD >= 0 ) read_close_file( hFileD );
				return 51;
			}

			// StepRead
			StepEndFind = FALSE;
//			kill = 0; // 2016.04.11
			while( !StepEndFind ) {
				if ( !read_return_form2( hFile , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
					read_close_file( hFile );
					if ( hFileD >= 0 ) read_close_file( hFileD );
					return 52;
				}
				switch (type_return) {
					case NAME_FORMAT :	if ( strcmp( name_return , "__&&STEP_END&&__" ) == 0 ) StepEndFind = TRUE;
										break;
					case CHAR_FORMAT :
					case INT_FORMAT	 :
										if ( LoopMode != 1 ) tres = Find_Tune_And_Redefine_CharInteger_OCR( Mindex , name_return , i + 1 , &cc_return , &ii_return , type_return == INT_FORMAT );
										else                 tres = 0;
										//
										if ( tres != -1 ) { // 2011.05.18
											sprintf( SendBuffer , "%c%s%c%d%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , ( type_return == INT_FORMAT ) ? ii_return : cc_return , SEP_RIGHT_BLACKET );
											if ( !Global_Buffer_strcat( SendBuffer ) ) {
												read_close_file( hFile );
												if ( hFileD >= 0 ) read_close_file( hFileD );
												return 53;
											}
										}
										break;
					case FLOAT_FORMAT:
					case DOUBLE_FORMAT:
										if ( LoopMode != 1 ) tres = Find_Tune_And_Redefine_FloatDouble_OCR( Mindex , name_return , i + 1 , &ff_return , &dd_return , type_return == DOUBLE_FORMAT );
										else                 tres = 0;
										//
										if ( tres != -1 ) { // 2011.05.18
											Optimize_FloatDouble_Buffer( FloatBuffer , ff_return , dd_return , type_return == DOUBLE_FORMAT );
											sprintf( SendBuffer , "%c%s%c%s%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , FloatBuffer , SEP_RIGHT_BLACKET );
											if ( !Global_Buffer_strcat( SendBuffer ) ) {
												read_close_file( hFile );
												if ( hFileD >= 0 ) read_close_file( hFileD );
												return 54;
											}
										}
										break;
					case STRING_FORMAT:	if ( LoopMode != 1 ) tres = Find_Tune_And_Redefine_String_OCR( Mindex , name_return , i + 1 , str_return );
										else                 tres = 0;
										//
										if ( tres != -1 ) { // 2011.05.18
											sprintf( SendBuffer , "%c%s%c%s%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , str_return , SEP_RIGHT_BLACKET );
											if ( !Global_Buffer_strcat( SendBuffer ) ) {
												read_close_file( hFile );
												if ( hFileD >= 0 ) read_close_file( hFileD );
												return 55;
											}
										}
										break;
					case FILENAME_FORMAT:
										//
										_UTIL_Cut_Space_Data( str_return ); // 2010.11.02
										if ( STRCMP_L( "" , str_return ) ) break; // 2010.11.02
										//
										sprintf( SendBuffer , "%c$F%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , name_return , SEP_RIGHT_BLACKET );
										if ( !Global_Buffer_strcat( SendBuffer ) ) {
											read_close_file( hFile );
											if ( hFileD >= 0 ) read_close_file( hFileD );
											return 56;
										}
										if ( ( LoopMode != 1 ) && ( str_return[0] != 0 ) ) { // 2006.04.14
											//
//											FileNameGenerate_From_String( Filename , str_return , SendBuffer ); // 2008.06.12 // 2015.07.20
											FileNameGenerate_From_String( Filename + FileName_Pnt , str_return , SendBuffer ); // 2008.06.12 // 2015.07.20
											//
//											if ( !STRCMP_L( Filename , str_return ) ) { // 2006.04.14 // 2015.07.20
											if ( !STRCMP_L( Filename + FileName_Pnt , str_return ) ) { // 2006.04.14 // 2015.07.20
												if ( FUNCTION_FILE_ADDRESS >= 0 ) {
													FileBuffer = calloc( 50000 + 1 , sizeof( char ) );
													if ( FileBuffer == NULL ) {
														_IO_CIM_PRINTF( "RcpHandler Sub Buffer 2 Init Error - Less Memory\n" );
														read_close_file( hFile );
														if ( hFileD >= 0 ) read_close_file( hFileD );
														return 57;
													}
													if ( Function_String_Send_Check( str_return ) ) {
														sprintf( FileBuffer , "RCPFileSave1.00%4d%s" , strlen( str_return ) , str_return );
														if ( !SETUP_MAKE_FILE( str_return , FileBuffer ) ) {
															free( FileBuffer );
															read_close_file( hFile );
															if ( hFileD >= 0 ) read_close_file( hFileD );
															return 58;
														}
														if ( _dRUN_FUNCTION( FUNCTION_FILE_ADDRESS , FileBuffer ) == SYS_ABORTED ) {
															_IO_CIM_PRINTF( "RcpHandler Sub File Setting Error - Return SYS_ABORTED\n" );
														}
													}
													free( FileBuffer );
													sprintf( SendBuffer , "%c%s%c%s%c" , SEP_LEFT_BLACKET , name_return , SEP_NAME_DATA , str_return , SEP_RIGHT_BLACKET );
													if ( !Global_Buffer_strcat( SendBuffer ) ) {
														read_close_file( hFile );
														if ( hFileD >= 0 ) read_close_file( hFileD );
														return 59;
													}
												}
												else {
//													if ( !SETUP_MESSAGE( 1 , str_return , Type_Mode , WfrID , CassChamber , Finger , PutTime , "" , 0 , "" , Step_Count ) ) { // 2007.06.01
													fres = SETUP_MESSAGE( 1 , str_return , Type_Mode , WfrID , CassChamber , Finger , PutTime , "" , Mindex , "" , Step_Count );
													if ( fres != 0 ) { // 2007.06.01
														read_close_file( hFile );
														if ( hFileD >= 0 ) read_close_file( hFileD );
														//
//														_IO_CIM_PRINTF( "RcpHandler Sub File Reading Error 2 - [%d][%s][%d]\n" , i , str_return , fres ); // 2015.02.13
														_IO_CIM_PRINTF( "RcpHandler Sub File Reading Error 2 - [%d][%s][%d] - [%ld]\n" , i , str_return , fres , GetLastError() ); // 2015.02.13
//														return 60;
														return 1000 + fres;
													}
												}
											}
										}
										sprintf( SendBuffer , "%c$f%c%s%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , name_return , SEP_RIGHT_BLACKET );					if ( !Global_Buffer_strcat( SendBuffer ) ) { read_close_file( hFile ); return 61; }
										break;
					//==============================================================================================
					case NAME_X_FORMAT :
					case CHAR_X_FORMAT :
					case INT_X_FORMAT :
					case FLOAT_X_FORMAT :
					case DOUBLE_X_FORMAT :
					case STRING_X_FORMAT :
					case FILENAME_X_FORMAT :
										break;
					//==============================================================================================
				}
//				if ( kill >= 1000 )	{ // 2016.04.11
//					read_close_file( hFile );
//					if ( hFileD >= 0 ) read_close_file( hFileD );
//					return 62;
//				}
			}
			//
			if ( ml == 0 ) { // 2006.04.14
				if ( mffind ) {
					if ( i >= Step_CountSub ) break;
					hFileD = hFile;
					hFileM = hFile;
					hFile = hFileS;
				}
				else {
					break;
				}
			}
			else {
				hFileD = hFile;
				hFileS = hFile;
				hFile = hFileM;
			}
			//
		}
		//========================================================================================================================================
		// 2007.06.07
		//========================================================================================================================================
		if ( LoopMode != 1 ) {
			if ( !Find_Tune_And_Append_OCR( Mindex , i + 1 , SendBuffer ) ) {
				read_close_file( hFile );
				if ( hFileD >= 0 ) read_close_file( hFileD );
				return 63;
			}
		}
		//========================================================================================================================================
		sprintf( SendBuffer , "%c$E%c%d%c" , SEP_LEFT_BLACKET , SEP_NAME_DATA , i+1 , SEP_RIGHT_BLACKET );
		if ( !Global_Buffer_strcat( SendBuffer ) ) {
			read_close_file( hFile );
			if ( hFileD >= 0 ) read_close_file( hFileD );
			return 64;
		}
	}
	read_close_file( hFile );
	if ( hFileD >= 0 ) read_close_file( hFileD );
	return 0;
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
int	Recipe_Item_Modify( char *RecipeFileName ) {
	char  IMSI_FILE_FOR_STEP_ITEM_CHANGE[256];
	HFILE hFileR;
	HFILE hFileS;
	int   i , t , tf;
	int   Step_Count = 0;
	char  cc_return;
	int   ii_return;
	float ff_return;
	double dd_return; // 2008.05.30
	char  str_return[256];
	char  name_return[256];
	char  type_return;

	if ( RecipeFileName == NULL ) return 1;
	//
	hFileR = _lopen( RecipeFileName , OF_READ );
	if ( hFileR == -1 ) return 2;
	//
	_IO_GET_SYSTEM_FOLDER( str_return , 255 );
	sprintf( IMSI_FILE_FOR_STEP_ITEM_CHANGE , "%s\\$$remove_%s_ric$$" , str_return , PROGRAM_FUNCTION_READ() );
	remove( IMSI_FILE_FOR_STEP_ITEM_CHANGE );
	//
	hFileS = _lcreat( IMSI_FILE_FOR_STEP_ITEM_CHANGE , 0 );
	if ( hFileS == -1 ) {
		_lclose( hFileR );
		return 3;
	}
	//
	//==============================================================================================================================
	//==============================================================================================================================
	tf = 0;
	while( TRUE ) {
		if ( !read_return_form2( hFileR , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
			_lclose( hFileR );
			_lclose( hFileS );
			remove( IMSI_FILE_FOR_STEP_ITEM_CHANGE );
			return 4;
		}
		//====================================================================================================
		if      ( ( type_return == INT_FORMAT ) && ( strcmp( name_return , "NumberOfSteps" ) == 0 ) ) {
			if ( tf == 0 ) Step_Count = ii_return;
		}
		else if ( ( type_return == INT_FORMAT ) && ( strcmp( name_return , "TotalOfSteps" ) == 0 ) ) {
			Step_Count = ii_return;
			tf = 1;
		}
		else if ( ( type_return == NAME_FORMAT ) && ( strcmp( name_return , "__&&HEADER_END&&__" ) == 0 ) ) {
			tf = 2;
		}
		switch( type_return ) {
		case NAME_FORMAT		:	write_nam_form( hFileS , name_return );	break;
		case CHAR_FORMAT		:	write_chr_form( hFileS , name_return , cc_return );	break;
		case INT_FORMAT			:	write_int_form( hFileS , name_return , ii_return );	break;
		case FLOAT_FORMAT		:	write_flt_form( hFileS , name_return , ff_return );	break;
		case DOUBLE_FORMAT		:	write_dbl_form( hFileS , name_return , dd_return );	break;
		case FILENAME_FORMAT	:	write_filename_form( hFileS , name_return , str_return );	break;
		case STRING_FORMAT		:	write_str_form( hFileS , name_return , str_return );	break;
		}
		if ( tf == 2 ) break;
	}
	//==============================================================================================================================
	//==============================================================================================================================
	tf = 0;
	while( TRUE ) {
		if ( !read_return_form2( hFileR , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
			_lclose( hFileR );
			_lclose( hFileS );
			remove( IMSI_FILE_FOR_STEP_ITEM_CHANGE );
			return 5;
		}
		//====================================================================================================
		if ( ( type_return == NAME_FORMAT ) && ( strcmp( name_return , "__&&STEP0_BEGIN&&__" ) == 0 ) ) {
			tf = 1;
		}
		if ( ( type_return == NAME_FORMAT ) && ( strcmp( name_return , "__&&STEP0_END&&__" ) == 0 ) ) {
			tf = 2;
		}
		else {
			//---------------
			if ( tf == 0 ) {
				_lclose( hFileR );
				_lclose( hFileS );
				remove( IMSI_FILE_FOR_STEP_ITEM_CHANGE );
				return 6;
			}
			//---------------
			for ( t = 0 ; t < UPFILT_COUNT ; t++ ) {
				if ( UPFILT_STEP[t] != 0 ) continue;
				if ( UPFILT_MODE[t] == -1 ) continue;
				//
				if ( STRCMP_L( UPFILT_NAME[t] , name_return ) ) {
					switch( type_return ) {
					case CHAR_FORMAT		:
					case CHAR_X_FORMAT		:
												cc_return = atoi( UPFILT_DATA[t] );	break;
					case INT_FORMAT			:
					case INT_X_FORMAT		:
												ii_return = atoi( UPFILT_DATA[t] );	break;
					case FLOAT_FORMAT		:
					case FLOAT_X_FORMAT		:
												ff_return = (float) atof( UPFILT_DATA[t] );	break;
					case DOUBLE_FORMAT		:
					case DOUBLE_X_FORMAT	:
												dd_return = atof( UPFILT_DATA[t] );	break;
					case FILENAME_FORMAT	:
					case FILENAME_X_FORMAT	:
					case STRING_FORMAT		:
					case STRING_X_FORMAT	:
												strcpy( str_return , UPFILT_DATA[t] );	break;
					}
					//
					break;
					//
				}
			}
			//---------------
			if ( t < UPFILT_COUNT ) { // 2013.03.28
				if ( UPFILT_MODE[t] == 3 ) {
					UPFILT_MODE[t] = 0;
					continue;
				}
				UPFILT_MODE[t] = 0;
			}
			//---------------
		}
		//====================================================================================================
		switch( type_return ) {
		case NAME_FORMAT		:	write_nam_form( hFileS , name_return );	break;
		case CHAR_FORMAT		:	write_chr_form( hFileS , name_return , cc_return );	break;
		case INT_FORMAT			:	write_int_form( hFileS , name_return , ii_return );	break;
		case FLOAT_FORMAT		:	write_flt_form( hFileS , name_return , ff_return );	break;
		case DOUBLE_FORMAT		:	write_dbl_form( hFileS , name_return , dd_return );	break;
		case FILENAME_FORMAT	:	write_filename_form( hFileS , name_return , str_return );	break;
		case STRING_FORMAT		:	write_str_form( hFileS , name_return , str_return );	break;
		//================================================================================================
		case NAME_X_FORMAT		:	write_nam_x_form( hFileS , name_return );	break;
		case CHAR_X_FORMAT		:	write_chr_x_form( hFileS , name_return , cc_return );	break;
		case INT_X_FORMAT		:	write_int_x_form( hFileS , name_return , ii_return );	break;
		case FLOAT_X_FORMAT		:	write_flt_x_form( hFileS , name_return , ff_return );	break;
		case DOUBLE_X_FORMAT	:	write_dbl_x_form( hFileS , name_return , dd_return );	break;
		case FILENAME_X_FORMAT	:	write_filename_x_form( hFileS , name_return , str_return );	break;
		case STRING_X_FORMAT	:	write_str_x_form( hFileS , name_return , str_return );	break;
		//====================================================================================================
		}
		//====================================================================================================
		if ( tf == 2 ) {
			//
			for ( t = 0 ; t < UPFILT_COUNT ; t++ ) { // 2013.03.28
				if ( UPFILT_STEP[t] != 0 ) continue;
				if ( ( UPFILT_MODE[t] != 1 ) && ( UPFILT_MODE[t] != 2 ) ) continue;
				//
				UPFILT_MODE[t] = 0;
				//
				write_int_form( hFileS , UPFILT_NAME[t] , atoi( UPFILT_DATA[t] ) );
				write_dbl_form( hFileS , UPFILT_NAME[t] , atof( UPFILT_DATA[t] ) );
				write_str_form( hFileS , UPFILT_NAME[t] , UPFILT_DATA[t] );
				//
			}
			//
			break;
		}
		//====================================================================================================
	}
	//==============================================================================================================================
	//==============================================================================================================================
	for ( i = 0 ; ( i < Step_Count ) && ( i < 999 ) ; i++ ) {
		tf = 0;
		while( TRUE ) {
			if ( !read_return_form2( hFileR , name_return , &type_return , &cc_return , &ii_return , &ff_return , &dd_return , str_return ) ) {
				_lclose( hFileR );
				_lclose( hFileS );
				remove( IMSI_FILE_FOR_STEP_ITEM_CHANGE );
				return 7;
			}
			//====================================================================================================
			if ( ( type_return == NAME_FORMAT ) && ( strcmp( name_return , "__&&STEP_BEGIN&&__" ) == 0 ) ) {
				tf = 1;
			}
			if ( ( type_return == NAME_FORMAT ) && ( strcmp( name_return , "__&&STEP_END&&__" ) == 0 ) ) {
				tf = 2;
			}
			else {
				//---------------
				if ( tf == 0 ) {
					_lclose( hFileR );
					_lclose( hFileS );
					remove( IMSI_FILE_FOR_STEP_ITEM_CHANGE );
					return 8;
				}
				//---------------
				for ( t = 0 ; t < UPFILT_COUNT ; t++ ) {
					if ( UPFILT_STEP[t] != ( i + 1 ) ) continue;
					if ( UPFILT_MODE[t] == -1 ) continue;
					//
					if ( STRCMP_L( UPFILT_NAME[t] , name_return ) ) {
						switch( type_return ) {
						case CHAR_FORMAT		:
						case CHAR_X_FORMAT		:
													cc_return = atoi( UPFILT_DATA[t] );	break;
						case INT_FORMAT			:
						case INT_X_FORMAT		:
													ii_return = atoi( UPFILT_DATA[t] );	break;
						case FLOAT_FORMAT		:
						case FLOAT_X_FORMAT		:
													ff_return = (float) atof( UPFILT_DATA[t] );	break;
						case DOUBLE_FORMAT		:
						case DOUBLE_X_FORMAT	:
													dd_return = atof( UPFILT_DATA[t] );	break;
						case FILENAME_FORMAT	:
						case FILENAME_X_FORMAT	:
						case STRING_FORMAT		:
						case STRING_X_FORMAT	:
													strcpy( str_return , UPFILT_DATA[t] );	break;
						}
						break;
					}
				}
				//---------------
				if ( t < UPFILT_COUNT ) { // 2013.03.28
					if ( UPFILT_MODE[t] == 3 ) {
						UPFILT_MODE[t] = 0;
						continue;
					}
					UPFILT_MODE[t] = 0;
				}
				//---------------
			}
			//====================================================================================================
			switch( type_return ) {
			case NAME_FORMAT		:	write_nam_form( hFileS , name_return );	break;
			case CHAR_FORMAT		:	write_chr_form( hFileS , name_return , cc_return );	break;
			case INT_FORMAT			:	write_int_form( hFileS , name_return , ii_return );	break;
			case FLOAT_FORMAT		:	write_flt_form( hFileS , name_return , ff_return );	break;
			case DOUBLE_FORMAT		:	write_dbl_form( hFileS , name_return , dd_return );	break;
			case FILENAME_FORMAT	:	write_filename_form( hFileS , name_return , str_return );	break;
			case STRING_FORMAT		:	write_str_form( hFileS , name_return , str_return );	break;
			//================================================================================================
			case NAME_X_FORMAT		:	write_nam_x_form( hFileS , name_return );	break;
			case CHAR_X_FORMAT		:	write_chr_x_form( hFileS , name_return , cc_return );	break;
			case INT_X_FORMAT		:	write_int_x_form( hFileS , name_return , ii_return );	break;
			case FLOAT_X_FORMAT		:	write_flt_x_form( hFileS , name_return , ff_return );	break;
			case DOUBLE_X_FORMAT	:	write_dbl_x_form( hFileS , name_return , dd_return );	break;
			case FILENAME_X_FORMAT	:	write_filename_x_form( hFileS , name_return , str_return );	break;
			case STRING_X_FORMAT	:	write_str_x_form( hFileS , name_return , str_return );	break;
			//================================================================================================
			}
			//====================================================================================================
			if ( tf == 2 ) {
				//
				for ( t = 0 ; t < UPFILT_COUNT ; t++ ) { // 2013.03.28
					if ( UPFILT_STEP[t] != ( i + 1 ) ) continue;
					if ( ( UPFILT_MODE[t] != 1 ) && ( UPFILT_MODE[t] != 2 ) ) continue;
					//
					UPFILT_MODE[t] = 0;
					//
					write_int_form( hFileS , UPFILT_NAME[t] , atoi( UPFILT_DATA[t] ) );
					write_dbl_form( hFileS , UPFILT_NAME[t] , atof( UPFILT_DATA[t] ) );
					write_str_form( hFileS , UPFILT_NAME[t] , UPFILT_DATA[t] );
					//
				}
				//
				break;
			}
			//====================================================================================================
		}
	}
	//==============================================================================================================================
	//==============================================================================================================================
	_lclose( hFileR );
	_lclose( hFileS );
	//
	sprintf( str_return , "%s_NeedRestore" , RecipeFileName );
	remove( str_return );
	if ( rename( RecipeFileName                 , str_return     ) != 0 ) return 10;
	if ( rename( IMSI_FILE_FOR_STEP_ITEM_CHANGE , RecipeFileName ) != 0 ) return 11;
	remove( str_return );
	remove( IMSI_FILE_FOR_STEP_ITEM_CHANGE );
	return 0;
}
//---------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//
// 2015.04.27
//
//void Get_Information_From_Message( int runstyle , char *Message ,
//								 char *filename , int *Main_Mode ,
//								 int *Type_Mode , int *Time , int *Slot , int *CassChamber , int *Finger , int *PutTime , int *Mindex , double *MinTime ,
//								 char *NextPM , char *lotStr ) {

int Get_Information_From_Message( int runstyle , char *Message ,
								 char *filename , int *Main_Mode ,
								 int *Type_Mode , int *Time , int *Slot , int *CassChamber , int *Finger , int *PutTime , int *Mindex , double *MinTime ,
								 char *NextPM , char *lotStr ) {
	int i;
// 256->1024 // 2010.08.30
	char MainStr[1024] , RunStr[1024] , InfoStr[1024] , ElseStr[1024] , ElseStr2[1024] , FileStr[1024], DirStr[1024];
	char s1str[1024] , s2str[1024] , rstr[1024];
	BOOL HasGrp;
	int GetInfoRes; // 2015.04.27

	//------
	GetInfoRes = 0; // 2015.04.27
	//------
	if ( runstyle ) {
		HasGrp = FALSE;
		for ( i = 0 ; i < (signed) strlen( Message ) ; i++ ) {
//			if ( Message[i] == ':' ) HasGrp = TRUE; // 2014.04.22
			if ( Message[i] == ':' ) { // 2014.04.22
				if ( Message[i+1] == '(' ) HasGrp = TRUE;
			}
		}
		if ( HasGrp ) {
			STR_SEPERATE( Message , RunStr , ElseStr , 1023 );
			if ( strlen( ElseStr ) > 0 ) {
				STR_SEPERATE_CHAR( ElseStr , ':' , FileStr , DirStr , 1023 );
				i = strlen( DirStr );
				if ( ( i >= 2 ) && ( DirStr[0] == '(' ) && ( DirStr[i-1] == ')' ) ) {
					DirStr[i-1] = 0;
					sprintf( filename , RunStr , DirStr + 1 );
				}
				else {
					sprintf( filename , RunStr , DirStr );
				}
				i = strlen( filename );
				if ( ( i > 0 ) && ( filename[i-1] != '\\' ) && ( filename[i-1] != '/' ) ) {
					strcat( filename , "/" );
				}
				strcat( filename , FileStr );
			}
			else {
				strcpy( filename , Message );
			}
		}
		else {
			strcpy( filename , Message );
		}
		//------
		*Main_Mode = 0;
		*Type_Mode = 0;
		*Time = 0;
		*Slot = -1;
		*CassChamber = 0;
		*Finger = 0;
		*PutTime = 0;
		strcpy( NextPM , "" );
		*Mindex = 0;
		strcpy( lotStr , "" );
		*MinTime = 0;
		//------
	}
	else {
		STR_SEPERATE( Message , MainStr , ElseStr , 1023 );
		//
		if ( STRCMP_L( MainStr , "TIME" ) )	{ // 2010.03.18
			STR_SEPERATE( ElseStr , InfoStr , ElseStr2 , 1023 );
			//
			*Time = atoi( InfoStr );
			//
			strcpy( ElseStr , ElseStr2 );
		}
		else {
			*Time = 0;
		}
		//
		STR_SEPERATE( ElseStr , InfoStr , ElseStr2 , 1023 );
		STR_SEPERATE_CHAR( InfoStr , ':' , rstr   , s1str , 1023 );	*Slot        = atoi( rstr );
		STR_SEPERATE_CHAR( s1str   , ':' , rstr   , s2str , 1023 );	*CassChamber = atoi( rstr );
		STR_SEPERATE_CHAR( s2str   , ':' , rstr   , s1str , 1023 );	*Finger      = atoi( rstr );
		STR_SEPERATE_CHAR( s1str   , ':' , rstr   , s2str , 1023 );	*PutTime     = atoi( rstr );
		STR_SEPERATE_CHAR( s2str   , ':' , NextPM , s1str , 1023 ); // 2002.07.16
		//
		STR_SEPERATE_CHAR( s1str   , ':' , rstr   , s2str , 1023 );
		STR_SEPERATE_CHAR( rstr   , '/' , s1str   , lotStr , 1023 ); // 2006.04.06

		LotData_Change_to( FALSE , lotStr ); // 2007.10.07

		*MinTime     = atof( s1str ); // 2006.04.06

		STR_SEPERATE_CHAR( s2str   , ':' , rstr   , s1str , 1023 );	if ( rstr[0] == 0 ) *Mindex = -1; else *Mindex = atoi( rstr ); // 2005.10.21

		HasGrp = FALSE;
		for ( i = 0 ; i < (signed) strlen( ElseStr2 ) ; i++ ) {
//			if ( ElseStr2[i] == ':' ) HasGrp = TRUE; // 2014.04.22
			if ( ElseStr2[i] == ':' ) { // 2014.04.22
				if ( ElseStr2[i+1] == '(' ) HasGrp = TRUE;
			}
		}

		if ( HasGrp ) {
			STR_SEPERATE( ElseStr2 , RunStr , ElseStr , 1023 );
			if ( strlen( ElseStr ) > 0 ) {
				STR_SEPERATE_CHAR( ElseStr , ':' , FileStr , DirStr , 1023 );
				i = strlen( DirStr );
				if ( ( i >= 2 ) && ( DirStr[0] == '(' ) && ( DirStr[i-1] == ')' ) ) {
					DirStr[i-1] = 0;
					sprintf( filename , RunStr , DirStr + 1 );
				}
				else {
					sprintf( filename , RunStr , DirStr );
				}
				i = strlen( filename );
				if ( ( i > 0 ) && ( filename[i-1] != '\\' ) && ( filename[i-1] != '/' ) ) {
					strcat( filename , "/" );
				}
				strcat( filename , FileStr );
			}
			else {
				strcpy( filename , ElseStr2 );
			}
		}
		else {
			strcpy( filename , ElseStr2 );
		}
		//============================================================================================================================
		//============================================================================================================================
		//============================================================================================================================
		if      ( STRCMP_L( MainStr , "PROCESS_IN"      ) )	{	*Main_Mode = 0;	*Type_Mode = 0;	} //
		else if ( STRCMP_L( MainStr , "PROCESS_OUT"     ) )	{	*Main_Mode = 1;	*Type_Mode = 1;	} //
		else if ( STRCMP_L( MainStr , "PROCESS_PRE"     ) )	{	*Main_Mode = 1;	*Type_Mode = 2;	} //
		else if ( STRCMP_L( MainStr , "PROCESS_IN/B"    ) )	{	*Main_Mode = 0;	*Type_Mode = 11;	}
		else if ( STRCMP_L( MainStr , "PROCESS_IN/E"    ) )	{	*Main_Mode = 0;	*Type_Mode = 12;	}
		else if ( STRCMP_L( MainStr , "PROCESS_IN/D"    ) )	{	*Main_Mode = 0;	*Type_Mode = 13;	} // from 2002.05.28
		else if ( STRCMP_L( MainStr , "PROCESS_IN/M"    ) )	{	*Main_Mode = 0;	*Type_Mode = 14;	}
		else if ( STRCMP_L( MainStr , "PROCESS_OUT/B"   ) )	{	*Main_Mode = 1;	*Type_Mode = 21;	} //
		else if ( STRCMP_L( MainStr , "PROCESS_OUT/E"   ) )	{	*Main_Mode = 1;	*Type_Mode = 22;	} //
		else if ( STRCMP_L( MainStr , "PROCESS_OUT/D"   ) )	{	*Main_Mode = 1;	*Type_Mode = 23;	} // from 2002.05.28
		else if ( STRCMP_L( MainStr , "PROCESS_OUT/M"   ) )	{	*Main_Mode = 1;	*Type_Mode = 24;	}
		else if ( STRCMP_L( MainStr , "PROCESS_OUT/FN"  ) )	{	*Main_Mode = 1;	*Type_Mode = 25;	} // from 2003.11.05
		else if ( STRCMP_L( MainStr , "PROCESS_OUT/FY"  ) )	{	*Main_Mode = 1;	*Type_Mode = 26;	} // from 2003.11.05
		else if ( STRCMP_L( MainStr , "PROCESS_PRE/B"   ) )	{	*Main_Mode = 1;	*Type_Mode = 31;	}
		else if ( STRCMP_L( MainStr , "PROCESS_PRE/D"   ) )	{	*Main_Mode = 1;	*Type_Mode = 32;	}
		else if ( STRCMP_L( MainStr , "PROCESS_PRE/E"   ) )	{	*Main_Mode = 1;	*Type_Mode = 33;	}
		else if ( STRCMP_L( MainStr , "PROCESS_PRE/M"   ) )	{	*Main_Mode = 1;	*Type_Mode = 34;	}
		else if ( STRCMP_L( MainStr , "MAINTENANCE"     ) )	{	*Main_Mode = 2;	*Type_Mode = 44;	}
		//
		else if ( STRCMP_L( MainStr , "RECALL_IN"       ) )	{	*Main_Mode = 0;	*Type_Mode = 100;	}
		else if ( STRCMP_L( MainStr , "RECALL_OUT"      ) )	{	*Main_Mode = 1;	*Type_Mode = 101;	}
		else if ( STRCMP_L( MainStr , "RECALL_PRE"      ) )	{	*Main_Mode = 1;	*Type_Mode = 102;	}
//		else if ( STRCMP_L( MainStr , "RECALL_MAN"      ) )	{	*Main_Mode = 1;	*Type_Mode = 144;	} // 2012.02.01
		else if ( STRCMP_L( MainStr , "RECALL_MAN"      ) )	{	*Main_Mode = 2;	*Type_Mode = 144;	} // 2012.02.01
		//
		else {
			//
			*Main_Mode = 3;
			//
			// 2015.04.27
			//
			if ( STRCMP_L( MainStr , "TIME" ) )	{
				//
				switch( TIME_MESSAGE_OPERATION ) {
				case 1 :
					if ( TIME_MESSAGE_ALARM > 0 ) ALARM_POST( TIME_MESSAGE_ALARM );
					//
					GetInfoRes = 1;
					break;
				case 2 :
					if ( TIME_MESSAGE_ALARM > 0 ) ALARM_POST( TIME_MESSAGE_ALARM );
					//
					GetInfoRes = 2;
					break;
				}
				//
			}
			else {
				//
				switch( INVALID_MESSAGE_OPERATION ) {
				case 1 :
					if ( INVALID_MESSAGE_ALARM > 0 ) ALARM_POST( INVALID_MESSAGE_ALARM );
					//
					GetInfoRes = 1;
					break;
				case 2 :
					if ( INVALID_MESSAGE_ALARM > 0 ) ALARM_POST( INVALID_MESSAGE_ALARM );
					//
					GetInfoRes = 2;
					break;
				}
				//
			}
			//
		}
	}
	//
	return GetInfoRes; // 2015.04.27
	//
}
//=======================================================================================
Module_Status Program_Time( char *name , double Target , double puttime , double mintime ) {
	int CommStatus;
	double ElapsedTime , Run;
	char upstr[256];
	//
	if ( FUNCTION_TIME_GATEWAY_ADDRESS >= 0 ) { // 2017.04.01
		//
		CommStatus = _dRUN_FUNCTION( FUNCTION_TIME_GATEWAY_ADDRESS , name );
		//
		_dREAD_UPLOAD_MESSAGE( FUNCTION_TIME_GATEWAY_ADDRESS , upstr );
		_dWRITE_UPLOAD_MESSAGE( upstr );
		//
		return CommStatus;
		//
	}
	//
	printf( "PROCESS TIME WAIT %s\n" , name );
	TIME_FINISH = FALSE; // 2006.03.27
	ElapsedTime = 0;
	if ( Target > 0 ) {
		S_TIMER_READY();
		while( TRUE ) {
			if ( !WAIT_SECONDS( 0.5 ) ) {
				if ( TIMER_IO >= 0 ) _dWRITE_DIGITAL( TIMER_IO , 0 , &CommStatus );
//				TUNE_COUNT = 0; // 2007.06.11
				return SYS_ABORTED;
			}
			ElapsedTime = S_TIMER_ELAPSED();
			switch( TIMER_MODE ) {
			case 4 :
			case 5 :
			case 6 :
			case 7 :
				Run = ElapsedTime + puttime;
				break;
			default :
				Run = ElapsedTime;
				break;
			}
			if ( TIME_FINISH || ( Run >= Target ) ) break;
			if ( TIMER_IO >= 0 ) {
				switch( TIMER_MODE ) {
				case 1 :
				case 5 : // 2006.08.03
					_dWRITE_DIGITAL( TIMER_IO , (int) ( Target - Run ) , &CommStatus );
					break;
				case 2 :
				case 6 : // 2006.08.03
					_dWRITE_DIGITAL( TIMER_IO , (int) ( ( Run * 100 ) / Target ) , &CommStatus );
					break;
				case 3 :
				case 7 : // 2006.08.03
					_dWRITE_DIGITAL( TIMER_IO , (int) ( ( ( Target - Run ) * 100 ) / Target ) , &CommStatus );
					break;
				default :
					_dWRITE_DIGITAL( TIMER_IO , (int) Run , &CommStatus );
					break;
				}
			}
		}
		if ( TIMER_IO >= 0 ) _dWRITE_DIGITAL( TIMER_IO , 0 , &CommStatus );
	}
	printf( "PROCESS TIME END %s\n" , name );
//	TUNE_COUNT = 0; // 2007.06.11
	_dWRITE_UPLOAD_MESSAGE( "" ); // Append 2002.03.25
	//======================================================================================================
	// 2005.08.18
	//======================================================================================================
	if ( ( !TIME_FINISH ) && ( mintime > ElapsedTime ) ) { // 2006.03.27
		if ( !WAIT_SECONDS( mintime - ElapsedTime ) ) return SYS_ABORTED;
	}
	//======================================================================================================
	return SYS_SUCCESS;
}
//
Module_Status Program_Upload_Filtering( int MIndex , int Main_Mode , char *FileName , int Type_Mode , int Slot , int CassChamber , int Finger , int PutTime , char *NextPM , char *lotStr ) {
	char ElseStr[256];
//	char s1str[256] , s2str[256] , rstr[256]; // 2008.04.04
//	char s1str[256] , s2str[256] , rstr[512]; // 2008.04.04 // 2015.05.05
	char s1str[1024] , s2str[1024] , rstr[1024]; // 2008.04.04 // 2015.05.05
	int i , z1 , z2 , Res;
	char upstr[2048];


	//====================================================================================
	UPFILT_COUNT = 0; // 2013.03.28
	//====================================================================================
	// 2013.03.28
	if ( TUNE_DATA_SAVING ) {
		//
		//-------------------------------------------------------------------
		//
		EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
		//
		//-------------------------------------------------------------------
		//
		for ( i = 0 ; i < TUNE_SAVE_COUNT ; i++ ) {
			//
			if ( TUNE_SAVE_TINDEX[i] >= TUNE_COUNT ) continue;
			//
			if ( ( TUNE_MIDX[TUNE_SAVE_TINDEX[i]] != -1 ) && ( TUNE_MIDX[TUNE_SAVE_TINDEX[i]] != MIndex ) ) continue;
			//
			if ( UPFILT_COUNT >= MAX_UPFILT_COUNT ) {
				_IO_CIM_PRINTF( "Program_Upload_Filtering Tune Save Data Setting Error - Too Many Data Set\n" );
				continue;
			}
			//
			if ( UPFILT_NAME[ UPFILT_COUNT ] == NULL ) {
				UPFILT_NAME[UPFILT_COUNT] = calloc( 127 + 1 , sizeof( char ) );
				if ( UPFILT_NAME[ UPFILT_COUNT ] == NULL ) {
					_IO_CIM_PRINTF( "UPFILT DATA AREA has no Memory 1 - [%d,%d] is Truncated\n" , TUNE_SAVE_MODE[i] , TUNE_SAVE_TINDEX[i] );
					continue;
				}
			}
			if ( UPFILT_DATA[ UPFILT_COUNT ] == NULL ) {
				UPFILT_DATA[UPFILT_COUNT] = calloc( 127 + 1 , sizeof( char ) );
				if ( UPFILT_DATA[ UPFILT_COUNT ] == NULL ) {
					_IO_CIM_PRINTF( "UPFILT DATA AREA has no Memory 2 - [%d,%d] is Truncated\n" , TUNE_SAVE_MODE[i] , TUNE_SAVE_TINDEX[i] );
					continue;
				}
			}
			//=================================================================================
			UPFILT_MODE[UPFILT_COUNT] = ( TUNE_SAVE_MODE[i] == -1 ) ? 3 : 2;
			//
			UPFILT_STEP[UPFILT_COUNT] = TUNE_STEP[TUNE_SAVE_TINDEX[i]];
			//=================================================================================
			strncpy( UPFILT_NAME[UPFILT_COUNT] , TUNE_NAME[TUNE_SAVE_TINDEX[i]] , 127 );	UPFILT_NAME[UPFILT_COUNT][127] = 0;
			strncpy( UPFILT_DATA[UPFILT_COUNT] , TUNE_DATA[TUNE_SAVE_TINDEX[i]] , 127 );	UPFILT_DATA[UPFILT_COUNT][127] = 0;
			//---------------------------------------------------------
			UPFILT_COUNT++;
			//---------------------------------------------------------
		}
		//-------------------------------------------------------------------
		//
		LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
		//
		//-------------------------------------------------------------------
	}
	//====================================================================================
	if ( !UPLOAD_FILTERING ) {
		_dREAD_UPLOAD_MESSAGE( FUNCTION_ADDRESS[Main_Mode] , upstr );
		_dWRITE_UPLOAD_MESSAGE( upstr );
	}
	else {
		//====================================================================================
		if ( UPLOAD_FIL_STR == NULL ) {
			UPLOAD_FIL_STR = calloc( 32768 + 1 , sizeof( char ) );
			if ( UPLOAD_FIL_STR == NULL ) {
				_IO_CIM_PRINTF( "RcpHandler Sub Buffer 32768 Init Error - Less Memory\n" );
				return SYS_ABORTED;
			}
		}
		_dREAD_UPLOAD_MESSAGE( FUNCTION_ADDRESS[Main_Mode] , UPLOAD_FIL_STR );
		//
//		UPFILT_COUNT = 0; // 2013.03.28
		strcpy( upstr , "" );
		i = 0;
		z1 = 0;
		//
		while( TRUE ) {
			//
			z2 = STR_SEPERATE_CHAR_WITH_POINTER( UPLOAD_FIL_STR , '|' , ElseStr , z1 , 255 );
			//
			if ( z1 == z2 ) break;
			//
			if      ( ( ElseStr[0] == 'T' ) && ( ElseStr[1] == 'X' ) ) { // 2013.03.28
				//
				for ( i = 0 ; i < UPFILT_COUNT ; i++ ) {
					if ( UPFILT_MODE[i] > 1 ) UPFILT_MODE[i] = -1;
				}
				//
			}
			else if ( ( ElseStr[0] == 'R' ) && ( ElseStr[1] == ' ' ) ) {
				if ( UPFILT_COUNT < MAX_UPFILT_COUNT ) {
					//---------------------------------------------------------
					STR_SEPERATE_CHAR( ElseStr + 2 , ':' , s1str , rstr , 255 );
					//
					Res = atoi( s1str );
					STR_SEPERATE_CHAR( rstr , ':' , s1str , s2str , 255 );
					//
					//=================================================================================
					// 2007.06.08
					//=================================================================================
					if ( UPFILT_NAME[ UPFILT_COUNT ] == NULL ) {
						UPFILT_NAME[UPFILT_COUNT] = calloc( 127 + 1 , sizeof( char ) );
						if ( UPFILT_NAME[ UPFILT_COUNT ] == NULL ) {
							_IO_CIM_PRINTF( "UPFILT DATA AREA has no Memory 1 - [Step %d,%s=%s,%d] is Truncated\n" , Res , s1str , s2str , MIndex );
							break;
						}
					}
					if ( UPFILT_DATA[ UPFILT_COUNT ] == NULL ) {
						UPFILT_DATA[UPFILT_COUNT] = calloc( 127 + 1 , sizeof( char ) );
						if ( UPFILT_DATA[ UPFILT_COUNT ] == NULL ) {
							_IO_CIM_PRINTF( "UPFILT DATA AREA has no Memory 2 - [Step %d,%s=%s,%d] is Truncated\n" , Res , s1str , s2str , MIndex );
							break;
						}
					}
					//=================================================================================
					UPFILT_MODE[UPFILT_COUNT] = 1; // 2013.03.28
					//
					UPFILT_STEP[UPFILT_COUNT] = Res;
					//=================================================================================
					strncpy( UPFILT_NAME[UPFILT_COUNT] , s1str , 127 );
					strncpy( UPFILT_DATA[UPFILT_COUNT] , s2str , 127 );
					UPFILT_COUNT++;
					//---------------------------------------------------------
				}
			}
			else if ( ( ElseStr[0] == 'C' ) && ( ElseStr[1] == ' ' ) ) { // 2006.04.28
				if      ( Event_Parameter_Received_Tag == 0 ) { // not run
					_IO_CONSOLE_PRINTF( "[RECALL]\t%s REJECT BY NOTRUN\n" , ElseStr );
				}
				else if ( ( Event_Parameter_Received_Tag == 1 ) || ( Event_Parameter_Received_Tag == 2 ) ) { // run
					//------------------------------------------------
					if ( Event_Parameter_Received_Tag == 1 ) {
						Event_Parameter_Received_Tag = 2;
						Event_Parameter_Received_Count = 0;
					}
					if ( Event_Parameter_Received_Count >= MAX_EVTRECV_COUNT ) {
						//------------------------------------------------
						_IO_CONSOLE_PRINTF( "[RECALL]\t%s REJECT BY OVERFLOW DATA\n" , ElseStr );
						//------------------------------------------------
					}
					else {
						EnterCriticalSection( &Event_Parameter_CR );

						LotData_Change_to( TRUE , lotStr ); // 2007.10.07

						switch( Type_Mode ) {
						case 0 :	case 11 :	case 12 :	case 13 :	case 14 :							case 100 :
//							sprintf( Event_Parameter_Data[Event_Parameter_Received_Count] , "RECALL_IN %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							sprintf( rstr , "RECALL_IN %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							break;
						case 1 :	case 21 :	case 22 :	case 23 :	case 24 :	case 25 :	case 26 :	case 101 :
//							sprintf( Event_Parameter_Data[Event_Parameter_Received_Count] , "RECALL_OUT %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							sprintf( rstr , "RECALL_OUT %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							break;
						case 2 :	case 31 :	case 32 :	case 33 :	case 34 :							case 102 :
//							sprintf( Event_Parameter_Data[Event_Parameter_Received_Count] , "RECALL_PRE %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							sprintf( rstr , "RECALL_PRE %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							break;
						case 44 :																			case 144 :
//							sprintf( Event_Parameter_Data[Event_Parameter_Received_Count] , "RECALL_MAN %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							sprintf( rstr , "RECALL_MAN %d:%d:%d:%d:%s:%d%s:%d %s" , Slot , CassChamber , Finger , PutTime , NextPM , 0 , lotStr , RECALL_INDEX , ElseStr + 2 ); // 2008.04.04
							break;
						default : // 2008.04.04
							strcpy( rstr , "" );
							break;
						}
						//------------------------------------------------
						STR_MEM_MAKE_COPY( &(Event_Parameter_Data[Event_Parameter_Received_Count]) , rstr ); // 2008.04.04
						Event_Parameter_Mode[Event_Parameter_Received_Count] = 2; // 2010.03.10
						//------------------------------------------------
						Event_Parameter_Received_Count++;
						//------------------------------------------------
						LeaveCriticalSection( &Event_Parameter_CR );
						//------------------------------------------------
						RECALL_INDEX++;
					}
				}
			}
			else {
				if ( ElseStr[0] != 0 ) {
					if ( i == 0 ) {
						strcat( upstr , ElseStr );
					}
					else {
						strcat( upstr , "|" );
						strcat( upstr , ElseStr );
					}
					i++;
				}
			}
			z1 = z2;
		}
		_dWRITE_UPLOAD_MESSAGE( upstr );
		//================================================================================
/*
// 2013.03.28
		if ( ( FileName[0] != 0 ) && ( FileName[0] != '?' ) ) {
			if ( UPFILT_COUNT > 0 ) {
				i = Recipe_Item_Modify( FileName );
				if ( i != 0 ) _IO_CIM_PRINTF( "RcpHandler Recipe_Item_Modify Fail for Upload Data[%d]\n" , i );
			}
		}
*/
		//================================================================================
	}
	//
// 2013.03.28
	if ( UPFILT_COUNT > 0 ) {
		if ( ( FileName[0] != 0 ) && ( FileName[0] != '?' ) ) {
			i = Recipe_Item_Modify( FileName );
			if ( i != 0 ) _IO_CIM_PRINTF( "RcpHandler Recipe_Item_Modify Fail for Upload Data[%d]\n" , i );
		}
	}
	return SYS_SUCCESS;
}
//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
int LAST_RUN_CHECK; // 2010.03.10
int LAST_RUN_MAIN; // 2010.03.10
int LAST_RUN_TYPE; // 2010.03.10
int LAST_RUN_SLOT; // 2010.03.10
int LAST_RUN_CASS; // 2010.03.10
char LAST_RUN_FILE[256]; // 2010.03.10
//
Module_Status Program_Main2( int startindex , BOOL *prcheck ) { // 2004.10.07
	int	Main_Mode , Type_Mode , i , Res , Time , Slot , CassChamber , Finger , PutTime , Mindex;
	char AllStr[1024] , lotStr[1024];
	char NextPM[256] = { 0 , }; // 2002.07.16
	double MinTime; // 2005.08.18
//	int Res2; // 2002.08.20
	BOOL runstyle; // 2006.04.28
	int GetInfoRes; // 2015.04.27



//FILE *fpt;






	LAST_RUN_CHECK = FALSE; // 2010.03.14

	*prcheck = FALSE; // 2006.11.14

	_IO_CONSOLE_PRINTF( "[RUN]\t%s\n" , Program_Param_Read_Buffer ); // 2003.10.28
	//============================================================================================================================
	//============================================================================================================================
	//============================================================================================================================
	if ( !MAIN_STATUS ) {
		_IO_CIM_PRINTF( "Setup Information Error\n" );
//		TUNE_COUNT = 0; // 2007.06.11
		return SYS_ABORTED;
	}
	//============================================================================================================================
	//============================================================================================================================
	//============================================================================================================================
	if ( strlen( Program_Param_Read_Buffer ) <= 0 ) { // 2003.07.22
		//
		*prcheck = TRUE; // 2006.11.14
		//
		/*
		// 2007.10.25
		if ( FUNCTION_ADDRESS[0] >= 0 ) {
			i = _dREAD_FUNCTION( FUNCTION_ADDRESS[0] );
			if ( i == SYS_ABORTED ) {
				if ( !WAIT_SECONDS( 10 ) ) return SYS_ABORTED;
			}
		}
		else {
			if ( FUNCTION_ADDRESS[1] >= 0 ) {
				i = _dREAD_FUNCTION( FUNCTION_ADDRESS[1] );
				if ( i == SYS_ABORTED ) {
					if ( !WAIT_SECONDS( 10 ) ) return SYS_ABORTED;
				}
			}
			else {
				if ( FUNCTION_ADDRESS[2] >= 0 ) {
					i = _dREAD_FUNCTION( FUNCTION_ADDRESS[2] );
					if ( i == SYS_ABORTED ) {
						if ( !WAIT_SECONDS( 10 ) ) return SYS_ABORTED;
					}
				}
			}
		}
		*/
//		i = SYS_ABORTED; // 2007.10.25
		i = SYS_SUCCESS; // 2007.10.25
		if ( FUNCTION_ADDRESS[0] >= 0 ) {
//			i = _dRUN_WAIT_FUNCTION( FUNCTION_ADDRESS[0] ); // 2007.10.25
			Res = _dRUN_WAIT_FUNCTION( FUNCTION_ADDRESS[0] );
			if ( Res == SYS_ABORTED ) i = SYS_ABORTED; // 2007.10.25
		}
		if ( FUNCTION_ADDRESS[1] >= 0 ) {
//			i = _dRUN_WAIT_FUNCTION( FUNCTION_ADDRESS[1] ); // 2007.10.25
			Res = _dRUN_WAIT_FUNCTION( FUNCTION_ADDRESS[1] );
			if ( Res == SYS_ABORTED ) i = SYS_ABORTED; // 2007.10.25
		}
		if ( FUNCTION_ADDRESS[2] >= 0 ) {
//			i = _dRUN_WAIT_FUNCTION( FUNCTION_ADDRESS[2] ); // 2007.10.25
			Res = _dRUN_WAIT_FUNCTION( FUNCTION_ADDRESS[2] );
			if ( Res == SYS_ABORTED ) i = SYS_ABORTED; // 2007.10.25
		}
		return i;
	}
	//============================================================================================================================
	//============================================================================================================================
	// 2006.10.31
	//============================================================================================================================
	if ( startindex == 0 ) {
		if ( PRE_RUN_FUNCTION != -1 ) {
			if ( _dRUN_FUNCTION( PRE_RUN_FUNCTION , STR_MEM_GET_STR( PRE_RUN_FUNCTION_STR ) ) != SYS_SUCCESS ) return SYS_ABORTED;
		}
	}
	//============================================================================================================================
	//============================================================================================================================
	Function_String_Send_Reset();
	//============================================================================================================================
	if ( SINGLE_MODE ) { // 2006.04.28
		runstyle = TRUE;
		if      ( STRNCMP_L( Program_Param_Read_Buffer , "RECALL_IN"  , 9  ) )	runstyle = FALSE;
		else if ( STRNCMP_L( Program_Param_Read_Buffer , "RECALL_OUT" , 10 ) )	runstyle = FALSE;
		else if ( STRNCMP_L( Program_Param_Read_Buffer , "RECALL_PRE" , 10 ) )	runstyle = FALSE;
	}
	else {
		runstyle = FALSE;
	}
	//============================================================================================================================
	// 2015.04.27
	//
	GetInfoRes = Get_Information_From_Message( runstyle , Program_Param_Read_Buffer ,
								 AllStr , &Main_Mode ,
								 &Type_Mode , &Time , &Slot , &CassChamber , &Finger , &PutTime , &Mindex , &MinTime ,
								 NextPM , lotStr );
	//
	//======================================================================================================
	//
	// 2015.04.27
	//
	if ( ( GetInfoRes == 1 ) || ( GetInfoRes == 2 ) ) {
		//
		_IO_CIM_PRINTF( "Recipe Reading Message 1 Error[%d] - %s[%s] - [%ld]\n" , GetInfoRes , Program_Param_Read_Buffer , AllStr , GetLastError() );
		//
		Global_Buffer_strinit();
		//
		_dWRITE_UPLOAD_MESSAGE( "" );
		//
		return ( GetInfoRes == 1 ) ? SYS_ERROR : SYS_ABORTED;
	}
	//
	//
	//======================================================================================================
	// 2010.03.14
	LAST_RUN_CHECK = TRUE;
	LAST_RUN_MAIN = Main_Mode;
	LAST_RUN_TYPE = Type_Mode;
	LAST_RUN_SLOT = Slot;
	LAST_RUN_CASS = CassChamber;
	strcpy( LAST_RUN_FILE , AllStr );
	//============================================================================================================================
	if ( Main_Mode == 3 ) return Program_Time( Program_Param_Read_Buffer , (double) Time , ( double ) PutTime , MinTime );
	//======================================================================================================
	// 2005.08.18
	//======================================================================================================
	S_TIMER_READY();
	//======================================================================================================
		
	if ( FUNCTION_ADDRESS[Main_Mode] < 0 ) {
		Res = SYS_SUCCESS;
		_dWRITE_UPLOAD_MESSAGE( "" ); // Append 2002.03.25
	}
	else {
		if ( !Global_Buffer_strinit() ) { // 2004.01.27
			//
			_IO_CIM_PRINTF( "RcpHandler Buffer Init Error - Less Memory\n" );
			//
//			TUNE_COUNT = 0; // 2007.06.11
			_dWRITE_UPLOAD_MESSAGE( "" ); // Append 2002.03.25
			return SYS_ABORTED;
		}
		//
		TUNE_FILE_SET_OCR( CassChamber , Type_Mode , Slot ); // 2010.08.20
		//











//if (TUNE_COUNT != 4)	printf("          SETUP START [%d]    > ERROR\n", TUNE_COUNT);
//else					printf("          SETUP START [%d]\n", TUNE_COUNT);











		Res = SETUP_MESSAGE( 0 , AllStr , Type_Mode , Slot , CassChamber , Finger , PutTime , NextPM , Mindex , lotStr , 0 );
		//






//printf("          SETUP END   [%d]\n", TUNE_COUNT);















		if ( Res != 0 ) {
			//
//			_IO_CIM_PRINTF( "Recipe Reading 1 Error[%d] - %s[%s]\n" , Res , Program_Param_Read_Buffer , AllStr ); // 2015.02.13
			_IO_CIM_PRINTF( "Recipe Reading 1 Error[%d] - %s[%s] - [%ld]\n" , Res , Program_Param_Read_Buffer , AllStr , GetLastError() ); // 2015.02.13
			//
			Global_Buffer_strinit(); // 2004.01.27
			//
			if ( INVALID_FILE_ALARM > 0 ) ALARM_POST( INVALID_FILE_ALARM ); // 2014.01.10
			//
			if ( INVALID_FILE_OPERATION != -1 ) { // 2013.10.29
				//
				_dWRITE_UPLOAD_MESSAGE( "" );
				//
				return INVALID_FILE_OPERATION;
			}
			//
		}

//		Res = _dRUN_FUNCTION( FUNCTION_ADDRESS[Main_Mode] , Buffer ); // 2002.08.19
//		Res2 = _dRUN_FUNCTION_FC( FUNCTION_ADDRESS[Main_Mode] , Buffer , 100 ); // 2002.08.19
//		Res = _dRUN_FUNCTION_FC( FUNCTION_ADDRESS[Main_Mode] , Buffer , 50 ); // 2002.08.20 // 2004.01.27
		//------------------------------------------------------------------------------------------










































		for ( i = 0 ; i <= Global_Buffer_IndexCount() ; i++ ) {
//			Res = _dRUN_FUNCTION_FC( FUNCTION_ADDRESS[Main_Mode] , Global_Buffer_Get( i ) , 50 ); // 2002.08.20 // 2004.01.27 // 2005.10.27
			Res = _dRUN_FUNCTION( FUNCTION_ADDRESS[Main_Mode] , Global_Buffer_Get( i ) ); // 2002.08.20 // 2004.01.27 // 2005.10.27
//			if ( Res != SYS_SUCCESS ) break;
			if ( Res != SYS_SUCCESS ) break; // 2005.10.27
		}




//		if (CHECK_COUNT != 4) {
//			printf("     CALLING [%d]                                 ERROR[%d]\n", _dREAD_FUNCTION_EVENT(-1001), CHECK_COUNT);
//
//			fpt = fopen("error.log", "a");
//			fprintf(fpt, "     CALLING [%d]                                 ERROR[%d]\n", _dREAD_FUNCTION_EVENT(-1001), CHECK_COUNT);
//			fclose(fpt);
//
//		}
//		else {
//			printf("     CALLING [%d]\n", _dREAD_FUNCTION_EVENT(-1001));
//
//		}
//
//		Res = SYS_SUCCESS;
//
//



























		//------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------
		//==============================================================
		// Append 2002.08.19 // for Stable data
		// Delete 2002.08.20
		//==============================================================
//		Res = _dRUN_WAIT_FUNCTION( FUNCTION_ADDRESS[Main_Mode] );
//		if ( Res != Res2 ) {
//			_IO_CIM_PRINTF( "RcpHandler Result Monitor Data Different([First=%d],[Second&Use=%d])\n" , Res2 , Res );
//		}
		//==============================================================
		// Append 2002.03.25
		//==============================================================
//		_dREAD_UPLOAD_MESSAGE( FUNCTION_ADDRESS[Main_Mode] , upstr ); // 2006.04.14
//		_dWRITE_UPLOAD_MESSAGE( upstr ); // 2006.04.14
		//==============================================================
		//==============================================================
		// Append 2006.04.14
		//==============================================================
		if ( Program_Upload_Filtering( Mindex , Main_Mode , AllStr , Type_Mode , Slot , CassChamber , Finger , PutTime , NextPM , lotStr ) != SYS_SUCCESS ) {
			return SYS_ABORTED;
		}
		//==============================================================
	}
//	TUNE_COUNT = 0; // 2007.06.11
	//======================================================================================================
	// 2005.08.18
	//======================================================================================================
	if ( Res == SYS_SUCCESS ) { // 2005.10.27
		if ( MinTime > S_TIMER_ELAPSED() ) {
			if ( !WAIT_SECONDS( MinTime - S_TIMER_ELAPSED() ) ) return SYS_ABORTED;
		}
	}
	//======================================================================================================
	return Res;
}

//------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
Module_Status Program_Main3() {
	int	Main_Mode , Type_Mode , i , Res , Time , Slot , CassChamber , Finger , PutTime , Mindex;
	char AllStr[1024] , lotStr[1024];
	char NextPM[256] = { 0 , }; // 2002.07.16
	double MinTime;
	int GetInfoRes; // 2015.04.27

	//============================================================================================================================
	_IO_CONSOLE_PRINTF( "[RUN]\t%s\n" , Program_Param_Read_Buffer ); // 2003.10.28
	//============================================================================================================================
	if ( !MAIN_STATUS ) {
		_IO_CIM_PRINTF( "Setup Information Error\n" );
//		TUNE_COUNT = 0; // 2007.06.11
		return SYS_ABORTED;
	}
	//============================================================================================================================
	Function_String_Send_Reset();
	//============================================================================================================================
	if ( !Global_Buffer_strinit() ) {
		_IO_CIM_PRINTF( "RcpHandler Buffer Init Error - Less Memory\n" );
//		TUNE_COUNT = 0; // 2007.06.11
		_dWRITE_UPLOAD_MESSAGE( "" );
		return SYS_ABORTED;
	}
	//============================================================================================================================
	S_TIMER_READY();
	//============================================================================================================================
	// 2015.04.27
	//
	GetInfoRes = Get_Information_From_Message( FALSE , Program_Param_Read_Buffer ,
								 AllStr , &Main_Mode ,
								 &Type_Mode , &Time , &Slot , &CassChamber , &Finger , &PutTime , &Mindex , &MinTime ,
								 NextPM , lotStr );
	//
	// 2015.04.27
	//
	if ( ( GetInfoRes == 1 ) || ( GetInfoRes == 2 ) ) {
		//
		_IO_CIM_PRINTF( "Recipe Reading Message 2 Error[%d] - %s[%s] - [%ld]\n" , GetInfoRes , Program_Param_Read_Buffer , AllStr , GetLastError() );
		//
		Event_Parameter_Received_Count = 0;
		Global_Buffer_strinit();
		//
		_dWRITE_UPLOAD_MESSAGE( "" );
		//
		return ( GetInfoRes == 1 ) ? SYS_ERROR : SYS_ABORTED;
	}
	//
	//
	if ( Main_Mode == 3 ) {
		Event_Parameter_Received_Count = 0;
		return Program_Time( "MULTI_MIXED_RECIPE" , (double) Time , ( double ) PutTime , MinTime );
	}
	//
	Res = SETUP_MESSAGE( 0 , AllStr , Type_Mode , Slot , CassChamber , Finger , PutTime , NextPM , Mindex , lotStr , 0 );
	if ( Res != 0 ) {
		//
//		_IO_CIM_PRINTF( "Recipe Reading 2 Error[%d] - %s[%s]\n" , Res , Program_Param_Read_Buffer , AllStr ); // 2015.02.13
		_IO_CIM_PRINTF( "Recipe Reading 2 Error[%d] - %s[%s] - [%ld]\n" , Res , Program_Param_Read_Buffer , AllStr , GetLastError() ); // 2015.02.13
		//
		Event_Parameter_Received_Count = 0;
		Global_Buffer_strinit();
//		TUNE_COUNT = 0; // 2007.06.11
		_dWRITE_UPLOAD_MESSAGE( "" );
//		return SYS_ABORTED; // 2013.10.29
		if ( INVALID_FILE_ALARM > 0 ) ALARM_POST( INVALID_FILE_ALARM ); // 2014.01.10
		if ( INVALID_FILE_OPERATION == -1 ) return SYS_ABORTED; // 2013.10.29
		return INVALID_FILE_OPERATION; // 2013.10.29
	}
	for ( i = 0 ; i < Event_Parameter_Received_Count ; i++ ) {
		// 2015.04.27
		//
		GetInfoRes = Get_Information_From_Message( FALSE , STR_MEM_GET_STR( Event_Parameter_Data[i] ) ,
									 AllStr , &Main_Mode ,
									 &Type_Mode , &Time , &Slot , &CassChamber , &Finger , &PutTime , &Mindex , &MinTime ,
									 NextPM , lotStr );
		//
		//
		//======================================================================================================
		//
		// 2015.04.27
		//
		if ( ( GetInfoRes == 1 ) || ( GetInfoRes == 2 ) ) {
			//
			_IO_CIM_PRINTF( "Recipe Reading Message 3 Error[%d] - %s[%s] - [%ld]\n" , GetInfoRes , Program_Param_Read_Buffer , AllStr , GetLastError() );
			//
			Event_Parameter_Received_Count = 0;
			Global_Buffer_strinit();
			//
			_dWRITE_UPLOAD_MESSAGE( "" );
			//
			return ( GetInfoRes == 1 ) ? SYS_ERROR : SYS_ABORTED;
		}
		//
		//
		//======================================================================================================
		//
		Res = SETUP_MESSAGE( 2 , AllStr , Type_Mode , Slot , CassChamber , Finger , PutTime , NextPM , Mindex , lotStr , 0 );
		if ( Res != 0 ) {
			//
//			_IO_CIM_PRINTF( "Recipe Reading 3 Error[%d] - %s[%s]\n" , Res , Program_Param_Read_Buffer , AllStr ); // 2015.02.13
			_IO_CIM_PRINTF( "Recipe Reading 3 Error[%d] - %s[%s] - [%ld]\n" , Res , Program_Param_Read_Buffer , AllStr , GetLastError() ); // 2015.02.13
			//
			Event_Parameter_Received_Count = 0;
			Global_Buffer_strinit();
//			TUNE_COUNT = 0; // 2007.06.11
			_dWRITE_UPLOAD_MESSAGE( "" );
//			return SYS_ABORTED; // 2013.10.29
			if ( INVALID_FILE_ALARM > 0 ) ALARM_POST( INVALID_FILE_ALARM ); // 2014.01.10
			if ( INVALID_FILE_OPERATION == -1 ) return SYS_ABORTED; // 2013.10.29
			return INVALID_FILE_OPERATION; // 2013.10.29
		}
	}
	//============================================================================================================================
	Event_Parameter_Received_Count = 0;
	//============================================================================================================================
	if ( FUNCTION_ADDRESS[Main_Mode] < 0 ) {
		Res = SYS_SUCCESS;
		_dWRITE_UPLOAD_MESSAGE( "" );
	}
	else {
		//------------------------------------------------------------------------------------------
		for ( i = 0 ; i <= Global_Buffer_IndexCount() ; i++ ) {
			Res = _dRUN_FUNCTION( FUNCTION_ADDRESS[Main_Mode] , Global_Buffer_Get( i ) );
			if ( Res != SYS_SUCCESS ) break;
		}
		//==============================================================
		if ( Program_Upload_Filtering( Mindex , Main_Mode , "" , Type_Mode , Slot , CassChamber , Finger , PutTime , NextPM , lotStr ) != SYS_SUCCESS ) {
			return SYS_ABORTED;
		}
		//==============================================================
	}
//	TUNE_COUNT = 0; // 2007.06.11
	//======================================================================================================
	// 2005.08.18
	//======================================================================================================
	if ( Res == SYS_SUCCESS ) { // 2005.10.27
		if ( MinTime > S_TIMER_ELAPSED() ) {
			if ( !WAIT_SECONDS( MinTime - S_TIMER_ELAPSED() ) ) return SYS_ABORTED;
		}
	}
	//======================================================================================================
	return Res;
}

int LOT_LOG_POINTER[4] = { -2 , -2 , -2, -2 };

int _R_SCH_LOG_LOT_PRINTF( int M , LPSTR list , ... ) {
	va_list va;
	char TextBuffer[1024+1];
	//
	if ( M < 0 || M >= 4 ) return 1;
	//
	if ( LOT_LOG_POINTER[M] == -2 ) {
		LOT_LOG_POINTER[M] = _FIND_FROM_STRINGF( _K_F_IO + 1 , "SCHEDULER_LOTLOG%d" , M + 1 );
	}
	//
	if ( LOT_LOG_POINTER[M] < 0 ) return 2;
	//
	va_start( va , list );
	_vsnprintf( TextBuffer , 1024 , list , (LPSTR) va );
	va_end( va );
	//
	_dWRITE_FUNCTION_EVENT_TH( LOT_LOG_POINTER[M] , TextBuffer );
	//
	return 0;
}

BOOL Preparing() { // 2019.03.13
	//
	while ( TRUE ) {
		//
		if ( TryEnterCriticalSection( &Event_Tunning_CR ) ) {
			//
			if ( _dREAD_FUNCTION_EVENT( -1001 ) <= 0 ) {
				//
				LeaveCriticalSection( &Event_Tunning_CR );
				//
				break;
			}
			else {
				LeaveCriticalSection( &Event_Tunning_CR );
			}
			//
		}
		//
		if ( !WAIT_SECONDS( 0.1 ) ) return FALSE;
		//
	}
	return TRUE;
}


//
Module_Status Program_Main() { // 2004.10.07
	int i , Res , CommStatus;
	int z1 , z2;
	char Retdata[1024+1];
	int startindex , prcheck;
	//
	if ( !Preparing() ) return SYS_ABORTED; // 2019.03.13
	//


//	CHECK_COUNT = 0;
//	printf("     RUNNNING [%d]\n", _dREAD_FUNCTION_EVENT(-1001));




	if ( RUNIO_ADDRESS >= 0 ) _dWRITE_DIGITAL( RUNIO_ADDRESS , 1 , &CommStatus ); // 2017.04.01
	//
	//==================================================================
	RECALL_INDEX = 0; // 2006.04.28
	//==================================================================
	// 2006.03.23
	//==================================================================
	EnterCriticalSection( &Event_Parameter_CR );
	//
	Event_Parameter_Received_Tag = 1;
	//
	z1 = 0;
//	z2 = STR_SEPERATE_CHAR_WITH_POINTER( PROGRAM_PARAMETER_READ() , '>' , Program_Param_Read_Buffer , z1 , 512 ); // 2010.09.02
	z2 = STR_SEPERATE_CHAR_WITH_POINTER( PROGRAM_PARAMETER_READ() , '>' , Program_Param_Read_Buffer , z1 , 1024 ); // 2010.09.02
	if ( z1 != z2 ) {
		while( TRUE ) {
			z1 = z2;
			z2 = STR_SEPERATE_CHAR_WITH_POINTER( PROGRAM_PARAMETER_READ() , '>' , Retdata , z1 , 1024 );
			if ( z1 == z2 ) break;
			if ( strlen( Retdata ) > 0 ) {
				if ( Event_Parameter_Received_Tag == 1 ) {
					Event_Parameter_Received_Tag = 2;
					Event_Parameter_Received_Count = 0;
				}
				if ( Event_Parameter_Received_Count >= MAX_EVTRECV_COUNT ) {
					//------------------------------------------------
					_IO_CONSOLE_PRINTF( "[RUN]\t%s REJECT BY OVERFLOW DATA\n" , PROGRAM_PARAMETER_READ() );
					//------------------------------------------------
				}
				else {
					//------------------------------------------------
//					strncpy( Event_Parameter_Data[Event_Parameter_Received_Count] , Retdata , 512 ); // 2008.04.04
					STR_MEM_MAKE_COPY( &(Event_Parameter_Data[Event_Parameter_Received_Count]) , Retdata ); // 2008.04.04
					Event_Parameter_Mode[Event_Parameter_Received_Count] = 0; // 2010.03.10
					//------------------------------------------------
					Event_Parameter_Received_Count++;
					//------------------------------------------------
				}
			}
		}
	}
	LeaveCriticalSection( &Event_Parameter_CR );
	//
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	File_Save_Tune_Data_Setting_XCR( -99 , 0 ); // 2013.03.28
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	//
	if ( MULTI_FILE_MIXING && ( Event_Parameter_Received_Count > 0 ) ) {
		//====================================================================================================
		// 2007.04.06
		//====================================================================================================
		if ( PRE_RUN_FUNCTION != -1 ) {
			if ( _dRUN_FUNCTION( PRE_RUN_FUNCTION , STR_MEM_GET_STR( PRE_RUN_FUNCTION_STR ) ) != SYS_SUCCESS ) {
				//
				//
				//-------------------------------------------------------------------
				//
				EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
				//
				//-------------------------------------------------------------------
				//
				if ( TUNE_DATA_FIXING == 0 ) { // 2018.12.07
					TUNE_COUNT = 0; // 2007.06.11
				}
				//
				//-------------------------------------------------------------------
				//
				LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
				//
				//-------------------------------------------------------------------
				//
				if ( RUNIO_ADDRESS >= 0 ) _dWRITE_DIGITAL( RUNIO_ADDRESS , 0 , &CommStatus ); // 2017.04.01
				//
				return SYS_ABORTED;
			}
		}
		prcheck = FALSE;
		Res = Program_Main3();
		//====================================================================================================
	}
	else {
		//==================================================================
	//	Event_Parameter_Received_Tag = 1; // 2006.03.23
	//	strncpy( Program_Param_Read_Buffer , PROGRAM_PARAMETER_READ() , 2048 ); // 2006.03.22
	//	strncpy( Program_Param_Read_Buffer , PROGRAM_PARAMETER_READ() , 512 ); // 2006.03.22

		//==================================================================
		startindex = 0; // 2006.10.31
		//==================================================================
		while( TRUE ) {
			//
			Res = Program_Main2( startindex , &prcheck );
			//
			startindex++;
			//
			EnterCriticalSection( &Event_Parameter_CR );
			if ( Res == SYS_SUCCESS ) {
	//			if ( Event_Parameter_Received_Tag == 2 ) { // 2006.03.22
				if ( ( Event_Parameter_Received_Tag == 2 ) && ( Event_Parameter_Received_Count > 0 ) ) {
					//========================================================================
					_IO_CONSOLE_PRINTF( "APPEND RUN CONFIRMED [%s][%d][M=%d][LT=%d][LS=%d][LC=%d][LF=%s]\n" , Event_Parameter_Data[0] , LAST_RUN_CHECK , Event_Parameter_Mode[0] , LAST_RUN_TYPE , LAST_RUN_SLOT , LAST_RUN_CASS , LAST_RUN_FILE );
					//========================================================================
					if ( LAST_RUN_CHECK ) {
						if ( Event_Parameter_Mode[0] == 1 ) { // 2010.03.14
							/*
							// 2011.03.01
							if ( ( LAST_RUN_TYPE == 2 ) || ( LAST_RUN_TYPE == 11 ) || ( LAST_RUN_TYPE == 21 ) ) { // pre
								if ( LAST_RUN_SLOT <= 0 ) {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM PRE Process End(A) at [%s]%cPROCESS_PRE_END ::%s:%d\n" , LAST_RUN_FILE , 9 , LAST_RUN_FILE , 100000 );
								}
								else {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM PRE Process End(A) at (D%d)[%s]%cPROCESS_PRE_END :D%d:%s:%d\n" , LAST_RUN_SLOT , LAST_RUN_FILE , 9 , LAST_RUN_SLOT , LAST_RUN_FILE , 100000 );
								}
							}
							else {
								if ( LAST_RUN_SLOT <= 0 ) {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM POST Process End(A) at [%s]%cPROCESS_POST_END ::%s:%d\n" , LAST_RUN_FILE , 9 , LAST_RUN_FILE , 100000 );
								}
								else {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM POST Process End(A) at (D%d)[%s]%cPROCESS_POST_END :D%d:%s:%d\n" , LAST_RUN_SLOT , LAST_RUN_FILE , 9 , LAST_RUN_SLOT , LAST_RUN_FILE , 100000 );
								}
							}
							*/
							//
							// 2011.03.01
							if ( LAST_RUN_TYPE == 2 ) { // Wafer Pre
								if ( LAST_RUN_SLOT <= 0 ) {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM Pre Process End(A) at [%s]%cPROCESS_PRE_END ::%s:%d\n" , LAST_RUN_FILE , 9 , LAST_RUN_FILE , 100000 );
								}
								else {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM Pre Process End(A) at (D%d)[%s]%cPROCESS_PRE_END :D%d:%s:%d\n" , LAST_RUN_SLOT , LAST_RUN_FILE , 9 , LAST_RUN_SLOT , LAST_RUN_FILE , 100000 );
								}
							}
							else if ( ( LAST_RUN_TYPE == 11 ) || ( LAST_RUN_TYPE == 21 ) ) { // Lot Pre
								if ( LAST_RUN_SLOT <= 0 ) {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM PRE Process End(A) at [%s]%cPROCESS_PRE_END ::%s:%d\n" , LAST_RUN_FILE , 9 , LAST_RUN_FILE , 100000 );
								}
								else {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM PRE Process End(A) at (D%d)[%s]%cPROCESS_PRE_END :D%d:%s:%d\n" , LAST_RUN_SLOT , LAST_RUN_FILE , 9 , LAST_RUN_SLOT , LAST_RUN_FILE , 100000 );
								}
							}
							else if ( LAST_RUN_TYPE == 1 ) { // Wafer Post
								if ( LAST_RUN_SLOT <= 0 ) {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM Post Process End(A) at [%s]%cPROCESS_POST_END ::%s:%d\n" , LAST_RUN_FILE , 9 , LAST_RUN_FILE , 100000 );
								}
								else {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM Post Process End(A) at (D%d)[%s]%cPROCESS_POST_END :D%d:%s:%d\n" , LAST_RUN_SLOT , LAST_RUN_FILE , 9 , LAST_RUN_SLOT , LAST_RUN_FILE , 100000 );
								}
							}
							else { // Lot Post
								if ( LAST_RUN_SLOT <= 0 ) {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM POST Process End(A) at [%s]%cPROCESS_POST_END ::%s:%d\n" , LAST_RUN_FILE , 9 , LAST_RUN_FILE , 100000 );
								}
								else {
									i = _R_SCH_LOG_LOT_PRINTF( LAST_RUN_CASS - 1 , "PM POST Process End(A) at (D%d)[%s]%cPROCESS_POST_END :D%d:%s:%d\n" , LAST_RUN_SLOT , LAST_RUN_FILE , 9 , LAST_RUN_SLOT , LAST_RUN_FILE , 100000 );
								}
							}
							//========================================================================
							if ( i != 0 ) {
								_IO_CONSOLE_PRINTF( "APPEND RUN CONFIRMED LOG FAIL [%d] [LT=%d][LS=%d][LC=%d]\n" , i , LAST_RUN_TYPE , LAST_RUN_SLOT , LAST_RUN_CASS );
							}
							//========================================================================
						}
					}
					//========================================================================
	//				strncpy( Program_Param_Read_Buffer , Event_Parameter_Data , 2048 ); // 2006.03.22
					//========================================================================
					strncpy( Program_Param_Read_Buffer , STR_MEM_GET_STR(Event_Parameter_Data[0]) , 1024 ); // 2006.03.22
					//========================================================================
					if ( Event_Parameter_Mode[0] == 1 ) {
						//
						//-------------------------------------------------------------------
						//
						EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
						//
						//-------------------------------------------------------------------
						//
						if ( TUNE_DATA_FIXING == 0 ) { // 2018.12.07
							TUNE_COUNT = 0; // 2010.08.20
						}
						//
						//-------------------------------------------------------------------
						//
						LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
						//
						//-------------------------------------------------------------------
						//
					}
					//========================================================================
					Event_Parameter_Received_Count--;
					//
					for ( i = 0 ; i < Event_Parameter_Received_Count ; i++ ) {
//						strcpy( Event_Parameter_Data[i] , Event_Parameter_Data[i+1] ); // 2008.04.04
						STR_MEM_MAKE_COPY( &(Event_Parameter_Data[i]) , Event_Parameter_Data[i+1] ); // 2008.04.04
						Event_Parameter_Mode[i] = Event_Parameter_Mode[i+1]; // 2010.03.10
					}
					//========================================================================
					LeaveCriticalSection( &Event_Parameter_CR );
					continue;
				}
			}
			Event_Parameter_Received_Tag = 0;
			LeaveCriticalSection( &Event_Parameter_CR );
			break;
		}
	}
	//========================================================
	// 2006.11.14
	//========================================================
	if ( ( !prcheck ) && ( Res != SYS_ABORTED ) ) {
		if ( POST_RUN_FUNCTION != -1 ) {
			if ( _dRUN_FUNCTION( POST_RUN_FUNCTION , STR_MEM_GET_STR( POST_RUN_FUNCTION_STR ) ) != SYS_SUCCESS ) {
				//
				//
				//-------------------------------------------------------------------
				//
				EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
				//
				//-------------------------------------------------------------------
				//
				if ( TUNE_DATA_FIXING == 0 ) { // 2018.12.07
					TUNE_COUNT = 0; // 2007.06.11
				}
				//
				//-------------------------------------------------------------------
				//
				LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
				//
				//-------------------------------------------------------------------
				//
				if ( RUNIO_ADDRESS >= 0 ) _dWRITE_DIGITAL( RUNIO_ADDRESS , 0 , &CommStatus ); // 2017.04.01
				return SYS_ABORTED;
			}
		}
	}
	//========================================================
	//
	//-------------------------------------------------------------------
	//
	EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	//
	if ( TUNE_DATA_FIXING == 0 ) { // 2018.12.07
		TUNE_COUNT = 0; // 2007.06.11
	}
	//
	//-------------------------------------------------------------------
	//
	LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//
	//-------------------------------------------------------------------
	//
	//========================================================
	//
	if ( RUNIO_ADDRESS >= 0 ) _dWRITE_DIGITAL( RUNIO_ADDRESS , 0 , &CommStatus ); // 2017.04.01
	//
	return Res;


}
//
void Program_Enter_Code() {
	int i;
	char    FIO_NAME[ 63+1 ];
	//---------------------------------------------------
	char Buffer[32];
	//---------------------------------------------------
	sprintf( Buffer , "<BUILD>%s %s" , __DATE__ , __TIME__ );	REGIST_FILE_INFO( Buffer );
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.0(2002.01.01)" ); // CimSqCon Change Rebuild
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.1(2002.02.01)" ); // Recipe Locking name Conversion
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.2(2002.03.25)" ); // Upload data transfer
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.3(2002.07.16)" ); // Append Message for NextPM
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.4(2002.08.19)" ); // Run_Function change to Poll Time & append wait
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.5(2003.05.22)" ); // File Name Check Part Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.6(2003.07.22)" ); // Precheck Message append
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.7(2003.10.28)" ); // Console Log Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.8(2004.01.26)" ); // Rcp Overflow Error Modify
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.81(2004.05.10)" ); // Rcp Overflow Max Size Modify
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.9(2004.10.07)" ); // Event Append Action Append for Lot Post happen
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.91(2005.08.04)" ); // File Not Open Message Bug Update
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.92(2005.08.18)" ); // MinTime Check Part Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.93(2005.10.21)" ); // Multi Index Message Tag Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.94(2005.10.27)" ); // _FC Function not Used
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.95(2006.03.22)" ); // Multiple Event Receive Possible
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.96(2006.04.06)" ); // Lot Info Data Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.97(2006.04.14)" ); // Append File Step Cut & UPLOAD Filtering
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.98(2006.10.31)" ); // Append Pre Run Function Part
//	REGIST_FILE_INFO( "RecipeHandler(2) 3.99(2006.11.14)" ); // Append Post Run Function Part
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.0(2007.03.07)" ); // File Name Not Use Mode Part Bug Update
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.1(2007.04.06)" ); // Append Multi File Mixed Mode
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.2(2007.06.01)" ); // Append Tune Info Check include MIndex
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.21(2007.10.04)" ); // Append not define File Include part skip
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.22(2007.10.15)" ); // Pre Check Part Bug Update
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.3(2008.04.04)" ); // Memoty optimize
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.31(2008.05.30)" ); // Floating point overflow bug update
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.32(2009.06.30)" ); // Error Log Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.33(2010.03.14)" ); // Log Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.34(2010.08.20)" ); // Event Tune
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.35(2010.11.02)" ); // File Type Read
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.36(2011.03.01)" ); // Log Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.37(2011.05.18)" ); // Tune Remove
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.38(2012.07.05)" ); // Filename '$' Bug
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.39(2012.08.23)" ); // Filename ':' ' ' Bug
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.4(2013.03.28)" ); // Tune Data Save
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.41(2013.04.28)" ); // Tune Data Save IO
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.42(2013.10.11)" ); // /MD Rebuild
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.43(2014.01.10)" ); // File return/Alarm
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.44(2014.04.22)" ); // Filename ' ' Bug in locking
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.45(2015.02.13)" ); // GetLastError()
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.46(2015.04.27)" ); // invalid message Check
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.47(2015.05.05)" ); // SendBuffer Buffer Size bug
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.5(2015.07.20)" ); // SHMEM access Append
//	REGIST_FILE_INFO( "RecipeHandler(2) 4.51(2016.04.11)" ); // Error Monitor bug Update
//	REGIST_FILE_INFO( "RecipeHandler(2) 5.0(2017.04.01)" ); // RUNIO 기능 , TIME_FUNCTION Gateway 기능 추가
//	REGIST_FILE_INFO( "RecipeHandler(2) 5.1(2017.08.20)" ); // Buffer Size Option
	REGIST_FILE_INFO( "RecipeHandler(2) 5.2(2019.03.13)" ); // Tunning Data Set 충돌 문제

	//---------------------------------------------------
	for ( i = 0 ; i < 10 ; i++ ) { // 2008.04.04
		FUNCTION_FILE_STRING[i] = NULL;
	}
	//---------------------------------------------------
	for ( i = 0 ; i < MAX_EVTRECV_COUNT ; i++ ) { // 2008.04.04
		Event_Parameter_Data[i] = NULL;
	}
	//---------------------------------------------------
	for ( i = 0 ; i < MAX_TUNE_COUNT ; i++ ) {
		TUNE_NAME[i] = NULL; // 2007.06.08
		TUNE_DATA[i] = NULL; // 2007.06.08
	}
	//---------------------------------------------------
	for ( i = 0 ; i < MAX_UPFILT_COUNT ; i++ ) { // 2013.03.28
		UPFILT_NAME[i] = NULL;
		UPFILT_DATA[i] = NULL;
	}
	//---------------------------------------------------
	TUNE_FILE_INIT(); // 2010.08.20
	//---------------------------------------------------
//	strcpy( FILE_ID , "" ); // 2008.04.04
//	strcpy( RECIPE_ID , "" ); // 2008.04.04
	MAX_STEP_COUNT = 199;
//	strcpy( OPTION , "" ); // 2008.04.04

	InitializeCriticalSection( &Event_Parameter_CR ); // 2004.10.07
	//
	InitializeCriticalSection( &Event_Tunning_CR ); // 2019.03.13
	//

	Global_Buffer_Init();	// 2004.01.27

	OPTION = calloc( 63 + 1 , sizeof( char ) );

	if ( OPTION == NULL ) {
		MAIN_STATUS = FALSE;
		//
		_IO_CIM_PRINTF( "Memory Initialize Error - %s\n" , PROGRAM_ARGUMENT_READ() );
	}
	else {
		STR_SEPERATE_CHAR( PROGRAM_ARGUMENT_READ() , '|' , FIO_NAME , OPTION , 63 );
		//
		if ( STR_MEM_SIZE( OPTION ) <= 0 ) STR_MEM_MAKE_COPY( &OPTION , "" );
		//
		MAIN_STATUS = SETUP_FILE( FIO_NAME );
		//
		if ( !MAIN_STATUS ) {
			_IO_CIM_PRINTF( "Setup Information have Error(s) - %s\n" , PROGRAM_ARGUMENT_READ() );
		}
		else { // 2015.07.20
			//
			if ( FILE_READ_MODE != 1 ) {
				//
				i = _FILE2SM_INTERFACE_INITIALIZING( GET_SYSTEM_NAME() );	// 0:OK 1:AlreadyInit 2:CreateMutexFail 3:WaitObjectFail 4:OpenFileFail 5:CreateFileFail 6:MultiMutexFail
				//
				if ( i != 0 ) {
					//
					Sleep(500);
					//
					i = _FILE2SM_INTERFACE_INITIALIZING( GET_SYSTEM_NAME() );	// 0:OK 1:AlreadyInit 2:CreateMutexFail 3:WaitObjectFail 4:OpenFileFail 5:CreateFileFail 6:MultiMutexFail
					//
					if ( i != 0 ) {
						//
						MAIN_STATUS = FALSE;
						//
						_IO_CIM_PRINTF( "_FILE2SM_INTERFACE_INITIALIZING Error (%d) - %s\n" , i , PROGRAM_ARGUMENT_READ() );
						//
					}
					else {
						FILE_SHMEM_INIT = TRUE;
					}
					//
				}
				else {
					FILE_SHMEM_INIT = TRUE;
				}
				//
			}
			//
		}
		//
	}
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
void TUNE_DATA_SAVE_IO_MAKE() { // 2013.04.28
	int c;
	//
	if ( TUNE_DATA_SAVE_IO >= 0 ) {
		_dWRITE_DIGITAL( TUNE_DATA_SAVE_IO , TUNE_DATA_SAVING , &c );
	}
	//
}


BOOL Program_Init_Code() {
	TUNE_DATA_SAVE_IO_MAKE(); // 2013.04.28
	return MAIN_STATUS;
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------


//int xxxx = 0;
//






void Event_Message_Received() {
	int Res , i , j , z , z2 , ec;
	char Buffer[1024];
	char Buffer1[1024];
	char Buffer2[1024];
	char Buffer3[1024]; // 2007.06.01

	_IO_CONSOLE_PRINTF( "[EVENT]\t%s\n" , PROGRAM_EVENT_READ() ); // 2003.10.28

	if ( PROGRAM_EVENT_READ()[0] == '$' ) {
		//
		//-------------------------------------------------------------------
		//
		EnterCriticalSection( &Event_Tunning_CR ); // 2019.03.13
		//
		//-------------------------------------------------------------------
		//


//		xxxx++;	if (xxxx > 50) xxxx = 0;
//		if (xxxx != 0) Sleep(xxxx);





		z = 0;
		z2 = 0;
		ec = 0;
		//
		if ( TUNE_DATA_FIXING != 2 ) { // 2018.12.07
			TUNE_COUNT = 0;
		}
		//
		while( TRUE ) {
			z2 = STR_SEPERATE_CHAR_WITH_POINTER( PROGRAM_EVENT_READ() + 1 , '|' , Buffer , z , 1023 );
			if ( z == z2 ) break;
			//
			STR_SEPERATE_CHAR( Buffer  , ':' , Buffer1 , Buffer2 , 1023 );
			//
			Res = atoi( Buffer1 );
			//
			if ( ( strlen( Buffer1 ) > 0 ) && ( Res >= 0 ) && ( Res < MAX_STEP_USE_COUNT) ) {
				//
				STR_SEPERATE_CHAR( Buffer2 , ':' , Buffer1 , Buffer  , 1023 );
				//
				STR_SEPERATE_CHAR( Buffer  , ':' , Buffer2 , Buffer3 , 1023 ); // 2007.06.01
				//
				if ( strlen( Buffer1 ) > 0 ) {
					if ( TUNE_COUNT >= MAX_TUNE_COUNT ) {
						_IO_CIM_PRINTF( "TUNE DATA AREA has no space - [%d] [Step %d,%s=%s,%d] is Truncated\n" , ec + 1 , Res , Buffer1 , Buffer2 , ( strlen( Buffer3 ) <= 0 ) ? -1 : atoi( Buffer3 ) );
						ec++;
					}
					else {
						//=================================================================================
						// 2007.06.08
						//=================================================================================
						if ( TUNE_NAME[ TUNE_COUNT ] == NULL ) {
							TUNE_NAME[TUNE_COUNT] = calloc( 127 + 1 , sizeof( char ) );
							if ( TUNE_NAME[ TUNE_COUNT ] == NULL ) {
								_IO_CIM_PRINTF( "TUNE DATA AREA has no Memory 3 - [Step %d,%s=%s,%d] is Truncated\n" , Res , Buffer1 , Buffer2 , ( strlen( Buffer3 ) <= 0 ) ? -1 : atoi( Buffer3 ) );
								break;
							}
						}
						if ( TUNE_DATA[ TUNE_COUNT ] == NULL ) {
							TUNE_DATA[TUNE_COUNT] = calloc( 127 + 1 , sizeof( char ) );
							if ( TUNE_DATA[ TUNE_COUNT ] == NULL ) {
								_IO_CIM_PRINTF( "TUNE DATA AREA has no Memory 4 - [Step %d,%s=%s,%d] is Truncated\n" , Res , Buffer1 , Buffer2 , ( strlen( Buffer3 ) <= 0 ) ? -1 : atoi( Buffer3 ) );
								break;
							}
						}
						//=================================================================================
						TUNE_STEP[TUNE_COUNT] = Res;
						//=================================================================================
						strncpy( TUNE_NAME[TUNE_COUNT] , Buffer1 , 127 );
						TUNE_NAME[TUNE_COUNT][127] = 0; // 2019.03.13
	//					strncpy( TUNE_DATA[TUNE_COUNT] , Buffer , 127 ); // 2007.06.01
						strncpy( TUNE_DATA[TUNE_COUNT] , Buffer2 , 127 ); // 2007.06.01
						TUNE_DATA[TUNE_COUNT][127] = 0; // 2019.03.13
						//============================================================================
						// 2007.06.01
						//============================================================================
						if ( strlen( Buffer3 ) <= 0 ) {
							TUNE_MIDX[TUNE_COUNT] = -1;
						}
						else {
							TUNE_MIDX[TUNE_COUNT] = atoi( Buffer3 );
						}
						//============================================================================
						// 2007.06.07
						//============================================================================
						TUNE_MODE[TUNE_COUNT] = 1; // 2007.06.07
						//============================================================================
						TUNE_COUNT++;
					}
				}
			}
			z = z2;
		}
		//


//printf("RECEIVED [SLEEP=%d]\n", xxxx);


		Find_Tune_Data_Remap_XCR(); // 2010.09.07
		//

//printf("MAPPED\n");



		//
		//-------------------------------------------------------------------
		//
		LeaveCriticalSection( &Event_Tunning_CR ); // 2019.03.13
		//
		//-------------------------------------------------------------------
	}
	else {
		STR_SEPERATE( PROGRAM_EVENT_READ() , Buffer , Buffer1 , 1023 ); // 2004.10.07
		if ( STRCMP_L( Buffer , "APPEND" ) ) { // 2004.10.07
			EnterCriticalSection( &Event_Parameter_CR );
			//=================================================================================================
			if      ( Event_Parameter_Received_Tag == 0 ) { // not run
				//------------------------------------------------
				_IO_CONSOLE_PRINTF( "[EVENT]\t%s REJECT BY NOTRUN\n" , PROGRAM_EVENT_READ() );
				//------------------------------------------------
				PROGRAM_EVENT_STATUS_SET( SYS_ERROR );
				//------------------------------------------------
			}
			else if ( ( Event_Parameter_Received_Tag == 1 ) || ( Event_Parameter_Received_Tag == 2 ) ) { // run
				//------------------------------------------------
				if ( Event_Parameter_Received_Tag == 1 ) {
					Event_Parameter_Received_Tag = 2;
					Event_Parameter_Received_Count = 0; // 2006.03.22
				}
				if ( Event_Parameter_Received_Count >= MAX_EVTRECV_COUNT ) { // 2006.03.22
					//------------------------------------------------
					PROGRAM_EVENT_STATUS_SET( SYS_ABORTED );
					//------------------------------------------------
					_IO_CONSOLE_PRINTF( "[EVENT]\t%s REJECT BY OVERFLOW DATA\n" , PROGRAM_EVENT_READ() );
					//------------------------------------------------
				}
				else {
					//------------------------------------------------
//					strncpy( Event_Parameter_Data[Event_Parameter_Received_Count] , Buffer1 , 512 ); // 2008.04.04
					STR_MEM_MAKE_COPY( &(Event_Parameter_Data[Event_Parameter_Received_Count]) , Buffer1 ); // 2008.04.04
					Event_Parameter_Mode[Event_Parameter_Received_Count] = 1; // 2010.03.10
					//------------------------------------------------
					Event_Parameter_Received_Count++; // 2006.03.22
					//------------------------------------------------
					_IO_CONSOLE_PRINTF( "[EVENT]\t%s OK\n" , PROGRAM_EVENT_READ() );
					//------------------------------------------------
					PROGRAM_EVENT_STATUS_SET( SYS_SUCCESS );
					//------------------------------------------------
				}
			}
			else {
				//------------------------------------------------
				PROGRAM_EVENT_STATUS_SET( SYS_ABORTED );
				//------------------------------------------------
				_IO_CONSOLE_PRINTF( "[EVENT]\t%s REJECT BY ABORTED\n" , PROGRAM_EVENT_READ() );
				//------------------------------------------------
			}
			//=================================================================================================
			LeaveCriticalSection( &Event_Parameter_CR );
		}
		else if ( STRCMP_L( Buffer , "TIME_FINISH" ) ) { // 2006.03.27
			TIME_FINISH = TRUE; // 2006.03.27
		}
		else if ( STRCMP_L( Buffer , "TUNE_FILE" ) ) { // 2010.08.20
			//
			z = 0;
			z2 = 0;
			//
			z2 = STR_SEPERATE_CHAR_WITH_POINTER( Buffer1 , '|' , Buffer , z , 1023 );
			if ( z == z2 ) return;
			//
			z = z2;
			//
			Res = atoi( Buffer );
			//
			z2 = STR_SEPERATE_CHAR_WITH_POINTER( Buffer1 , '|' , Buffer , z , 1023 );
			if ( z == z2 ) {
				TUNE_FILE_DELETE_OCR( Res );
				return;
			}
			z = z2;
			//
			i = atoi( Buffer );
			//
			z2 = STR_SEPERATE_CHAR_WITH_POINTER( Buffer1 , '|' , Buffer , z , 1023 );
			if ( z == z2 ) return;	z = z2;
			//
			j = atoi( Buffer );
			//
			z2 = STR_SEPERATE_CHAR_WITH_POINTER( Buffer1 , '|' , Buffer , z , 1023 );
			if ( z == z2 ) return; z = z2;
			//
			TUNE_FILE_ADD_OCR( Res , i , j , Buffer );
			//
		}
		else if ( STRCMP_L( Buffer , "TUNEDATASAVE" ) ) { // 2013.04.28
			if ( STRCMP_L( Buffer1 , "ON" ) ) {
				TUNE_DATA_SAVING = TRUE;
			}
			else {
				TUNE_DATA_SAVING = FALSE;
			}
			//
			TUNE_DATA_SAVE_IO_MAKE();
			//
		}
		else if ( STRCMP_L( Buffer , "TUNEDATAFIX" ) ) { // 2018.12.07
			//
			if ( STRCMP_L( Buffer1 , "ON" ) ) {
				TUNE_DATA_FIXING = 1;
			}
			else if ( STRCMP_L( Buffer1 , "APPEND" ) ) {
				TUNE_DATA_FIXING = 2;
			}
			else {
				TUNE_DATA_FIXING = 0;
			}
			//
		}
		else {
			if ( FUNCTION_ADDRESS[0] < 0 ) Res = SYS_ABORTED;
			else {
				_dWRITE_FUNCTION_EVENT( FUNCTION_ADDRESS[0] , PROGRAM_EVENT_READ() );
				while( TRUE ) {
					Res = _dREAD_FUNCTION_EVENT( FUNCTION_ADDRESS[0] );
					if ( Res != SYS_RUNNING ) break;
//					_sleep(10); // 2013.03.28
					Sleep(10); // 2013.03.28
				}
			}
			PROGRAM_EVENT_STATUS_SET( Res );
		}
	}
}

//---------------------------------------------------------------------------------------
void Screen_Nav_Info( int no ) {
}
//---------------------------------------------------------------------------------------
