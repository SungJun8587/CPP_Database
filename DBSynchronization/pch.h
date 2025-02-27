﻿// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include <windows.h>
#include <atlbase.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <fstream>
#include <iostream>

// 여기에 미리 컴파일하려는 헤더 추가
#include <BaseDefine.h>
#include <BaseRedefineDataType.h>
#include <BaseTLS.h>
#include <BaseMacro.h>

#pragma comment(lib, LIB_NAME("libiconv"))
#pragma comment(lib, LIB_NAME("xlnt"))

#include <iconv.h>
#include <xlnt/xlnt.hpp>

#include <Memory/MemoryPool.h>
#include <Memory/Memory.h>
#include <Memory/CustomAllocator.h>
#include <Memory/Containers.h>
#include <Memory/ObjectPool.h>
#include <Memory/Singleton.h>
#include <Memory/MemBuffer.h>

#include <Util/IconvUtil.h>
#include <Util/ConvertCharset.h>
#include <Util/CommonUtil.h>
#include <Util/StringUtil.h>
#include <Util/FileUtil.h>
#include <Util/ShellUtil.h>
#include <Util/Log.h>

#include <BaseGlobal.h>

#include <JSON/RapidJSONUtil.h>
#include <XML/RapidXMLUtil.h>

#include <Excel/XlntUtil.h>

#include <ServerConnectInfo.h>
#include <ServerConfig.h>

#include <DB/DBCommon.h>
#include <DB/BaseODBC.h>
#include <DB/DBBind.h>
#include <DB/DBModel.h>
#include <DB/DBSyncBind.h>
#include <DB/DBQueryProcess.h>
#include <DB/DBSchema.h>

#include "DBSchemaToExcel.h"
#include "DBSchemaToXML.h"
#include "DBSchemaToSync.h"

#endif //PCH_H
