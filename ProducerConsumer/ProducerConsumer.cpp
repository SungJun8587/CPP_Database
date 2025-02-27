// ProducerConsumer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <conio.h>

#define MAKE_DB_ASYNC_CALL(STRUCT, CMD)\
{\
	STRUCT* pDBAsync = new STRUCT(); \
	pDBAsync->callIdent = CMD

#define PUSH_DB_ASYNC_CALL()\
	COdbcAsyncSrv::Instance()->Push(pDBAsync); \
}

static int			producerIndex = 1;
std::atomic<bool>	bStopProducerThread(false);		// Producer 스레드 종료 플래그

void ProducerThread()
{
	while( !bStopProducerThread.load() )
	{
		for( int i = 0; i < MAX_ROWS; i += BATCH_SIZE )
		{
			int currentBatchSize = BATCH_SIZE < (MAX_ROWS - i) ? BATCH_SIZE : MAX_ROWS - i;

			MAKE_DB_ASYNC_CALL(CONSUMER_DATA_BATCH_REQ, DBASYNC_BULKADD_CONSUMER_REQ);
			for( int j = 0; j < currentBatchSize; j++ )
			{
				pDBAsync->_consumers[j].nNo = i + j + 1;
				_stprintf_s(pDBAsync->_consumers[j].tszName1, 50, _T("제목_%d"), i + j + 1);
				_stprintf_s(pDBAsync->_consumers[j].tszName2, 50, _T("내용_%d"), i + j + 1);
				pDBAsync->_consumers[j].bFlag = false;
				pDBAsync->_consumers[j].nAge = i + j + 1;
			}
			PUSH_DB_ASYNC_CALL();
		}
		LOG_INFO(_T("[%d] %s data have been entered(Total : %s)"), producerIndex, addCommas(MAX_ROWS).c_str(), addCommas(producerIndex * MAX_ROWS).c_str());
		producerIndex++;
	
		for( int j = 0; j < INTERVAL_SEC; j++ )
		{
			if( bStopProducerThread.load() ) break;

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

int main()
{
#ifdef	_MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	TCHAR tszTempArgv[FULLPATH_STRLEN] = { 0, };

	//_sntprintf_s(tszTempArgv, FULLPATH_STRLEN, _TRUNCATE, _T("config\\server_config_mssql.json"));
	_sntprintf_s(tszTempArgv, FULLPATH_STRLEN, _TRUNCATE, _T("config\\server_config_mysql.json"));

	if( false == SERVER_CONFIG->Init(tszTempArgv) )
	{
		LOG_ERROR(_T("SERVER_CONFIG->Init Fail."));
		SERVER_CONFIG->ReleaseInstance();
		return -1;
	}

	if( false == COdbcAsyncSrv::Instance()->StartService(SERVER_CONFIG->GetDBNodeVec()) )	// DB Async Call Service 시작
	{
		LOG_ERROR(_T("Failed to COdbcAsyncSrv initialize."));
		SERVER_CONFIG->ReleaseInstance();
		return -1;
	}

	COdbcAsyncSrv::Instance()->StartIoThreads();
	std::cout << "Consumer " << COdbcAsyncSrv::Instance()->_nMaxThreadCnt << " threads started." << std::endl;

	gpThreadManager->CreateThread([=]() {
		ProducerThread();
	});
	std::cout << "Producer 1 thread started." << std::endl;

	std::cout << "It is processing. Please wait a moment." << std::endl;
	std::cout << "Press ESC to exit." << std::endl;

	while( 1 )
	{
		if( _kbhit() )	// 키 입력이 있을 경우
		{
			char key = _getch();	// 입력된 키를 가져옴
			if( key == 27 )			// ESC 키의 ASCII 코드 : 27
			{
				std::cout << "ESC pressed. Exiting..." << std::endl;
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // CPU 낭비 방지
	}

	bStopProducerThread = true;
	COdbcAsyncSrv::Instance()->StopThread();

	// wait main thread & check DEADLOCK
	gpThreadManager->JoinThreads();

	std::cout << "Thread processing completed." << std::endl;

	SERVER_CONFIG->ReleaseInstance();

	system("pause");

	return 0;
}


// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
