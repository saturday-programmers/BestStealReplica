#ifndef DRAWING_COMMON_H_
#define DRAWING_COMMON_H_

#include "../AppCommon.h"


namespace BestStealReplica {
namespace Drawing {

/* Structs ------------------------------------------------------------------------------------------ */
struct DrawingVertex {
	int x;
	int y;
	float tu;
	float tv;

	DrawingVertex() = default;
	DrawingVertex(const DrawingVertex&) = default;

	DrawingVertex& operator=(const DrawingVertex&) = delete;
};

/* Enums -------------------------------------------------------------------------------------------- */
enum TextureType {
	MAP,
	CHARACTER
};

/* Structs ------------------------------------------------------------------------------------------ */
struct DrawingContext {
	Rectangle<DrawingVertex> rect;
	TextureType textureType;
	UINT16 alpha = 0xFF;

	DrawingContext() = default;
	DrawingContext(const DrawingContext&) = default;

	DrawingContext& operator=(const DrawingContext&) = delete;
};

}
}

#endif
