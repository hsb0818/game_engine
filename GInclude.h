#ifndef _MY_INCLUDE_H_
#define _MY_INCLUDE_H_

// show debug console
#ifdef _DEBUG
	#pragma comment( linker, "/entry:WinMainCRTStartup /subsystem:console" )
#endif

#include "resource.h"

#include "DataStruct\interface.h"

// associated instance
#include "GameSystem\interface.h"
//
#include "Scene\SystemScene.h"
//
#include "app.h"

// instance list
#include "instance_returner.h" 
#include "Scene\interface.h"
#include "thread-routine.h"

#endif