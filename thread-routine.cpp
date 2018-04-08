#include "pch\pch.h"
#include "GInclude.h"

// HACCEL hAccelTable;

void Graphic_ThreadLoop(SOCThread* pCurThread)
{
	if (pCurThread == nullptr)
		return;

	printf("render t start\n");
	double fRenderTime = 0.0;
	double fElapsedTime = 0.0;
	MPTimer* pTimer = new MPTimer();

	pTimer->Start();
	while (R_SOC_APP().GetMsg()->message != WM_QUIT)
	{
		if (pCurThread->IsExited())
			break;
		if (pCurThread->GetHandle() == INVALID_HANDLE_VALUE)
			break;
		
		if (PeekMessage(R_SOC_APP().GetMsg(), NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(R_SOC_APP().GetMsg());
			DispatchMessage(R_SOC_APP().GetMsg());
		}
		else
		{
			pTimer->End();
			{
				fElapsedTime = pTimer->CalcRunTime();
				fRenderTime += fElapsedTime;
			}
			pTimer->StartisEnd();

			if (fRenderTime >= FRAME_LIMIT_RENDER)
			{
				R_SOC_GL().Render(fRenderTime);
				R_SCENE_MNG().FrameRender();
				fRenderTime = 0.0;

				SwapBuffers(R_SOC_APP().DC());
			}
		}
	}

	SAFE_DELETE(pTimer);
	if (pCurThread)
		pCurThread->Exit();
}

void Physics_ThreadLoop(SOCThread* pCurThread)
{
	if (pCurThread == nullptr)
		return;

	double fPhysicsTime = 0.0;
	double fElapsedTime = 0.0;
	MPTimer* pTimer = new MPTimer();
	
	pTimer->Start();
	while(pCurThread->IsExited() == false)
	{
		if (pCurThread->GetHandle() == INVALID_HANDLE_VALUE)
			break;

		pTimer->End();
		{
			fElapsedTime = pTimer->CalcRunTime();
			fPhysicsTime += fElapsedTime;
		}
		pTimer->StartisEnd();

		if (fPhysicsTime >= FRAME_LIMIT_PHYSICS)
		{
			// call physics func
			fPhysicsTime = 0.0;
		}

		printf("-----t : physics\n");
		Sleep(500);
	}

	SAFE_DELETE(pTimer);
	if (pCurThread)
		pCurThread->Exit();
}

void System_ThreadLoop(SOCThread* pCurThread)
{
	if (pCurThread == nullptr)
		return;

	double fElapsedTime = 0.0;
	MPTimer* pTimer = new MPTimer();

	pTimer->Start();
	while(pCurThread->IsExited() == false)
	{
		if (pCurThread->GetHandle() == INVALID_HANDLE_VALUE)
			break;

		printf("-----t : system\n");
		Sleep(500);
	}

	SAFE_DELETE(pTimer);
	if (pCurThread)
		pCurThread->Exit();
}

void Profiling_ThreadLoop(SOCThread* pCurThread)
{
	if (pCurThread == nullptr)
		return;

	double fElapsedTime = 0.0;
	MPTimer* pTimer = new MPTimer();

	pTimer->Start();
	while(pCurThread->IsExited() == false)
	{
		if (pCurThread->GetHandle() == INVALID_HANDLE_VALUE)
			break;

		printf("-----t : profile\n");
		Sleep(500);
	}

	SAFE_DELETE(pTimer);
	if (pCurThread)
		pCurThread->Exit();
}