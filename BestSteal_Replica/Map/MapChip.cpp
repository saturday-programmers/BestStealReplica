#include "MapChip.h"
#include "../Map/MapChipWall.h"
#include "../Map/MapChipGate.h"
#include "../Map/MapChipJewelry.h"


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

void MapChip::ConvertChipNumberToTuTv(int mapChipNumber, Vertices<FloatPoint>* pRet) {
	int chipPosX = mapChipNumber % CHIP_COUNT_PER_ROW;
	int chipPosY = mapChipNumber / CHIP_COUNT_PER_ROW;

	pRet->topLeft.x = (float)chipPosX / (float)CHIP_COUNT_PER_ROW;
	pRet->topLeft.y = (float)chipPosY / (float)CHIP_COUNT_PER_COL;
	pRet->bottomRight.x = (float)(chipPosX + 1) / (float)CHIP_COUNT_PER_ROW;
	pRet->bottomRight.y = (float)(chipPosY + 1) / (float)CHIP_COUNT_PER_COL;
}

void MapChip::ConvertTopLeftXYToVertices(const POINT& rTopLeftXY, Vertices<POINT>* pRet) {
	pRet->topLeft.x = rTopLeftXY.x;
	pRet->topLeft.y = rTopLeftXY.y;
	pRet->bottomRight.x = rTopLeftXY.x + MapChip::WIDTH;
	pRet->bottomRight.y = rTopLeftXY.y + MapChip::HEIGHT;
}


/* Getters / Setters -------------------------------------------------------------------------------- */
MapChipType MapChip::GetChipType() const {
	return this->chipType;
}

void MapChip::GetTopLeftXY(POINT* pRet) const {
	pRet->x = this->vertices.topLeft.x;
	pRet->y = this->vertices.topLeft.y;
}

void MapChip::SetXY(const POINT& rTopLeftXY) {
	this->vertices.topLeft.x = rTopLeftXY.x;
	this->vertices.topLeft.y = rTopLeftXY.y;
	this->vertices.bottomRight.x = rTopLeftXY.x + MapChip::WIDTH;
	this->vertices.bottomRight.y = rTopLeftXY.y + MapChip::HEIGHT;
}


/* Public Functions --------------------------------------------------------------------------------- */
void MapChip::AssignChipNumber() {
	this->chipNumber = (int)this->chipType;

	// テクスチャー上のマップチップの位置
	ConfigureTuTv();
}

void MapChip::CreateDrawingVertices(Vertices<Drawing::DrawingVertex>* pRet) const {
	pRet->topLeft.x = this->vertices.topLeft.x;
	pRet->topLeft.y = this->vertices.topLeft.y;
	pRet->topLeft.tu = this->vertices.topLeft.tu;
	pRet->topLeft.tv = this->vertices.topLeft.tv;

	pRet->bottomRight.x = this->vertices.bottomRight.x;
	pRet->bottomRight.y = this->vertices.bottomRight.y;
	pRet->bottomRight.tu = this->vertices.bottomRight.tu;
	pRet->bottomRight.tv = this->vertices.bottomRight.tv;
}


/* Private Constructor / Destructor ----------------------------------------------------------------- */
MapChip::MapChip(MapChipType chipType) : chipType(chipType) {}


/* Private Functions -------------------------------------------------------------------------------- */
void MapChip::ConfigureTuTv() {
	Vertices<FloatPoint> tutv;
	ConvertChipNumberToTuTv(this->chipNumber, &tutv);
	this->vertices.topLeft.tu = tutv.topLeft.x;
	this->vertices.topLeft.tv = tutv.topLeft.y;
	this->vertices.bottomRight.tu = tutv.bottomRight.x;
	this->vertices.bottomRight.tv = tutv.bottomRight.y;
}

}
}