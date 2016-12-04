#ifndef IDRAWABLE_H_
#define IDRAWABLE_H_

#include <vector>

#include "../AppCommon.h"
#include "../Drawing/DrawingCommon.h"


namespace BestStealReplica {
namespace Drawing {

class IDrawable {
public:
	/* Getters / Setters -------------------------------------------------------------------------------- */
	virtual std::vector<TextureType> GetTextureTypes() const = 0;

	/* Functions ---------------------------------------------------------------------------------------- */
	virtual void CreateDrawingContexts(std::vector<DrawingContext>* pDrawingContexts) const = 0;
};

}
}

#endif