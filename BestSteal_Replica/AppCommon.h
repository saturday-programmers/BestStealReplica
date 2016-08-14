#ifndef APP_COMMON_H_
#define APP_COMMON_H_

#include <windows.h>

// new演算子をオーバーロードしているクラスはcrtdbg.hより前にincludeする必要有
#include <d3dx9math.h>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


namespace BestStealReplica {

struct FloatPoint {
	float x;
	float y;

	FloatPoint();
	FloatPoint(float x, float y);
};

struct DrawingVertex {
	int x;
	int y;
	float tu;
	float tv;
};

template<typename T> struct Vertices {
	T topLeft;
	T bottomRight;
};


class AppCommon {
public:
	enum Direction {
		TOP,
		RIGHT,
		BOTTOM,
		LEFT
	};

	static const int WINDOW_DEFAULT_HEIGHT = 630;
	static const int WINDOW_DEFAULT_WIDTH = 1120;
	static const int FRAME_COUNT_PER_CUT = 10;

	static void SetWindowHeight(int heidht);
	static void SetWindowWidth(int width);
	static int GetWindowHeight();
	static int GetWindowWidth();
};

}

#endif
