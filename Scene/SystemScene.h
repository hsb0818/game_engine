#pragma once

using namespace SOC_System;
using namespace SOC_GL;

#define SYSTEMSCENE_NAME (L"SystemScene")

using namespace SOC_Utill;

class SystemScene : public SceneBase
{
public:
	SystemScene();
	~SystemScene();

public:
	virtual void Init();
	virtual void Destroy();
	virtual void FrameMove(double elapsedTime);
	virtual void FrameRender(double elapsedTime);
	virtual void Event(MSG* pMsg);

private:
	SOCFont* m_pFont;
	ProGraph2D* m_pMemGraph;
};