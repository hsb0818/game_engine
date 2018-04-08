#include "../pch/pch.h"

using namespace std;
using std::ios;

#define LOGTYPE_LIST() \
	DEF_LOGTYPE(LOG_NORMAL) \
	DEF_LOGTYPE(LOG_ERROR) \
	DEF_LOGTYPE(LOG_DEBUG) \
	DEF_LOGTYPE(LOG_WARNING) \
	
#define DEF_LOGTYPE(LOGTYPE) LOGTYPE,

enum
{
	LOGTYPE_LIST()
	LOG_MAX
};

#undef DEF_LOGTYPE

#define DEF_LOGTYPE(LOGTYPE) L#LOGTYPE,

const wchar_t* g_LogTypeName[] =
{
	LOGTYPE_LIST()
};

#undef DEF_LOGTYPE

DWORD LOG_COLOR[LOG_MAX] = { 0x00000000, 0x000000ff, 0x00000000, 0x00000000 };

CLog::CLog()
{
	ZeroMemory(LogLine, sizeof(LogLine));
	ZeroMemory(LineSize, sizeof(LineSize));
	ZeroMemory(LogColor, sizeof(LogColor));

	LineIndex = 0;

	// 서버 시작 시각을 이름에 포함한 로그 파일 생성.
	time_t time1;
	struct tm time2;
	int nowYear = 0;
	int nowMonth = 0;
	int nowDay = 0;
	int nowHour = 0;
	int nowMin = 0;
	int nowSec = 0;

	time(&time1);
	localtime_s(&time2, &time1);

	char strTime[128] = "";
	nowYear = time2.tm_year + 1900;
	nowMonth = time2.tm_mon + 1;
	nowDay = time2.tm_mday;
	nowHour = time2.tm_hour;
	nowMin = time2.tm_min;
	nowSec = time2.tm_sec;

	sprintf_s(strTime, "Debug Log %d %d %d %d %d %d.txt", nowYear, nowMonth, nowDay, nowHour, nowMin, nowSec);
	m_os.open(strTime, ios::out);
};

CLog::~CLog() 
{
	m_os.close();
};

void CLog::SetWnd(HWND hwnd)
{
	this->hWnd = hwnd;
}

void CLog::Add(int log_type, const WCHAR* format, ...)
{
	if (log_type < LOG_NORMAL || log_type >= LOG_MAX)
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

	LogColor[idx] = LOG_COLOR[log_type];

//	if (log_type == LOG_ERROR || log_type == LOG_NORMAL)
	{
		InvalidateRect(hWnd, NULL, TRUE);
	}

	WCHAR logBuffer[2048];
	struct tm fnow;
	time_t now;
	time(&now);
	localtime_s( &fnow, &now );

	swprintf_s(logBuffer, L"%s : %04d.%02d.%02d.%02d.%02d.%02d : ", g_LogTypeName[log_type], fnow.tm_year+1900, fnow.tm_mon+1, fnow.tm_mday, fnow.tm_hour, fnow.tm_min, fnow.tm_sec);
	wcsncat_s(logBuffer, LogLine[idx], LineSize[idx]);

	{
		TYPED_SCOPE_LOCK(m_lock);
		m_os << logBuffer <<endl;
	}
}

void CLog::ReDraw(HDC hDC)
{
	RECT clientRect;
	SIZE fontSize;
	HFONT font;

	GetClientRect(hWnd, &clientRect);
	font = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
	SelectObject(hDC, font);
	GetTextExtentPoint32(hDC, L"H", 1, &fontSize);

	int height = clientRect.bottom;
	for (long i=LineIndex+MAX_LOG_LINE; i>LineIndex;i--)
	{
		if (!LogLine[i%MAX_LOG_LINE] || height<0)
		{
			break;
		}

		SetTextColor(hDC, LogColor[i%MAX_LOG_LINE]);

		height = height - fontSize.cy;
		TextOut(hDC, 0, height, LogLine[i%MAX_LOG_LINE], LineSize[i%MAX_LOG_LINE]);
	}
}
