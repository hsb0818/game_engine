#ifndef _PCH_H_
#define _PCH_H_

#pragma warning(disable: 4819)

#if defined(_WIN32) || defined(_WIN64)
	#pragma comment(lib, "Ws2_32.lib")
	#pragma comment(lib, "mswsock.lib") 
	#pragma comment(lib, "winmm.lib")
	#pragma comment (lib, "opengl32.lib")  /* link with Microsoft OpenGL lib */
	#pragma comment (lib, "glu32.lib")     /* link with OpenGL Utility lib */
	#pragma comment (lib, "glut32.lib")    /* link with Win32 GLUT lib */
	#pragma message("Automatically linking in pch.h file...")    

	#include <winsock2.h>
	#include <gl/gl.h>
	#include <gl/glu.h>
	#include <gl/glaux.h>

	#include <MSWSock.h>
	#include <ws2tcpip.h>
	#include <iphlpapi.h>
	#include <process.h>
	#include <commctrl.h>
	#include <windows.h>
    #include <conio.h>
    #include <tchar.h>		
	#include <mmsystem.h>
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <libkern/OSAtomic.h>
#endif

//////////////
// this is.... I will use only [std::min, std::max] function.
#undef min
#undef max
//////////////

#include <iostream>
#include <fstream>
#include <limits>
#include <stdio.h>
#include <time.h>
#include <assert.h>

// stl lib
#include <map>
#include <list>
#include <string>
#include <vector>
#include <string>
#include <stack>

// my sources
#include "pre-define.h"
#include "../System/interface.h"
#include "../Memory/interface.h"
#include "../GL/interface.h"
#include "../Utility/interface.h"
#include "../HighLevel_System/interface.h"

#endif