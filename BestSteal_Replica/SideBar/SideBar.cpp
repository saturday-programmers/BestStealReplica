#include "SideBar.h"
#include "AppCommon.h"
#include "Map\MapChip.h"


namespace BestStealReplica {
namespace SideBar {

/* Public Functions  -------------------------------------------------------------------------------- */
void SideBar::ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const {}

void SideBar::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const {
	
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::NONE;
		context.rect.bottomRight.x = AppCommon::GetWindowWidth();
		context.rect.bottomRight.y = AppCommon::GetWindowHeight();
		context.rect.topLeft.y = 0;
		context.rect.topLeft.x = AppCommon::GetWindowWidth() - (MapChip::WIDTH * CHIP_COUNT_PER_ROW);
		context.r = 0;
		context.g = 0;
		context.b = 0;

		pRet->push_back(context);
}

}
}