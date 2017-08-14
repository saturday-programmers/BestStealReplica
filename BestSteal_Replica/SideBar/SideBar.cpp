#include "SideBar.h"


namespace BestStealReplica {
namespace SideBar {

/* Public Functions  -------------------------------------------------------------------------------- */
void SideBar::ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const {}

void SideBar::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const {
	
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::NONE;

		pRet->push_back(context);
}

}
}