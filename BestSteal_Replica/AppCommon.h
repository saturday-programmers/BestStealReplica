#ifndef APP_COMMON_H_
#define APP_COMMON_H_

#include <windows.h>

// new���Z�q���I�[�o�[���[�h���Ă���N���X��crtdbg.h���O��include����K�v�L
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
	/* Structs ------------------------------------------------------------------------------------------ */
	struct Key {
		bool up = false;
		bool down = false;
		bool right = false;
		bool left = false;
		bool z = false;
		bool x = false;
		bool shift = false;
	};

	/* Enums -------------------------------------------------------------------------------------------- */
	enum SceneType {
		DRAWING_MAP,
		BLACKOUT
	};

	enum Direction {
		TOP,
		RIGHT,
		BOTTOM,
		LEFT
	};

	enum KeyType {
		None,
		Silver,
		Gold
	};

	/* Constants ---------------------------------------------------------------------------------------- */
	static const int WINDOW_DEFAULT_HEIGHT = 630;
	static const int WINDOW_DEFAULT_WIDTH = 1120;
	static const int FRAME_COUNT_PER_CUT = 10;

	/* Getters / Setters -------------------------------------------------------------------------------- */
	static int GetWindowHeight();
	static void SetWindowHeight(int heidht);
	static int GetWindowWidth();
	static void SetWindowWidth(int width);
	static AppCommon::SceneType GetScene();
	static void SetScene(AppCommon::SceneType argScene);
};

}

#endif
