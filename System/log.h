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
		virtual void Add(int log_type, const WCHAR* format, ...); // �α׸� ����մϴ�. 
		void ReDraw(HDC hDC);

		void AddConcurrentUser();

	protected:
		volatile long LineIndex;					// ���� ���̴� ������ �ε���.
		std::wofstream m_os;	

		// ȭ�� ��¿� ����. 
		wchar_t LogLine[MAX_LOG_LINE][MAX_LOG_LEN];	// MAX_LOG_LINE ���α��� ��������.
		int LineSize[MAX_LOG_LINE];		// �� ������ ����.
		DWORD LogColor[MAX_LOG_LINE];	// �� ������ ���ڻ�.

		HWND hWnd;
		Lock::CriticalSectionLock m_lock;
	};
}