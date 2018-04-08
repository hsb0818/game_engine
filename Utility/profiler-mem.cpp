#include "../pch/pch.h"

namespace SOC_Utill {
	MemoryProfiler* MemoryProfiler::m_pInst = nullptr;
}

using namespace std;
using namespace SOC_Utill;
using std::ios;

// Log is Tag. equal.
// MEMLOG_EXPEND 소모성 메모리 (할당/해제가 잦은가)
// MEMLOG_LTIME	 좀 오래 쓸 메모리
// MEMLOG_LLTIME 진짜 엄청 오래 쓸 메모리
// MEMLOG_FOREVER 프로그램 종료 시까지 해제하지 않는 메모리

#define LOGTYPE_LIST()	\
	DEF_LOGTYPE(MEMLOG_NORMAL)	\
	DEF_LOGTYPE(MEMLOG_ERROR)	\
	DEF_LOGTYPE(MEMLOG_DEBUG)	\
	DEF_LOGTYPE(MEMLOG_EXPEND)	\
	DEF_LOGTYPE(MEMLOG_LTIME)	\
	DEF_LOGTYPE(MEMLOG_LLTIME)	\
	DEF_LOGTYPE(MEMLOG_FOREVER)

#define DEF_LOGTYPE(LOGTYPE) LOGTYPE,

enum
{
	LOGTYPE_LIST()
	LOG_MAX
};

#undef DEF_LOGTYPE
#define DEF_LOGTYPE(LOGTYPE) L#LOGTYPE, // L is for unicode.

const wchar_t* g_MemLogTypeName[] =
{
	LOGTYPE_LIST()
};

#undef DEF_LOGTYPE

DWORD MEMLOG_COLOR[LOG_MAX] = { 0x00000000, 0x000000ff, 0x00000000, 0x00000000 };

void MemoryProfiler::Add(int log_type, const WCHAR* format, ...)
{
	if (log_type < MEMLOG_NORMAL || log_type >= LOG_MAX)
	{
		return;
	}
	
	long idx = 0;
	// SYNC
	{
		TYPED_SCOPE_LOCK(m_lock);
		idx = LineIndex = ((LineIndex + 1) % MAX_LOG_LINE);
	}

	va_list argptr;
	va_start(argptr, format);

	LineSize[idx] = _vsnwprintf_s(LogLine[idx], _TRUNCATE, format, argptr);

	va_end(argptr);

	if (LineSize[idx] == -1)
	{
		LineSize[idx] = MAX_LOG_LEN;
	}

	LogColor[idx] = MEMLOG_COLOR[log_type];

//	if (log_type == LOG_ERROR || log_type == LOG_NORMAL)
	{
		InvalidateRect(hWnd, NULL, TRUE);
	}

	WCHAR logBuffer[2048];
	struct tm fnow;
	time_t now;
	time(&now);
	localtime_s( &fnow, &now );

	swprintf_s(logBuffer, L"%s : %04d.%02d.%02d.%02d.%02d.%02d : ", g_MemLogTypeName[log_type], fnow.tm_year+1900, fnow.tm_mon+1, fnow.tm_mday, fnow.tm_hour, fnow.tm_min, fnow.tm_sec);
	wcsncat_s(logBuffer, LogLine[idx], LineSize[idx]);

	{
		TYPED_SCOPE_LOCK(m_lock);
		m_os << logBuffer <<endl;
	}
}