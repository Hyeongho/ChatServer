#pragma once

#include "../../Common.h"

#include "resource.h"

#define SERVERIP _T("127.0.0.1")
#define SERVERPORT 9000

#define SIZE_TOT 256
#define SIZE_DAT (SIZE_TOT - sizeof(int))

#define WM_DRAWLINE (WM_USER + 1)
#define WM_ERASEPIC (WM_USER + 2)