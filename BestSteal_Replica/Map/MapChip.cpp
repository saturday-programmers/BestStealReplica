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

Vertices<FloatPoint> MapChip::GetTuTvs(int mapChipNumber) {
	Vertices<FloatPoint> ret;

	int chipPosX = mapChipNumber % CHIP_COUNT_PER_ROW;
	int chipPosY = mapChipNumber / CHIP_COUNT_PER_ROW;

	ret.topLeft.x = (float)chipPosX / (float)CHIP_COUNT_PER_ROW;
	ret.topLeft.y = (float)chipPosY / (float)CHIP_COUNT_PER_COL;
	ret.bottomRight.x = (float)(chipPosX + 1) / (float)CHIP_COUNT_PER_ROW;
	ret.bottomRight.y = (float)(chipPosY + 1) / (float)CHIP_COUNT_PER_COL;

	return ret;
}

Vertices<POINT> MapChip::GetXY(POINT topLeftXY) {
	Vertices<POINT> ret;
	ret.topLeft.x = topLeftXY.x;
	ret.topLeft.y = topLeftXY.y;
	ret.bottomRight.x = topLeftXY.x + MapChip::WIDTH;
	ret.bottomRight.y = topLeftXY.y + MapChip::HEIGHT;
	return ret;
}


/* Getters / Setters -------------------------------------------------------------------------------- */
MapChipType MapChip::GetChipType() const {
	return this->chipType;
}

void MapChip::AssignChipNumber() {
	this->chipNumber = (int)this->chipType;

	// テクスチャー上のマップチップの位置
	SetTuTv();
}

POINT MapChip::GetTopLeftXY() const {
	POINT ret;
	ret.x = this->vertices.topLeft.x;
	ret.y = this->vertices.topLeft.y;
	return ret;
}

void MapChip::SetXY(POINT topLeftXY) {
	Vertices<POINT> xy = GetXY(topLeftXY);
	this->vertices.topLeft.x = xy.topLeft.x;
	this->vertices.topLeft.y = xy.topLeft.y;
	this->vertices.bottomRight.x = xy.bottomRight.x;
	this->vertices.bottomRight.y = xy.bottomRight.y;
}

Vertices<Drawing::DrawingVertex> MapChip::CreateVertex() const {
	return this->vertices;
}


/* Private Constructor / Destructor ----------------------------------------------------------------- */
MapChip::MapChip(MapChipType chipType) : chipType(chipType) {}


/* Private Functions  ------------------------------------------------------------------------------- */
void MapChip::SetTuTv() {
	Vertices<FloatPoint> tutv = GetTuTvs(this->chipNumber);
	this->vertices.topLeft.tu = tutv.topLeft.x;
	this->vertices.topLeft.tv = tutv.topLeft.y;
	this->vertices.bottomRight.tu = tutv.bottomRight.x;
	this->vertices.bottomRight.tv = tutv.bottomRight.y;
}

}
}