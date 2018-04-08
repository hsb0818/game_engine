#include "../pch/pch.h"
#include "../GInclude.h"


SystemScene::SystemScene()
	: m_pFont(nullptr)
	, m_pMemGraph(nullptr)
{
	Init();
}

SystemScene::~SystemScene()
{
	Destroy();
}

void SystemScene::Init()
{
	if (m_pFont == nullptr)
	{
		m_pFont = new SOCFont();
		m_pFont->Init(R_SOC_APP().DC());
	}

	if (m_pMemGraph == nullptr)
	{
		m_pMemGraph = new ProGraph2D();
		
		m_pMemGraph->Insert(0.f);
		for (int i=1; i<18; i++)
			m_pMemGraph->Insert(rand()%20 * i);
		m_pMemGraph->Insert(0.f);
	}
}

void SystemScene::Destroy()
{
	SAFE_DELETE(m_pMemGraph);
	SAFE_DELETE(m_pFont);
}

void SystemScene::FrameMove(double elapsedTime)
{
	m_pMemGraph->Update();
}

void SystemScene::FrameRender(double elapsedTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	R_SOC_GL().ReSizeScene(false, R_SOC_APP().GetScreenWidth(), R_SOC_APP().GetScreenHeight());

	glPushMatrix();
    glTranslatef(0.0f, 55.0f, 0.0f);
	m_pFont->Render("Profiling", 0.f, 0.f, SOC_Color3f(1.f, 1.f, 1.f, 1.f));
	glPopMatrix();

	glPushMatrix();
//	glTranslatef(20.0f, 0.f, 0.0f);
	m_pMemGraph->Draw(2.f, 2.f);
	glPopMatrix();
}

void SystemScene::Event(MSG* pMsg)
{
}