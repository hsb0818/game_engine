//
//  PreDefine.h
//  Created by SungBin_Hong on 13. 10. 13..
//  Copyright (c) 2013 years Teraphonia. All rights reserved.
//

#pragma once

#define SOC_BOT L"SOC_BOT"

// Profiling option macro.
#define __PROFILE_MODE__

#undef SAFE_DELETE
#define SAFE_DELETE(x) { if ( (x) != nullptr ) { delete (x); (x) = nullptr; } }

#define SAFE_FREE(x) { if ( (x) != nullptr ) { free(x); (x) = nullptr; } }

#if defined (WIN32)
	#  define __func__ __FUNCTION__
#endif

/*
#ifndef _T
	#ifndef __T
		#define __T(x) L ## x
	#endif

	#define _T(x) __T(x)
#endif
*/

const int WIN_DEFAULT_WIDTH = 1024;
const int WIN_DEFAULT_HEIGHT = 768;
const double FRAME_LIMIT_RENDER = 0.0167; // 1/60
const double FRAME_LIMIT_PHYSICS = 0.00001;