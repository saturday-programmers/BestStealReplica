#ifndef SIDE_BAR_H_
#define SIDE_BAR_H_

#include "AppCommon.h"
#include "Drawing/IDrawable.h" 


namespace BestStealReplica {
namespace SideBar {

class SideBar : public Drawing::IDrawable {
public:
	/* Functions ---------------------------------------------------------------------------------------- */
	void ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const;
	void CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const; 

	int GetWidth() const;

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	int CHIP_COUNT_PER_ROW = 3;
};

}
}

#endif