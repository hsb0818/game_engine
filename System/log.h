#pragma once

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <ostream>

#define MAX_LOG_LINE	128
#define MAX_LOG_LEN		2048

namespace SOC_System {
	
	class CLog
	{
	public:
		CLog();
		virtual ~CLog();

	public:
		void SetWnd(HWND hwnd);
		virtual HWND GetWnd() { return hWnd; }
		virtual void Add(int log_type, const WCHAR* format, ...); // 로그를 기록합니다. 
		void ReDraw(HDC hDC);

		void AddConcurrentUser();

	protected:
		volatile long LineIndex;					// 현재 쓰이는 라인의 인덱스.
		std::wofstream m_os;	

		// 화면 출력용 버퍼. 
		wchar_t LogLine[MAX_LOG_LINE][MAX_LOG_LEN];	// MAX_LOG_LINE 라인까지 저장하자.
		int LineSize[MAX_LOG_LINE];		// 각 라인의 길이.
		DWORD LogColor[MAX_LOG_LINE];	// 각 라인의 글자색.

		HWND hWnd;
		Lock::CriticalSectionLock m_lock;
	};
}