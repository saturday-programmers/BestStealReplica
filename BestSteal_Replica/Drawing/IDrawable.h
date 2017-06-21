#ifndef IDRAWABLE_H_
#define IDRAWABLE_H_

#include <vector>

#include "../AppCommon.h"
#include "../Drawing/DrawingCommon.h"


namespace BestStealReplica {
namespace Drawing {

class IDrawable {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	IDrawable() = default;
	IDrawable(const IDrawable&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	virtual void ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const = 0;
	virtual void CreateDrawingContexts(std::vector<DrawingContext>* pRet) const = 0;

};

}
}

#endif