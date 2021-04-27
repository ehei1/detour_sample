#pragma once

// if you uncomment CINTERFACE macro, you can access virtual table in COM directly.
// e.g) g_pd3dDevice->lpVtbl
//#define CINTERFACE

#include <cassert>

#include <ole2.h>
#include <windows.h>
#include <d3d9.h>

#include "..\Detours\include\detours.h"