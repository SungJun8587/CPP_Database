
//***************************************************************************
// DBAsyncHandler.cpp : implementation of the DBAsyncHandler class.
//
//***************************************************************************

#include "pch.h"
#include "DBAsyncHandler.h"

DECLARE_DBASYNC_HANDLER(DBASYNC_BULKADD_CONSUMER_REQ)
{
	int32	iName1Size = 50;
	int32	iName2Size = 50;

	CONSUMER_DATA_BATCH_REQ* pDBParam = (CONSUMER_DATA_BATCH_REQ*)pStAsync;

	auto pOdbcConn = COdbcAsyncSrv::Instance()->GetAccountOdbcConnPool()->GetOdbcConn(gpThreadManager->getThreadID() - 1);
	if( nullptr == pOdbcConn )
		return EDBReturnType::INVALID;

	const uint64 rowCount = BATCH_SIZE;  // 삽입할 행의 수

	// 자동 커밋 모드 Off 설정
	pOdbcConn->SetAutoCommitMode(SQL_AUTOCOMMIT_OFF);

	pOdbcConn->InitStmtHandle();

	// 사용할 커서의 유형을 지정(동적 커서를 활성화)
	pOdbcConn->SetStmtAttr(SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_DYNAMIC, 0);

	// 커서의 동시성(concurrency) 제어 방식을 설정(행 버전 기반 동시성 제어)
	pOdbcConn->SetStmtAttr(SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_ROWVER, 0);

	// 행 단위 바인딩은 값을 결과 열이 바인딩될 버퍼의 길이 또는 버퍼의 길이로 설정
	pOdbcConn->SetStmtAttr(SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(CONSUMER_DATA), SQL_IS_UINTEGER);

	// 대량 작업 행 크기 설정(한 번에 삽입할 행 수)
	pOdbcConn->SetStmtAttr(SQL_ATTR_ROW_ARRAY_SIZE, reinterpret_cast<SQLPOINTER>(rowCount), 0);

	if( !pOdbcConn->ExecDirect(_T("SELECT Name1, Name2, Flag, Age FROM Consumer WHERE 1=0")) )
		return EDBReturnType::INVALID;

	iName1Size = static_cast<int32>(_tcslen(pDBParam->_consumers[0].tszName1));
	iName2Size = static_cast<int32>(_tcslen(pDBParam->_consumers[0].tszName2));

	pOdbcConn->BindCol(pDBParam->_consumers[0].tszName1, iName1Size);
	pOdbcConn->BindCol(pDBParam->_consumers[0].tszName2, iName2Size);
	pOdbcConn->BindCol(pDBParam->_consumers[0].bFlag);
	pOdbcConn->BindCol(pDBParam->_consumers[0].nAge);

	if( !pOdbcConn->BulkOperations(SQL_ADD) )
		return EDBReturnType::INVALID;

	pOdbcConn->Commit();

	pOdbcConn->UnBindColStmt();

	return EDBReturnType::OK;
}

