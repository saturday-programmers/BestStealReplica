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
	this->topLeft.x = 0;
	this->topLeft.y = 0;
}

Map::~Map() {
	for (int i = 0; i < (int)this->pMapData.size(); ++i) {
		for (int j = 0; j < (int)this->pMapData[i].size(); ++j) {
			delete this->pMapData[i][j];
		}
	}

	for (int i = 0; i < (int)this->pStones.size(); ++i) {
		delete this->pStones[i];
	}
}


/* Getters / Setters -------------------------------------------------------------------------------- */
std::vector<Drawing::TextureType> Map::GetTextureTypes() const {
	std::vector<Drawing::TextureType> ret;
	ret.push_back(Drawing::TextureType::MAP);
	return ret;
}

POINT Map::GetTopLeftXYonChip(POINT mapChipPos) const {
	return this->pMapData[mapChipPos.y][mapChipPos.x]->GetTopLeftXY();
}

MapChipType Map::GetMapChipType(POINT mapChipPos) const {
	return this->pMapData[mapChipPos.y][mapChipPos.x]->GetChipType();
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Map::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const {
	for (int i = 0; i < (int)this->pMapData.size(); ++i) {
		for (int j = 0; j < (int)this->pMapData[i].size(); ++j) {
			Drawing::DrawingContext context;
			context.vertices = pMapData[i][j]->CreateVertex();
			context.textureType = Drawing::TextureType::MAP;
			pDrawingContexts->push_back(context);
		}
	}

	for (int i = 0; i < (int)this->pStones.size(); ++i) {
		this->pStones[i]->CreateDrawingContexts(pDrawingContexts);
	}
}

void Map::Load(const Stage::IStage& rStage) {
	int yChipCount = rStage.GetYChipCount();
	int xChipCount = rStage.GetXChipCount();

	this->pMapData.resize(yChipCount);
	for (int i = 0; i < yChipCount; ++i) {
		this->pMapData[i].resize(xChipCount);

		for (int j = 0; j < xChipCount; ++j) {			
			this->pMapData[i][j] = MapChip::Create(rStage.GetMapChipType(i, j));

			switch (this->pMapData[i][j]->GetChipType()) {
				case MapChipType::GATE:
				case MapChipType::GOLD_GATE:
					this->pGateMapChips.push_back((MapChipGate*)this->pMapData[i][j]);
					break;
				case MapChipType::JEWELRY:
					this->pJewelryMapChip = (MapChipJewelry*)this->pMapData[i][j];
					break;
				default:
					break;
			}
		}
	}

	// プレイヤーが画面中央になる場合の座標を計算
	POINT playerChipPos = rStage.GetPlayerFirstChipPos();
	POINT centerXY;
	centerXY.x = AppCommon::GetWindowWidth() / 2;
	centerXY.y = AppCommon::GetWindowHeight() / 2;
	this->topLeft.x = centerXY.x - (playerChipPos.x * MapChip::WIDTH);
	this->topLeft.y = centerXY.y - (playerChipPos.y * MapChip::HEIGHT);

	// 左上が余白になる場合は余白が0になるように調整
	if (this->topLeft.x > 0) {
		this->topLeft.x = 0;
	}
	if (this->topLeft.y > 0) {
		this->topLeft.y = 0;
	}

	// 右下が余白になる場合は余白が0になるように調整
	POINT minTopLeft;
	minTopLeft.x = AppCommon::GetWindowWidth() - (xChipCount * MapChip::WIDTH);
	minTopLeft.y = AppCommon::GetWindowHeight() - (yChipCount * MapChip::HEIGHT);
	if (this->topLeft.x < minTopLeft.x) {
		this->topLeft.x = minTopLeft.x;
	}
	if (this->topLeft.y < minTopLeft.y) {
		this->topLeft.y = minTopLeft.y;
	}
	this->defaultTopLeft = this->topLeft;

	for (int i = 0; i < yChipCount; ++i) {
		for (int j = 0; j < xChipCount; ++j) {
			if (this->pMapData[i][j]->GetChipType() == MapChipType::WALL) {
				MapChipWall* chip = (MapChipWall*)this->pMapData[i][j];
				// 壁とそれ以外のチップの間に線を引く
				if (i == 0) {
					chip->SetNeedsTopLine();
				} else {
					if (this->pMapData[i - 1][j]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsTopLine();
					}
				}

				if (j == 0) {
					chip->SetNeedsLeftLine();
				} else {
					if (this->pMapData[i][j - 1]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsLeftLine();
					}
				}

				if (i == yChipCount - 1) {
					chip->SetNeedsBottomLine();
				} else {
					if (this->pMapData[i + 1][j]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsBottomLine();
					}
				}

				if (j == xChipCount - 1) {
					chip->SetNeedsRightLine();
				} else {
					if (this->pMapData[i][j + 1]->GetChipType() != MapChipType::WALL) {
						chip->SetNeedsRightLine();
					}
				}
			}
			this->pMapData[i][j]->SetChipNumber();
		}
	}
	SetChipXY();
}

void Map::Move(POINT xy) {
	this->topLeft.x += xy.x;
	this->topLeft.y += xy.y;
	SetChipXY();

	std::for_each(this->pStones.begin(), this->pStones.end(), [xy](Stone* pStone) -> void { pStone->Move(xy); });
}

void Map::MoveToDefault() {
	this->topLeft = this->defaultTopLeft;
	SetChipXY();
}

bool Map::IsOnRoad(Vertices<POINT> xy) const {
	// 4頂点のチップ位置を取得
	POINT topLeftChipPos = ConvertToMapChipPos(xy.topLeft);

	POINT topRightXY;
	topRightXY.x = xy.bottomRight.x;
	topRightXY.y = xy.topLeft.y;
	POINT topRightChipPos = ConvertToMapChipPos(topRightXY);

	POINT bottomRightChipPos = ConvertToMapChipPos(xy.bottomRight);

	POINT bottomLeftXY;
	bottomLeftXY.x = xy.topLeft.x;
	bottomLeftXY.y = xy.bottomRight.y;
	POINT bottomLeftChipPos = ConvertToMapChipPos(bottomLeftXY);

	return IsOnRoad(topLeftChipPos) && IsOnRoad(topRightChipPos) && IsOnRoad(bottomRightChipPos) && IsOnRoad(bottomLeftChipPos);
}

bool Map::IsMovableX(int x) const {
	if (x == 0) {
		return true;
	}

	if (x > 0) {
		if (this->topLeft.x + x > 0) {
			return false;
		}
	} else {
		if (this->topLeft.x + (int)this->pMapData[0].size() * MapChip::WIDTH <= AppCommon::GetWindowWidth()) {
			return false;
		}
	}
	return true;
}

bool Map::IsMovableY(int y) const {
	if (y == 0) {
		return true;
	}
	if (y > 0) {
		if (this->topLeft.y + y > 0) {
			return false;
		}
	} else if (y < 0) {
		if (this->topLeft.y + (int)this->pMapData.size() * MapChip::HEIGHT <= AppCommon::GetWindowHeight()) {
			return false;
		}
	}
	return true;
}

void Map::KeepOpeningGates() {
	// 開きかけのドアがあればアニメーション
	for (int i = 0; i < (int)this->pGateMapChips.size(); ++i) {
		switch (this->pGateMapChips[i]->GetState()) {
			case MapChipGate::State::START_OPENING:
			case MapChipGate::State::OPENING:
				this->pGateMapChips[i]->OpenGate();
				break;
			default:
				break;
		}
	}
}

POINT Map::GetFrontMapChipPos(Vertices<POINT> playerXY, AppCommon::Direction headingDirection) const {
	// プレイヤーの目の前のマップチップ取得
	POINT centerXY;
	centerXY.x = (playerXY.bottomRight.x + playerXY.topLeft.x) / 2;
	centerXY.y = (playerXY.bottomRight.y + playerXY.topLeft.y) / 2;
	POINT chipPos = ConvertToMapChipPos(centerXY);

	switch (headingDirection) {
		case AppCommon::Direction::TOP:
			if (chipPos.y > 0) {
				--chipPos.y;
			}
			break;
		case AppCommon::Direction::RIGHT:
			if (chipPos.x < (int)this->pMapData[0].size() - 1) {
				++chipPos.x;
			}
			break;
		case AppCommon::Direction::BOTTOM:
			if (chipPos.y < (int)this->pMapData.size() - 1) {
				++chipPos.y;
			}
			break;
		case AppCommon::Direction::LEFT:
			if (chipPos.x > 0) {
				--chipPos.x;
			}
			break;
	}

	return chipPos;
}

bool Map::StartOpeningGate(POINT mapChipPos) {
	bool ret = false;
	switch (this->pMapData[mapChipPos.y][mapChipPos.x]->GetChipType()) {
		case MapChipType::GATE:
		case MapChipType::GOLD_GATE:
		{
			MapChipGate* pMapChipGate = (MapChipGate*)this->pMapData[mapChipPos.y][mapChipPos.x];
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

bool Map::IsGateOpened(POINT mapChipPos) const {
	bool ret = false;
	MapChip& rChip = *(this->pMapData[mapChipPos.y][mapChipPos.x]);
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

bool Map::ExistsWallBetween(POINT xy1, POINT xy2) const {
	bool ret = false;
	POINT p1 = ConvertToMapChipPos(xy1);
	POINT p2 = ConvertToMapChipPos(xy2);

	if (p1.x != p2.x && p1.y != p2.y) {
		return true;
	}

	POINT checkPos = p1;
	while (p1.x == p2.x ? checkPos.y != p2.y : checkPos.x != p2.x) {
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

		if (p1.x == p2.x) {
			if (p1.y < p2.y) {
				++checkPos.y;
			} else {
				--checkPos.y;
			}
		} else {
			if (p1.x < p2.x) {
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

POINT Map::ConvertToMapChipPos(POINT xy) const {
	POINT ret;
	if (xy.x < this->topLeft.x) {
		ret.x = -1;
	} else {
		ret.x = (xy.x - this->topLeft.x) / MapChip::WIDTH;
	}

	if (xy.y < this->topLeft.y) {
		ret.y = -1;
	} else {
		ret.y = (xy.y - this->topLeft.y) / MapChip::HEIGHT;
	}
	return ret;
}

void Map::AddStone(POINT topLeftXY, AppCommon::Direction direction) {
	this->pStones.push_back(new Stone(topLeftXY, direction));
}

void Map::AnimateStones() {
	for (int i = 0; i < (int)this->pStones.size(); ++i) {
		POINT topLeftXYOnGndBeforeMoving = this->pStones[i]->GetXYsOnGround().topLeft;
		this->pStones[i]->KeepBeingThrown();
		
		if (this->pStones[i]->Exists()) {
			Vertices<POINT> nextXYOnGround = this->pStones[i]->GetXYsOnGround();
			if (!IsOnRoad(nextXYOnGround)) {
				// 移動先の地面が道でない場合は戻す				
				this->pStones[i]->SetDropped();
				bool isOnRoad = false;
				while (!isOnRoad) {
					this->pStones[i]->BackOnePixcel();
					nextXYOnGround = this->pStones[i]->GetXYsOnGround();
					isOnRoad = IsOnRoad(nextXYOnGround);
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

std::vector<Vertices<POINT>> Map::GetDroppedStoneXYs() const {
	std::vector<Vertices<POINT>> stoneXYs;
	for (int i = 0; i < (int)this->pStones.size(); ++i) {
		switch (this->pStones[i]->GetState()) {
			case Stone::State::DROPPED:
			case Stone::State::DISAPPEARING:
				stoneXYs.push_back(this->pStones[i]->GetXYsOnGround());
				break;
			default:
				break;
		}
	}
	return stoneXYs;
}


/* Private Functions  ------------------------------------------------------------------------------- */
void Map::SetChipXY() {
	POINT point;
	for (int i = 0; i < (int)this->pMapData.size(); ++i) {
		for (int j = 0; j < (int)this->pMapData[i].size(); ++j) {
			point.x = this->topLeft.x + (j * MapChip::WIDTH);
			point.y = this->topLeft.y + (i * MapChip::HEIGHT);
			this->pMapData[i][j]->SetXY(point);
		}
	}
}

bool Map::IsOnRoad(POINT mapChipPos) const {
	if (mapChipPos.x < 0 || mapChipPos.x >= (int)this->pMapData[0].size() || mapChipPos.y < 0 || mapChipPos.y >= (int)this->pMapData.size()) {
		return false;
	}
	switch (this->pMapData[mapChipPos.y][mapChipPos.x]->GetChipType()) {
		case MapChipType::WALL:
		case MapChipType::WALL_SIDE:
		case MapChipType::JEWELRY:
			return false;
		case MapChipType::GATE:
		case MapChipType::GOLD_GATE:
			if (((MapChipGate*)this->pMapData[mapChipPos.y][mapChipPos.x])->GetState() != MapChipGate::OPENED) {
				return false;
			}
			break;
	}
	return true;
}
}
}