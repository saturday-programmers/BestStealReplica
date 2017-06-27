#include "MapChip.h"
#include "Map/MapChipWall.h"
#include "Map/MapChipGate.h"
#include "Map/MapChipJewelry.h"


namespace BestStealReplica {
namespace Map {

/* Static Public Functions -------------------------------------------------------------------------- */
MapChip* MapChip::Create(MapChipType chipType) {
	switch (chipType) {
		case MapChipType::WALL:
			return new MapChipWall();
			break;
		case MapChipType::GATE:
		case MapChipType::GOLD_GATE:
			return new MapChipGate(chipType);
			break;
		case MapChipType::JEWELRY:
			return new MapChipJewelry();
		default:
			return new MapChip(chipType);
			break;
	}
}

void MapChip::ConvertChipNumberToTexRect(int mapChipNumber, Rectangle<Point<float>>* pRet) {
	int chipPosX = mapChipNumber % CHIP_COUNT_PER_ROW;
	int chipPosY = mapChipNumber / CHIP_COUNT_PER_ROW;

	pRet->topLeft.x = (float)chipPosX / (float)CHIP_COUNT_PER_ROW;
	pRet->topLeft.y = (float)chipPosY / (float)CHIP_COUNT_PER_COL;
	pRet->bottomRight.x = (float)(chipPosX + 1) / (float)CHIP_COUNT_PER_ROW;
	pRet->bottomRight.y = (float)(chipPosY + 1) / (float)CHIP_COUNT_PER_COL;
}

void MapChip::ConvertTopLeftPointToRect(const POINT& rTopLeftPoint, Rectangle<POINT>* pRet) {
	pRet->topLeft.x = rTopLeftPoint.x;
	pRet->topLeft.y = rTopLeftPoint.y;
	pRet->bottomRight.x = rTopLeftPoint.x + MapChip::WIDTH;
	pRet->bottomRight.y = rTopLeftPoint.y + MapChip::HEIGHT;
}


/* Getters / Setters -------------------------------------------------------------------------------- */
MapChipType MapChip::GetChipType() const {
	return this->chipType;
}

void MapChip::GetTopLeftPoint(POINT* pRet) const {
	pRet->x = this->drawingVertexRect.topLeft.x;
	pRet->y = this->drawingVertexRect.topLeft.y;
}

void MapChip::SetTopLeftPoint(const POINT& rTopLeftPoint) {
	this->drawingVertexRect.topLeft.x = rTopLeftPoint.x;
	this->drawingVertexRect.topLeft.y = rTopLeftPoint.y;
	this->drawingVertexRect.bottomRight.x = rTopLeftPoint.x + MapChip::WIDTH;
	this->drawingVertexRect.bottomRight.y = rTopLeftPoint.y + MapChip::HEIGHT;
}


/* Public Functions --------------------------------------------------------------------------------- */
void MapChip::AssignChipNumber() {
	this->chipNumber = (int)this->chipType;

	// テクスチャー上のマップチップの位置
	ConfigureTexRect();
}

void MapChip::CreateDrawingVertexRect(Rectangle<Drawing::DrawingVertex>* pRet) const {
	pRet->topLeft.x = this->drawingVertexRect.topLeft.x;
	pRet->topLeft.y = this->drawingVertexRect.topLeft.y;
	pRet->topLeft.tu = this->drawingVertexRect.topLeft.tu;
	pRet->topLeft.tv = this->drawingVertexRect.topLeft.tv;

	pRet->bottomRight.x = this->drawingVertexRect.bottomRight.x;
	pRet->bottomRight.y = this->drawingVertexRect.bottomRight.y;
	pRet->bottomRight.tu = this->drawingVertexRect.bottomRight.tu;
	pRet->bottomRight.tv = this->drawingVertexRect.bottomRight.tv;
}


/* Private Constructor / Destructor ----------------------------------------------------------------- */
MapChip::MapChip(MapChipType chipType) : chipType(chipType) {}


/* Private Functions -------------------------------------------------------------------------------- */
void MapChip::ConfigureTexRect() {
	Rectangle<Point<float>> texRect;
	ConvertChipNumberToTexRect(this->chipNumber, &texRect);
	this->drawingVertexRect.topLeft.tu = texRect.topLeft.x;
	this->drawingVertexRect.topLeft.tv = texRect.topLeft.y;
	this->drawingVertexRect.bottomRight.tu = texRect.bottomRight.x;
	this->drawingVertexRect.bottomRight.tv = texRect.bottomRight.y;
}

}
}