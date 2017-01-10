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
};

}
}

#endif
