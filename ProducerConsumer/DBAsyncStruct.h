
//***************************************************************************
// DBAsyncStruct.h : interface for the COdbcMainSvr class.
//
//***************************************************************************

#ifndef __DBASYNCSTRUCT_H__
#define __DBASYNCSTRUCT_H__

#pragma once

#pragma pack(push, 1)

#ifndef __DBASYNCSRV__H__
#include <DB/DBAsyncSrv.h>
#endif

#define MAX_ROWS		10000	// 
#define BATCH_SIZE		1000	// 
#define INTERVAL_SEC	5		// 데이터 생산 주기(초)

typedef struct _CONSUMER_DATA
{
	_CONSUMER_DATA()
	{
		nNo = 0;
		memset(tszName1, 0, sizeof(tszName1));
		memset(tszName2, 0, sizeof(tszName2));
		bFlag = false;
		nAge = 0;
	}

	uint32	nNo;
	TCHAR	tszName1[50];
	TCHAR	tszName2[50];
	bool	bFlag;
	int32   nAge;

} CONSUMER_DATA;


#define DBASYNC_BULKADD_CONSUMER_REQ 1
typedef struct _CONSUMER_DATA_BATCH_REQ : public st_DBAsyncRq
{
	_CONSUMER_DATA_BATCH_REQ()
	{
		memset(_consumers, 0, sizeof(_consumers));
	}

	CONSUMER_DATA	_consumers[BATCH_SIZE];
} CONSUMER_DATA_BATCH_REQ;

#pragma pack(pop)

#endif // ndef __DBASYNCSTRUCT_H__
