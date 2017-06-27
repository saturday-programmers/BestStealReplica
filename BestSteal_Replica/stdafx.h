#ifndef STDAFX_H_
#define STDAFX_H_

#pragma warning( push )
#pragma warning( disable : 4365 4514 4571 4987 )
#include <vector>
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4365 4514 4571 4625 4626 5027)
#include <map>
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4365 4514 )
#include <algorithm>
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4668 )
#include <windows.h>
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4514 5029 )
// new演算子をオーバーロードしているクラスはcrtdbg.hより前にincludeする必要有
#include <d3dx9math.h>
#pragma warning( pop )

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif
