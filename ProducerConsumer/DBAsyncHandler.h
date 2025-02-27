
//***************************************************************************
// DBAsyncHandler.h : interface for the command##_handler class.
//
//***************************************************************************

#ifndef __DBASYNCHANDLER_H__
#define __DBASYNCHANDLER_H__

#ifndef __DBASYNCSRV__H__
#include <DBAsyncSrv.h>
#endif

#ifndef __ODBCASYNCSRV__H__
#include <OdbcAsyncSrv.h>
#endif

// 프로토콜 생성 및 바인더에 추가
#define DECLARE_DBASYNC_HANDLER(command) \
class command##_handler : public CDBAsyncSrvHandler \
{\
public:\
	command##_handler(){} \
	virtual ~command##_handler(){} \
	virtual EDBReturnType ProcessAsyncCall(st_DBAsyncRq* pStAsync); \
	\
	static std::shared_ptr<CDBAsyncSrvHandler> asyncHandler; \
}; \
	shared_ptr<CDBAsyncSrvHandler> command##_handler::asyncHandler = COdbcAsyncSrv::Instance()->Regist(command, std::make_shared<command##_handler>()); \
	EDBReturnType command##_handler::ProcessAsyncCall(st_DBAsyncRq* pStAsync)

#endif // ndef __DBASYNCHANDLER_H__