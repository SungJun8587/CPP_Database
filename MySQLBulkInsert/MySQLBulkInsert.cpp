// MySQLBulkInsert.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"

constexpr int poolSize = 5;             // 고정된 크기의 DB 커넥션 풀. 스레드도 DB 커넥션 수(poolSize)만큼 생성.
constexpr int batchCount = 10000;		// 한 번에 삽입할 데이터 수

thread_local int threadDBIndex = -1;						// 스레드별 DB 풀 고유 인덱스를 부여

std::vector<std::unique_ptr<CBaseMySQL>> connectionPool;    // DB 커넥션 풀(고정 크기)
std::atomic<int32> nextIndex { 0 };							// 원자적 인덱스 변수

//***************************************************************************
// [libmysql의 멀티스레드 지원 여부]
//	- MySQL C API(libmysql) 자체는 멀티스레드 안전(thread - safe)하게 설계되어 있음.
//	- 그러나 하나의 MYSQL* 객체를 여러 스레드에서 공유하면 안전하지 않음.
//	- 따라서 멀티스레드에서 MYSQL* 객체를 공유하지 않고, 커넥션 풀을 사용해야 함.
// 
// [구현 방법]
//  - 스레드별로 원자적 연산을 사용하여 고유 인덱스를 부여. 즉 스레드별로 다른 커넥션을 사용.
CBaseMySQL* GetDBConnection()
{
	if( threadDBIndex == -1 ) 
	{
		// nextIndex를 원자적으로 증가시키고, poolSize로 나눈 나머지를 인덱스로 사용(0부터 poolSize까지 int형 인덱스)
		// std::atomic::fetch_add()를 사용하여 동기화 없이 원자적으로 인덱스를 증가
		threadDBIndex = nextIndex.fetch_add(1, std::memory_order_relaxed) % poolSize;
	}
	return connectionPool[threadDBIndex].get();		// 스레드에 해당하는 고유 인덱스로 커넥션 반환
}

// 스레드별 MySQL 작업 수행
void MySQLWorkerThread()
{
	bool	bResult = true;
	TCHAR	tszName1[50];
	TCHAR	tszName2[50];
	bool	bFlag[batchCount];
	int32   nAge[batchCount];

	CBaseMySQL* pConn = GetDBConnection();
	if( pConn == nullptr ) return;

	try
	{
		if( !pConn->AutoCommit(false) ) throw 0;

		// INSERT INTO Consumer(Name1, Name2, Flag, Age) VALUES 문자열 뒤에 batchCount 만큼 (?, ?, ?, ?) 문자열을 생성
		//	Ex) INSERT INTO Consumer(Name1, Name2, Flag, Age) VALUES (?, ?, ?, ?), (?, ?, ?, ?), ..., (?, ?, ?, ?); 
		std::ostringstream query;
		query << "INSERT INTO Consumer(Name1, Name2, Flag, Age) VALUES ";
		for( int i = 0; i < batchCount; i++ )
		{
			if( i > 0 ) query << ", ";
			query << "(? , ? , ? , ?)";
		}
		query << ";";

		if( !pConn->Prepare(query.str().c_str()) ) throw 0;

		int colCount = 4;
		int paramCount = batchCount * colCount;
		std::vector<MYSQL_BIND> bindParams(paramCount);
		for( int i = 0; i < batchCount; i++ )
		{
			_stprintf_s(tszName1, 50, _T("제목_%d"), i + 1);
			_stprintf_s(tszName2, 50, _T("내용_%d"), i + 1);
			if( i % 2 == 0 )
				bFlag[i] = true;
			else bFlag[i] = false;
			nAge[i] = 20 + ((i + 1) % 50);

			bindParams[i * colCount] = CBaseMySQL::BindParam(tszName1, (ulong)_tcslen(tszName1));
			bindParams[i * colCount + 1] = CBaseMySQL::BindParam(tszName2, (ulong)_tcslen(tszName2));
			bindParams[i * colCount + 2] = CBaseMySQL::BindParam(bFlag[i]);
			bindParams[i * colCount + 3] = CBaseMySQL::BindParam(nAge[i]);
		}

		bResult = pConn->PrepareBindParam(bindParams);
		if( bResult ) bResult = pConn->PrepareExecute();

		for( const MYSQL_BIND& bindParam : bindParams )
			pConn->ClearBindParam(bindParam);

		if( !bResult ) throw 0;
	}
	catch( ... )
	{
		pConn->Rollback();
		std::cout << "Transaction Rollback." << std::endl;
	}

	pConn->StmtClose();
	pConn->Commit();
}

int main()
{
#ifdef	_MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    mysql_library_init(0, NULL, NULL);		// MySQL 라이브러리 초기화(프로그램에서 단 한 번만 호출)

    // DB 커넥션 풀을 poolSize 만큼 생성
    for( int i = 0; i < poolSize; i++ )
    {
        CBaseMySQL *pMySQLConns = new CBaseMySQL("127.0.0.1", "root", "1234", "common", 3306);
        if( pMySQLConns->Connect() )
        {
            connectionPool.emplace_back(pMySQLConns);
        }
    }

	// 스레드를 DB 커넥션 수(poolSize)만큼 생성하여, 스레드 각각 DB 커넥션을 소유하게 함.
    std::vector<std::thread> threads;
    for( int i = 0; i < poolSize; i++ )    
    { 
        threads.emplace_back(MySQLWorkerThread);
    }

	std::cout << "Thread " << poolSize << " threads started." << std::endl;
	std::cout << "It is processing. Please wait a moment." << std::endl;

    for( auto& t : threads ) 
    {
        t.join();
    }

	std::cout << "Thread processing completed." << std::endl;
	std::cout << poolSize << " threads have inserted " << poolSize * batchCount << " pieces of data." << std::endl;

    mysql_library_end();					// MySQL 라이브러리 메모리 정리(프로그램 종료 시 한 번만 호출)

    system("pause");
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
