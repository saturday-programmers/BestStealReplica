#include <algorithm>

#include "Map.h"
#include "../AppCommon.h"
#include "../Map/MapChip.h"
#include "../Map/MapChipWall.h"
#include "../Map/MapChipGate.h"
#include "../Map/MapChipJewelry.h"
#include "../Drawing/Drawer.h"
#include "../Stage/IStage.h"
#include "../Map/Stone.h"


namespace BestStealReplica {
namespace Map {

/* Constructor / Destructor ------------------------------------------------------------------------- */
Map::Map() {
	this->topLeftPoint.x = 0;
	this->topLeftPoint.y = 0;
}

Map::~Map() {
	for (auto& pRow : this->pMapData) {
		for (auto& pCell : pRow) {
			delete pCell;
		}
	}

	for (auto& pStone : this->pStones) {
		delete pStone;
	}
}


/* Getters / Setters -------------------------------------------------------------------------------- */
MapChipType Map::GetMapChipType(const POINT& rMapChipPos) const {
	return this->pMapData[rMapChipPos.y][rMapChipPos.x]->GetChipType();
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Map::ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const {
	pRet->push_back(Drawing::TextureType::MAP);
}

void Map::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const {
	for (auto& pRow : this->pMapData) {
		for (auto& pCell : pRow) {
			Drawing::DrawingContext context;
			pCell->CreateDrawingVertexRect(&context.rect);
			context.textureType = Drawing::TextureType::MAP;
			pRet->push_back(context);
		}
	}

	for (auto& pStone : this->pStones) {
		pStone->CreateDrawingContexts(pRet);
	}
}

void Map::Load(const Stage::IStage& rStage) {
	int yChipCount = rStage.GetYChipCount();
	int xChipCount = rStage.GetXChipCount();

	this->pMapData.resize(yChipCount);
	for (int rowIdx = 0; rowIdx < yChipCount; ++rowIdx) {
		this->pMapData[rowIdx].resize(xChipCount);

		for (int colIdx = 0; colIdx < xChipCount; ++colIdx) {			
			this->pMapData[rowIdx][colIdx] = MapChip::Create(rStage.GetMapChipType(rowIdx, colIdx));

			switch (this->pMapData[rowIdx][colIdx]->GetChipType()) {
				case MapChipType::GATE:
				case MapChipType::GOLD_GATE:
					this->pGateMapChips.push_back((MapChipGate*)this->pMapData[rowIdx][colIdx]);
					break;
				case MapChipType::JEWELRY:
					this->pJewelryMapChip = (MapChipJewelry*)this->pMapData[rowIdx][colIdx];
					break;
				default:
					break;
			}
		}
	}

	// プレイヤーが画面中央になる場合の座標を計算
	POINT playerChipPos;
	rStage.GetPlayerFirstChipPos(&playerChipPos);
	POINT center;
	center.x = AppCommon::GetWindowWidth() / 2;
	center.y = AppCommon::GetWindowHeight() / 2;
	this->topLeftPoint.x = center.x - (playerChipPos.x * MapChip::WIDTH);
	this->topLeftPoint.y = center.y - (playerChipPos.y * MapChip::HEIGHT);

	// 左上が余白になる場合は余白が0になるように調整
	if (this->topLeftPoint.x > 0) {
		this->topLeftPoint.x = 0;
	}
	if (this->topLeftPoint.y > 0) {
		this->topLeftPoint.y = 0;
	}

	// 右下が余白になる場合は余白が0になるように調整
	POINT minTopLeft;
	minTopLeft.x = AppCommon::GetWindowWidth() - (xChipCount * MapChip::WIDTH);
	minTopLeft.y = AppCommon::GetWindowHeight() - (yChipCount * MapChip::HEIGHT);
	if (this->topLeftPoint.x < minTopLeft.x) {
		this->topLeftPoint.x = minTopLeft.x;
	}
	if (this->topLeftPoint.y < minTopLeft.y) {
		this->topLeftPoint.y = minTopLeft.y;
	}
	this->defaultTopLeftPoint = this->topLeftPoint;

	AssignChipNumber();

	ConfigureChipPoint();
}

void Map::Move(const POINT& rPixel) {
	this->topLeftPoint.x += rPixel.x;
	this->topLeftPoint.y += rPixel.y;
	ConfigureChipPoint();

	std::for_each(this->pStones.begin(), this->pStones.end(), [rPixel](Stone* pStone) -> void { pStone->Move(rPixel); });
}

void Map::MoveToDefault() {
	this->topLeftPoint = this->defaultTopLeftPoint;
	ConfigureChipPoint();
}

bool Map::IsMovableX(int pixel) const {
	return IsMovable(pixel, this->topLeftPoint.x, (int)this->pMapData[0].size(), MapChip::WIDTH, AppCommon::GetWindowWidth());
}

bool Map::IsMovableY(int pixel) const {
	return IsMovable(pixel, this->topLeftPoint.y, (int)this->pMapData.size(), MapChip::HEIGHT, AppCommon::GetWindowHeight());
}

bool Map::IsOnRoad(const Rectangle<POINT>& rRect) const {
	// 4頂点のチップ位置を取得
	POINT topLeftChipPos;
	ConvertToMapChipPos(rRect.topLeft, &topLeftChipPos);

	POINT topRightPoint;
	topRightPoint.x = rRect.bottomRight.x;
	topRightPoint.y = rRect.topLeft.y;
	POINT topRightChipPos;
	ConvertToMapChipPos(topRightPoint, &topRightChipPos);

	POINT bottomRightChipPos;
	ConvertToMapChipPos(rRect.bottomRight, &bottomRightChipPos);

	POINT bottomLeftPoint;
	bottomLeftPoint.x = rRect.topLeft.x;
	bottomLeftPoint.y = rRect.bottomRight.y;
	POINT bottomLeftChipPos;
	ConvertToMapChipPos(bottomLeftPoint, &bottomLeftChipPos);

	return IsOnRoad(topLeftChipPos) && IsOnRoad(topRightChipPos) && IsOnRoad(bottomRightChipPos) && IsOnRoad(bottomLeftChipPos);
}

void Map::ConvertMapChipPosToTopLeftPoint(const POINT& rMapChipPos, POINT* pRet) const {
	this->pMapData[rMapChipPos.y][rMapChipPos.x]->GetTopLeftPoint(pRet);
}

void Map::KeepOpeningGates() {
	// 開きかけのドアがあればアニメーション
	for (auto& pGateMapChip : this->pGateMapChips) {
		switch (pGateMapChip->GetState()) {
			case MapChipGate::State::START_OPENING:
			case MapChipGate::State::OPENING:
				pGateMapChip->OpenGate();
				break;
			default:
				break;
		}
	}
}

/**
 * キャラクターの目の前のマップチップ取得
 * 
 * @param[in] rCharacterRect キャラクターの位置（Pixel）
 * @param[in] headingDirection キャラクターの向き
 * @param[out] pRet キャラクターの目の前のマップチップの行列番号を格納して返却
 */
void Map::ConvertToCharacterFrontMapChipPos(const Rectangle<POINT>& rCharacterRect, AppCommon::Direction headingDirection, POINT* pRet) const {
	POINT characterCenter;
	characterCenter.x = (rCharacterRect.bottomRight.x + rCharacterRect.topLeft.x) / 2;
	characterCenter.y = (rCharacterRect.bottomRight.y + rCharacterRect.topLeft.y) / 2;
	ConvertToMapChipPos(characterCenter, pRet);

	switch (headingDirection) {
		case AppCommon::Direction::TOP:
			if (pRet->y > 0) {
				--pRet->y;
			}
			break;
		case AppCommon::Direction::RIGHT:
			if (pRet->x < (int)this->pMapData[0].size() - 1) {
				++pRet->x;
			}
			break;
		case AppCommon::Direction::BOTTOM:
			if (pRet->y < (int)this->pMapData.size() - 1) {
				++pRet->y;
			}
			break;
		case AppCommon::Direction::LEFT:
			if (pRet->x > 0) {
				--pRet->x;
			}
			break;
	}
}

bool Map::StartOpeningGate(const POINT& rMapChipPos) {
	bool ret = false;
	switch (this->pMapData[rMapChipPos.y][rMapChipPos.x]->GetChipType()) {
		case MapChipType::GATE:
		case MapChipType::GOLD_GATE:
		{
			MapChipGate* pMapChipGate = (MapChipGate*)this->pMapData[rMapChipPos.y][rMapChipPos.x];
			switch (pMapChipGate->GetState()) {
				case MapChipGate::State::CLOSED:
					pMapChipGate->StartOpeningGate();
					ret = true;
					break;
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
	return ret;
}

bool Map::IsGateOpened(const POINT& rMapChipPos) const {
	bool ret = false;
	MapChip& rChip = *(this->pMapData[rMapChipPos.y][rMapChipPos.x]);
	switch (rChip.GetChipType()) {
		case MapChipType::GATE:
		case MapChipType::GOLD_GATE:
			ret = ((MapChipGate&)rChip).GetState() == MapChipGate::State::OPENED;
			break;
		default:
			break;
	}
	return ret;
}

bool Map::ExistsWallBetween(const POINT& rPoint1, const POINT& rPoint2) const {
	bool ret = false;
	POINT pos1;
	ConvertToMapChipPos(rPoint1, &pos1);
	POINT pos2;
	ConvertToMapChipPos(rPoint2, &pos2);

	if (pos1.x != pos2.x && pos1.y != pos2.y) {
		return true;
	}

	POINT checkPos = pos1;
	while (pos1.x == pos2.x ? checkPos.y != pos2.y : checkPos.x != pos2.x) {
		switch (this->pMapData[checkPos.y][checkPos.x]->GetChipType()) {
			case MapChipType::WALL:
			case MapChipType::WALL_SIDE:
			case MapChipType::JEWELRY:
				ret = true;
				break;
			case MapChipType::GATE:
			case MapChipType::GOLD_GATE:
				if (!IsGateOpened(checkPos)) {
					ret = true;
				}
				break;
			default:
				break;
		}
		if (ret) {
			break;
		}

		if (pos1.x == pos2.x) {
			if (pos1.y < pos2.y) {
				++checkPos.y;
			} else {
				--checkPos.y;
			}
		} else {
			if (pos1.x < pos2.x) {
				++checkPos.x;
			} else {
				--checkPos.x;
			}
		}
	}
	return ret;
}

void Map::OpenJewelryBox() {
	this->pJewelryMapChip->OpenBox();
}

void Map::ConvertToMapChipPos(const POINT& rPoint, POINT* pRet) const {
	if (rPoint.x < this->topLeftPoint.x) {
		pRet->x = -1;
	} else {
		pRet->x = (rPoint.x - this->topLeftPoint.x) / MapChip::WIDTH;
	}

	if (rPoint.y < this->topLeftPoint.y) {
		pRet->y = -1;
	} else {
		pRet->y = (rPoint.y - this->topLeftPoint.y) / MapChip::HEIGHT;
	}
}

void Map::AddStone(const POINT& rTopLeftPoint, AppCommon::Direction direction) {
	this->pStones.push_back(new Stone(rTopLeftPoint, direction));
}

void Map::AnimateStones() {
	for (int i = 0; i < (int)this->pStones.size(); ++i) {
		this->pStones[i]->KeepBeingThrown();
		
		if (this->pStones[i]->Exists()) {
			Rectangle<POINT> nextRectOnGround;
			this->pStones[i]->CalcRectOnGround(&nextRectOnGround);
			if (!IsOnRoad(nextRectOnGround)) {
				// 移動先の地面が道でない場合は戻す				
				this->pStones[i]->SetDropped();
				bool isOnRoad = false;
				while (!isOnRoad) {
					this->pStones[i]->BackOnePixel();
					this->pStones[i]->CalcRectOnGround(&nextRectOnGround);
					isOnRoad = IsOnRoad(nextRectOnGround);
				}
			}
		} else {
			// 表示期間が終わった場合は削除
			Stone* disappearedStone = pStones[i];
			this->pStones.erase(this->pStones.begin() + i);
			delete disappearedStone;
		}
	}
}

void Map::CalcDroppedStonesRect(std::vector<Rectangle<POINT>>* pRet) const {
	for (auto& pStone : this->pStones) {
		switch (pStone->GetState()) {
			case Stone::State::DROPPED:
			case Stone::State::DISAPPEARING:
				Rectangle<POINT> rect;
				pStone->CalcRectOnGround(&rect);
				pRet->push_back(rect);
				break;
			default:
				break;
		}
	}
}


/* Private Functions  ------------------------------------------------------------------------------- */
void Map::AssignChipNumber() {
	for (int rowIdx = 0; rowIdx < (int)this->pMapData.size(); ++rowIdx) {
		for (int colIdx = 0; colIdx < (int)this->pMapData[rowIdx].size(); ++colIdx) {

			if (this->pMapData[rowIdx][colIdx]->GetChipType() == MapChipType::WALL) {
				MapChipWall* chip = (MapChipWall*)this->pMapData[rowIdx][colIdx];
				// 壁とそれ以外のチップの間に線を引く
				if (rowIdx == 0) {
					chip->SetNeedsTopLine();
				} else {
					if (this->pMapData[rowIdx - 1][colIdx]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsTopLine();
					}
				}

				if (colIdx == 0) {
					chip->SetNeedsLeftLine();
				} else {
					if (this->pMapData[rowIdx][colIdx - 1]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsLeftLine();
					}
				}

				if (rowIdx == this->pMapData.size() - 1) {
					chip->SetNeedsBottomLine();
				} else {
					if (this->pMapData[rowIdx + 1][colIdx]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsBottomLine();
					}
				}

				if (colIdx == this->pMapData[rowIdx].size() - 1) {
					chip->SetNeedsRightLine();
				} else {
					if (this->pMapData[rowIdx][colIdx + 1]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsRightLine();
					}
				}
			}
			this->pMapData[rowIdx][colIdx]->AssignChipNumber();
		}

	}
}

void Map::ConfigureChipPoint() {
	POINT point;
	for (int rowIdx = 0; rowIdx < (int)this->pMapData.size(); ++rowIdx) {
		for (int colIdx = 0; colIdx < (int)this->pMapData[rowIdx].size(); ++colIdx) {
			point.x = this->topLeftPoint.x + (colIdx * MapChip::WIDTH);
			point.y = this->topLeftPoint.y + (rowIdx * MapChip::HEIGHT);
			this->pMapData[rowIdx][colIdx]->SetTopLeftPoint(point);
		}
	}
}

bool Map::IsOnRoad(const POINT& rMapChipPos) const {
	if (rMapChipPos.x < 0 || rMapChipPos.x >= (int)this->pMapData[0].size() || rMapChipPos.y < 0 || rMapChipPos.y >= (int)this->pMapData.size()) {
		return false;
	}
	switch (this->pMapData[rMapChipPos.y][rMapChipPos.x]->GetChipType()) {
		case MapChipType::WALL:
		case MapChipType::WALL_SIDE:
		case MapChipType::JEWELRY:
			return false;
		case MapChipType::GATE:
		case MapChipType::GOLD_GATE:
			if (((MapChipGate*)this->pMapData[rMapChipPos.y][rMapChipPos.x])->GetState() != MapChipGate::OPENED) {
				return false;
			}
			break;
	}
	return true;
}

bool Map::IsMovable(int targetPoint, int topLeftPoint, int mapChipCount, int mapChipSize, int windowSize) const {
	if (targetPoint == 0) {
		return true;
	}
	if (targetPoint > 0) {
		if (topLeftPoint + targetPoint > 0) {
			return false;
		}
	} else if (targetPoint < 0) {
		if (topLeftPoint + mapChipCount * mapChipSize <= windowSize) {
			return false;
		}
	}
	return true;
}

}
}