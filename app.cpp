#include "pch/pch.h"
#include "GInclude.h"

SOC_APP::SOC_APP() 
	: m_screenWidth(WIN_DEFAULT_WIDTH)
	, m_screenHeight(WIN_DEFAULT_HEIGHT)
	, m_hWnd(NULL), m_hInst(NULL), m_hDC(NULL), m_hRC(NULL)
	, m_pTimer(nullptr)
	, m_frameCount(0)
	, m_bActivated(true)
{
	ZeroMemory(&m_msg, sizeof(m_msg));
}

SOC_APP::~SOC_APP()
{
}

bool SOC_APP::EnvTest()
{
	if (m_pTimer == nullptr)
		return false;

	return true;
}

void SOC_APP::ReturnResources()
{
	if (m_bFullScreen)
	{
		ChangeDisplaySettings(nullptr, 0);
		ShowCursor(true);
	}

	if (m_hRC)
	{
		if (wglMakeCurrent(NULL, NULL) == false)
			MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		if (wglDeleteContext(m_hRC) == false)
			MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		m_hRC = NULL;
	}

	if (m_hDC && (ReleaseDC(m_hWnd, m_hDC) == false))
	{
		MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hDC = NULL;
	}

	if (m_hWnd && (DestroyWindow(m_hWnd) == false))
	{
		MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		m_hWnd = NULL;
	}

	if (UnregisterClass(m_wc.lpszClassName, m_hInst) == false)
	{
		printf("Error Code : %d\n", GetLastError());
		MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR"
			, MB_OK || MB_ICONINFORMATION); 
		
		m_hInst = NULL;
	} 
}

void SOC_APP::Start()
{
	if (EnvTest() == false)
		return;

	m_pTimer->Start();
}

void SOC_APP::End()
{
	if (EnvTest() == false)
		return;

	delete m_pTimer;
}

void SOC_APP::Init(HINSTANCE hInst)
{
	m_hInst = hInst;
	timeBeginPeriod(1);
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	InitWindow(false, L"Project HSB");
}

void SOC_APP::InitWindow(bool bFullScreen, WCHAR* pTitle, WCHAR* pCaption, int width, int height)
{
	RECT screenRect;
	RECT wndRect = {0, 0, width, height};

	int iWndWidth = (wndRect.right - wndRect.left);
	int iWndHeight = (wndRect.bottom - wndRect.top);

	m_screenWidth = iWndWidth;
	m_screenHeight = iWndHeight;
	m_bFullScreen = bFullScreen;

	memset(&m_wc, 0, sizeof(m_wc));

	m_hInst = GetModuleHandle(NULL);

	m_wc.cbSize			= sizeof(m_wc);
	m_wc.cbClsExtra		= 0;
	m_wc.cbWndExtra		= 0;
	m_wc.hbrBackground	= NULL; // (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	m_wc.hIcon			= LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	m_wc.hIconSm		= LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	m_wc.hInstance		= m_hInst;
	m_wc.lpszClassName	= pTitle;
	m_wc.lpfnWndProc	= WndProc;
	m_wc.lpszMenuName	= NULL;
	m_wc.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	if (RegisterClassEx(&m_wc) == false)
	{
		printf("[Error Code : %d] Failed To Register The Window Class\n", GetLastError());
		MessageBox(NULL, L"Failed To Register The Window Class.", L"ERROR"
			, MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	m_dwStyle	= WS_OVERLAPPEDWINDOW;
	m_dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	bool bErrCheck = 1;
	if (bFullScreen)
		bErrCheck = FullMode(pTitle, width, height);

	if (bErrCheck == false)
	{
		printf("[Error Code : %d] : failed set to Full-Screen mode \n", GetLastError());
	}

	AdjustWindowRectEx(&wndRect, m_dwStyle, false, m_dwExStyle);

	GetWindowRect(GetDesktopWindow(), &screenRect);
	int iX = ((screenRect.right - screenRect.left) - iWndWidth) >> 1;
	int iY = ((screenRect.bottom - screenRect.top) - iWndHeight) >> 1;
	
	m_hWnd = CreateWindowEx(m_dwExStyle, pTitle, pCaption, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | m_dwStyle, 
		iX, iY, iWndWidth, iWndHeight, NULL, 0, m_hInst, 0);

	if (m_hWnd == NULL)
	{
		printf("[Error Code : %d] : failed to run CreateWindowEx \n", GetLastError());
		MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		ReturnResources();
		return;
	}

	m_hDC = GetDC(m_hWnd);
	if (m_hDC == NULL)
	{
		printf("[Error Code : %d] : failed to get DC \n", GetLastError());
		MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		ReturnResources();
		return;
	}

	int pixelFormat = ChoosePixelFormat(m_hDC, &R_SOC_GL().m_pfd);
	if (pixelFormat == 0)
	{
		ReturnResources();
		return;
	}

	if (SetPixelFormat(m_hDC, pixelFormat, &R_SOC_GL().m_pfd) == false)
	{
		ReturnResources();
		return;
	}

	m_hRC = wglCreateContext(m_hDC);
	if (m_hRC == NULL)
	{
		ReturnResources();
		return;
	}

	if (wglMakeCurrent(m_hDC, m_hRC) == false)
	{
		ReturnResources();
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR"
			, MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	R_SOC_GL().ReSizeScene(m_screenWidth, m_screenHeight);

	if (R_SOC_GL().Render_Init() == false)
	{
		ReturnResources();
		MessageBox(NULL, L"GL Initialization Failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
}

bool SOC_APP::FullMode(WCHAR* pTitle, int width, int height)
{
	DEVMODE dmScreenSettings;
	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = width;
	dmScreenSettings.dmPelsHeight = height;
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)
		!= DISP_CHANGE_SUCCESSFUL)	
	{
		printf("[Error Code : %d] : failed to Display Setting \n", GetLastError());
		return false;
	}

	m_dwExStyle = WS_EX_APPWINDOW;
	m_dwStyle	= WS_POPUP;

	ShowCursor(false);

	return true;
}

LRESULT SOC_APP::ReturnProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))
				m_bActivated = true;
			else
				m_bActivated = false;
			return 0;
		}
	case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				// 0을 반환함으로써 스크린 세이버와 모니터 절전 모드의 진입을 막는다.
				return 0;
			}

			break;
		}
	case WM_SIZE:
		{
			R_SOC_GL().ReSizeScene(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
	case WM_KEYDOWN:
		{
#if defined(DEBUG) || defined(_DEBUG)
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage( 0 );
			}
#endif
			break;
		}
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc( hWnd, msg, wParam ,lParam );
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return SOC_APP::GetInstance().ReturnProc( hWnd, msg, wParam, lParam );
}

// this is call in physics loop
void SOC_APP::PrintFrame()
{
	static int countFrame = 0;
	static float time = (float)timeGetTime();

	++countFrame;

	if((timeGetTime() - time) >= 1000.f)
	{
		printf("UpdateFrame : %d\nRenderFrame : %d\n", countFrame, m_frameCount);
		countFrame = 0;
		m_frameCount = 0;
		time = (float)timeGetTime();
	}
}