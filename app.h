#ifndef _APP_H_
#define _APP_H_

class SOC_APP : public SOC_System::Singleton<SOC_APP>
{
public:
	SOC_APP();
	~SOC_APP();

public:
	void Init(HINSTANCE hInst);

	bool	IsFullScreenState() { return m_bFullScreen; }
	int		GetScreenHeight() { return m_screenHeight; }
	int		GetScreenWidth() { return m_screenWidth; }
	HWND	GethWnd() { return m_hWnd; }
	MSG*	GetMsg() { return &m_msg; }
	HDC		DC() { return m_hDC; }
	void	PrintFrame();
	void	Start();
	void	End();

	// Delete Rendering-Context(RC), and
	// return DC and windows handle.
	void ReturnResources();
	LRESULT ReturnProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	void InitWindow(bool bFullScreen, WCHAR* pTitle = SOC_BOT, WCHAR* pCaption = SOC_BOT, 
		int width = WIN_DEFAULT_WIDTH, int height = WIN_DEFAULT_HEIGHT);
	bool FullMode(WCHAR* pTitle, int width, int height);
	bool EnvTest();

private:
	HWND		m_hWnd;
	HINSTANCE	m_hInst;
	MSG			m_msg;
	WNDCLASSEX	m_wc;
	HDC			m_hDC;
	HGLRC		m_hRC;
	DWORD		m_dwStyle;
	DWORD		m_dwExStyle;

	MPTimer*	m_pTimer;
	int			m_screenWidth;
	int			m_screenHeight;
	__int64		m_frameCount;
	bool		m_bFullScreen;
	bool		m_bActivated; // state : minimize / maximize
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif