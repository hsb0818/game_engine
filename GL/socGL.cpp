#include "../pch/pch.h"
#include "../GInclude.h"

namespace SOC_GL {

GLApp::GLApp() 
	: m_aspect(1.f)
	, m_bPerspective(true)
{
	Init();
}

GLApp::~GLApp()
{
}

void GLApp::Init()
{
	m_aspect = 1.f;
	m_bPerspective = true;

	memset(&m_pfd, 0, sizeof(m_pfd));
	m_pfd.nSize = sizeof(m_pfd);
	m_pfd.nVersion = 1;
	m_pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	m_pfd.iPixelType = PFD_TYPE_RGBA;	// Request An RGBA Format
	m_pfd.cColorBits = 24;	// Select Our Color Depth
	m_pfd.cDepthBits = 16;	// 16Bit Z-Buffer (Depth Buffer)
	m_pfd.iLayerType = PFD_MAIN_PLANE;	// Main Drawing Layer
}

bool GLApp::Render_Init()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.f, 0.f, 0.5f, 0.f);
	glClearDepth(1.f);
	glEnable(GL_DEPTH_TEST);	// testing.
	glDepthFunc(GL_LEQUAL);		// setting depth-test test solution
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return true;
}

void GLApp::ReSizeScene(int width, int height)
{
	if (height == 0)
		height = 1;

	m_aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	UpdateProjection();
}

void GLApp::ReSizeScene(bool bPerspective, int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
	UpdateProjection(bPerspective, (float)width / (float)height);
}

void GLApp::UpdateProjection(int width, int height, double n, double f)
{
	UpdateProjection(m_bPerspective, m_aspect, width, height, n, f);
}

void GLApp::UpdateProjection(bool bPerspective, float aspect, int width, int height, double n, double f)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if (bPerspective)
		gluPerspective(45.f, aspect, 0.1f, 100.f);
	else
	{
		glOrtho(0, width, height, 0, n, f);
	}

	glMatrixMode(GL_MODELVIEW);
}

void GLApp::Render(double fElapsedTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();

	UpdateProjection(m_bPerspective);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(-0.5, 1.0, 7.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

}