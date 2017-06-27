#ifndef APP_COMMON_H_
#define APP_COMMON_H_


namespace BestStealReplica {

/**
 * Point構造体のジェネリック型
 *
 * 当構造体はコピー可能とする。
 */
template<typename T> struct Point {
	T x;
	T y;

	Point() = default;
	Point(T x, T y);
};

/**
 * 四角形の頂点情報を保有する構造体
 *
 * 当構造体はコピー可能とする。
 */
template<typename T> struct Rectangle {
	T topLeft;
	T bottomRight;
};

template<typename T>
using DataTable = std::vector<std::vector<T>>;


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

		Key() = default;
		Key(const Key&) = delete;

		Key& operator=(const Key&) = delete;
	};


	/* Enums -------------------------------------------------------------------------------------------- */
	enum struct SceneType {
		DRAWING_MAP,
		BLACKOUT
	};

	enum struct Direction {
		TOP,
		RIGHT,
		BOTTOM,
		LEFT
	};

	enum struct GateKeyType {
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


	/* Operator Overloads ------------------------------------------------------------------------------- */
	AppCommon& operator=(const AppCommon&) = delete;


private:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	AppCommon() = delete;
	AppCommon(const AppCommon&) = delete;

};

}

#endif
