#include "MapChipSetting.h"


namespace BestStealReplica {
namespace Map {

Vertices<FloatPoint> MapChipSetting::GetTuTvs(int mapChipNumber) {
	Vertices<FloatPoint> ret;

	int chipPosX = mapChipNumber % CHIP_COUNT_PER_ROW;
	int chipPosY = mapChipNumber / CHIP_COUNT_PER_ROW;

	ret.topLeft.x = (float)chipPosX / (float)CHIP_COUNT_PER_ROW;
	ret.topLeft.y = (float)chipPosY / (float)CHIP_COUNT_PER_COL;
	ret.bottomRight.x = (float)(chipPosX + 1) / (float)CHIP_COUNT_PER_ROW;
	ret.bottomRight.y = (float)(chipPosY + 1) / (float)CHIP_COUNT_PER_COL;

	return ret;
}

Vertices<POINT> MapChipSetting::GetXY(POINT topLeftXY) {
	Vertices<POINT> ret;

	ret.topLeft.x = topLeftXY.x;
	ret.topLeft.y = topLeftXY.y;
	ret.bottomRight.x = topLeftXY.x + WIDTH;
	ret.bottomRight.y = topLeftXY.y + HEIGHT;
	return ret;
}



MapChipSetting::MapChipSetting() {}

MapChipSetting::MapChipSetting(MapCommon::MapChipType chipType) :
	chipType(chipType)
{}

MapCommon::MapChipType MapChipSetting::GetChipType() {
	return this->chipType;
}

void MapChipSetting::SetChipNumber() {
	this->chipNumber = (int)this->chipType;
	if (this->chipType == MapCommon::MapChipType::WALL) {
		// 線のパターンに合わせてチップ番号を指定
		if (!this->needsTopLine && !this->needsRightLine && !this->needsBottomLine && !this->needsLeftLine) {
			// 四方線がないチップはデフォルトのものを使用
		} else if (this->needsTopLine && this->needsRightLine && this->needsBottomLine && this->needsLeftLine) {
			this->chipNumber = ChipNumber::WALL_WITH_SURROUNDED_LINE;
		} else {
			if (this->needsTopLine) {
				if (this->needsRightLine) {
					if (this->needsBottomLine) {
						this->chipNumber = ChipNumber::WALL_WITHOUT_LEFT_LINE;
					} else {
						if (this->needsLeftLine) {
							this->chipNumber = ChipNumber::WALL_WITHOUT_BOTTOM_LINE;
						} else {
							this->chipNumber = ChipNumber::WALL_WITH_TOP_RIGHT_LINE;
						}
					}
				} else {
					if (this->needsBottomLine) {
						if (this->needsLeftLine) {
							this->chipNumber = ChipNumber::WALL_WITHOUT_RIGHT_LINE;
						} else {
							this->chipNumber = ChipNumber::WALL_WITH_TOP_BOTTOM_LINE;
						}
					} else {
						if (this->needsLeftLine) {
							this->chipNumber = ChipNumber::WALL_WITH_TOP_LEFT_LINE;
						} else {
							this->chipNumber = ChipNumber::WALL_WITH_TOP_LINE;
						}
					}
				}
			} else {
				if (this->needsRightLine) {
					if (this->needsBottomLine) {
						if (this->needsLeftLine) {
							this->chipNumber = ChipNumber::WALL_WITHOUT_TOP_LINE;
						} else {
							this->chipNumber = ChipNumber::WALL_WITH_BOTTOM_RIGHT_LINE;
						}
					} else {
						if (this->needsLeftLine) {
							this->chipNumber = ChipNumber::WALL_WITH_RIGHT_LEFT_LINE;
						} else {
							this->chipNumber = ChipNumber::WALL_WITH_RIGHT_LINE;
						}
					}
				} else {
					if (this->needsBottomLine) {
						if (this->needsLeftLine) {
							this->chipNumber = ChipNumber::WALL_WITH_BOTTOM_LEFT_LINE;
						} else {
							this->chipNumber = ChipNumber::WALL_WITH_BOTTOM_LINE;
						}
					} else {
						this->chipNumber = ChipNumber::WALL_WITH_LEFT_LINE;
					}
				}
			}
		}
	}
	// テクスチャー上のマップチップの位置
	Vertices<FloatPoint> tutv = GetTuTvs(this->chipNumber);
	this->vertices.topLeft.tu = tutv.topLeft.x;
	this->vertices.topLeft.tv = tutv.topLeft.y;
	this->vertices.bottomRight.tu = tutv.bottomRight.x;
	this->vertices.bottomRight.tv = tutv.bottomRight.y;
}

POINT MapChipSetting::GetTopLeftXY() {
	POINT ret;
	ret.x = (int)this->vertices.topLeft.x;
	ret.y = (int)this->vertices.topLeft.y;
	return ret;
}

void MapChipSetting::SetXY(POINT topLeftXY) {
	Vertices<POINT> xy = GetXY(topLeftXY);
	this->vertices.topLeft.x = xy.topLeft.x;
	this->vertices.topLeft.y = xy.topLeft.y;
	this->vertices.bottomRight.x = xy.bottomRight.x;
	this->vertices.bottomRight.y = xy.bottomRight.y;
}


void MapChipSetting::SetNeedsTopLine() {
	this->needsTopLine = true;
}

void MapChipSetting::SetNeedsRightLine() {
	this->needsRightLine = true;
}

void MapChipSetting::SetNeedsBottomLine() {
	this->needsBottomLine = true;
}

void MapChipSetting::SetNeedsLeftLine() {
	this->needsLeftLine = true;
}

Vertices<DrawingVertex> MapChipSetting::GetVertex() {
	return this->vertices;
}
}
}