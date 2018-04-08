#include "pch/pch.h"
#include "GInclude.h"

#define __INITIALIZE(x) ( x() )

void Init_Engine()
{
	R_MEMORYPOOL(); //have to call very first. Singleton<> Class의 atexit() 의 특성 때문이다.
	R_CONFIG().Init();
	R_SOC_APP();
	R_SOC_GL().Init();

	R_THREAD_POOL().CreateThreadPool(4);
	R_MEMORYPOOL().Init(R_THREAD_POOL().GetThreadMax());
	R_THREAD_POOL().Insert(Physics_ThreadLoop);
	R_THREAD_POOL().Insert(System_ThreadLoop);
	R_THREAD_POOL().Insert(Profiling_ThreadLoop);

	SCENE_CREATE(SystemScene, SYSTEMSCENE_NAME);
	SCENE_START(SYSTEMSCENE_NAME);

	printf("%d allocated to use at Initialize\n", R_MEMORYPOOL().GetAllocatedMemoryAll());
}

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
	__INITIALIZE(Init_Engine);
	R_SOC_APP().Init(hInstance);

	R_SOC_APP().Start();
	R_THREAD_POOL().Excute(Graphic_ThreadLoop); // mainloop is Graphic
	R_SOC_APP().End();

	return 0;
}