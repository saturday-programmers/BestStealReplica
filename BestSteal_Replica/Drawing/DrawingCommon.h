#ifndef DRAWING_COMMON_H_
#define DRAWING_COMMON_H_

#include "AppCommon.h"


namespace BestStealReplica {
namespace Drawing {

/* Structs ------------------------------------------------------------------------------------------ */
/**
 * 描画情報を持つ構造体
 *
 * 当構造体はコピー可能とする。
 */
struct DrawingVertex {
	int x;
	int y;
	float tu;
	float tv;
};

/* Enums -------------------------------------------------------------------------------------------- */
enum struct TextureType {
	NONE,
	MAP,
	CHARACTER
};

/* Structs ------------------------------------------------------------------------------------------ */
struct DrawingContext {
	Rectangle<DrawingVertex> rect;
	TextureType textureType;
	UINT8 r = 0xFF;
	UINT8 g = 0xFF;
	UINT8 b = 0xFF;
	UINT8 alpha = 0xFF;

	DrawingContext() = default;
	DrawingContext(const DrawingContext&) = default;

	DrawingContext& operator=(const DrawingContext&) = delete;
};

}
}

#endif
