#include "Map.h"
#include "AppCommon.h"
#include "MapChipSetting.h"
#include "Drawer.h"
#include "IStage.h"


namespace BestStealReplica {
namespace Map {

Map::Map(int yChipCount, int xChipCount, Drawer* pDrawer) :
	FILE_PATH("image\\mapchip.png"),
	pDrawer(pDrawer),
	yChipCount(yChipCount),
	xChipCount(xChipCount)
{
	topLeft.x = 0;
	topLeft.y = 0;
	pDrawer->CreateTexture(this->FILE_PATH, Drawer::TextureType::MAP);
}

void Map::Load(const IStage* pStage) {
	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			this->mapData[i][j] = MapChipSetting(pStage->GetMapChipType(i, j));
		}
	}

	// プレイヤーが画面中央になる場合の座標を計算
	POINT playerChipPos = pStage->GetPlayerFirstChipPos();
	POINT centerXY;
	centerXY.x = AppCommon::GetWindowWidth() / 2;
	centerXY.y = AppCommon::GetWindowHeight() / 2;
	this->topLeft.x = centerXY.x - (playerChipPos.x * MapChipSetting::WIDTH);
	this->topLeft.y = centerXY.y - (playerChipPos.y * MapChipSetting::HEIGHT);

	// 左上が余白になる場合は余白が0になるように調整
	if (this->topLeft.x > 0) {
		this->topLeft.x = 0;
	}
	if (this->topLeft.y > 0) {
		this->topLeft.y = 0;
	}

	// 右下が余白になる場合は余白が0になるように調整
	POINT minTopLeft;
	minTopLeft.x = AppCommon::GetWindowWidth() - (this->xChipCount * MapChipSetting::WIDTH);
	minTopLeft.y = AppCommon::GetWindowHeight() - (this->yChipCount * MapChipSetting::HEIGHT);
	if (this->topLeft.x < minTopLeft.x) {
		this->topLeft.x = minTopLeft.x;
	}
	if (this->topLeft.y < minTopLeft.y) {
		this->topLeft.y = minTopLeft.y;
	}

	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			if (this->mapData[i][j].GetChipType() == MapCommon::MapChipType::WALL) {
				// 壁とそれ以外のチップの間に線を引く
				if (i == 0) {
					this->mapData[i][j].SetNeedsTopLine();
				} else {
					if (this->mapData[i - 1][j].GetChipType() != MapCommon::MapChipType::WALL) {
						this->mapData[i][j].SetNeedsTopLine();
					}
				}

				if (j == 0) {
					this->mapData[i][j].SetNeedsLeftLine();
				} else {
					if (this->mapData[i][j - 1].GetChipType() != MapCommon::MapChipType::WALL) {
						this->mapData[i][j].SetNeedsLeftLine();
					}
				}

				if (i == yChipCount - 1) {
					this->mapData[i][j].SetNeedsBottomLine();
				} else {
					if (this->mapData[i + 1][j].GetChipType() != MapCommon::MapChipType::WALL) {
						this->mapData[i][j].SetNeedsBottomLine();
					}
				}

				if (j == xChipCount - 1) {
					this->mapData[i][j].SetNeedsRightLine();
				} else {
					if (this->mapData[i][j + 1].GetChipType() != MapCommon::MapChipType::WALL) {
						this->mapData[i][j].SetNeedsRightLine();
					}
				}
			}
			this->mapData[i][j].SetChipNumber();
		}
	}
	SetChipXY();
}

void Map::Draw() {
	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			this->pDrawer->Draw(mapData[i][j].GetVertex(), Drawer::TextureType::MAP);
		}
	}
}

void Map::Move(POINT xy) {
	this->topLeft.x += xy.x;
	this->topLeft.y += xy.y;
	SetChipXY();
}

POINT Map::GetTopLeftXYonChip(POINT mapChipPos) {
	return this->mapData[mapChipPos.y][mapChipPos.x].GetTopLeftXY();
}

bool Map::IsOnRoad(Vertices<POINT> xy) {
	// 4頂点のチップ位置を取得
	POINT topLeftChipPos = GetMapChipPos(xy.topLeft);

	POINT topRightXY;
	topRightXY.x = xy.bottomRight.x;
	topRightXY.y = xy.topLeft.y;
	POINT topRightChipPos = GetMapChipPos(topRightXY);

	POINT bottomRightChipPos = GetMapChipPos(xy.bottomRight);

	POINT bottomLeftXY;
	bottomLeftXY.x = xy.topLeft.x;
	bottomLeftXY.y = xy.bottomRight.y;
	POINT bottomLeftChipPos = GetMapChipPos(bottomLeftXY);

	return IsOnRoad(topLeftChipPos) && IsOnRoad(topRightChipPos) && IsOnRoad(bottomRightChipPos) && IsOnRoad(bottomLeftChipPos);
}

bool Map::IsMovableX(int x) {
	if (x == 0) {
		return true;
	}

	if (x > 0) {
		if (this->topLeft.x + x > 0) {
			return false;
		}
	} else {
		if (this->topLeft.x + this->xChipCount * MapChipSetting::WIDTH <= AppCommon::GetWindowWidth()) {
			return false;
		}
	}
	return true;
}

bool Map::IsMovableY(int y) {
	if (y == 0) {
		return true;
	}
	if (y > 0) {
		if (this->topLeft.y + y > 0) {
			return false;
		}
	} else if (y < 0) {
		if (this->topLeft.y + this->yChipCount * MapChipSetting::HEIGHT <= AppCommon::GetWindowHeight()) {
			return false;
		}
	}
	return true;
}


void Map::SetChipXY() {
	POINT point;
	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			point.x = this->topLeft.x + (j * MapChipSetting::WIDTH);
			point.y = this->topLeft.y + (i * MapChipSetting::HEIGHT);
			this->mapData[i][j].SetXY(point);
		}
	}
}

POINT Map::GetMapChipPos(POINT xy) {
	POINT ret;
	if (xy.x < 0) {
		ret.x = -1;
	} else {
		ret.x = (xy.x - this->topLeft.x) / MapChipSetting::WIDTH;
	}
	if (xy.y < 0) {
		ret.y = -1;
	} else {
		ret.y = (xy.y - this->topLeft.y) / MapChipSetting::HEIGHT;
	}
	return ret;
}

bool Map::IsOnRoad(POINT mapChipPos) {
	if (mapChipPos.x < 0 || mapChipPos.x >= xChipCount || mapChipPos.y < 0 || mapChipPos.y >= yChipCount) {
		return false;
	}
	switch (this->mapData[mapChipPos.y][mapChipPos.x].GetChipType()) {
		case MapCommon::MapChipType::WALL:
		case MapCommon::MapChipType::WALL_SIDE:
			return false;
	}
	return true;
}
}
}