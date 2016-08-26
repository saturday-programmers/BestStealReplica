#include "Map.h"
#include "AppCommon.h"
#include "MapChip.h"
#include "MapChipWall.h"
#include "MapChipJewelry.h"
#include "Drawer.h"
#include "IStage.h"


namespace BestStealReplica {
namespace Map {

/* Constructor / Destructor ------------------------------------------------------------------------- */
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

Map::~Map() {
	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			delete this->mapData[i][j];
		}
	}
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Map::Load(const Stage::IStage* pStage) {
	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			this->mapData[i][j] = MapChip::Create(pStage->GetMapChipType(i, j));

			switch (this->mapData[i][j]->GetChipType()) {
				case MapCommon::MapChipType::DOOR:
				case MapCommon::MapChipType::GOLD_DOOR:
					this->doorMapChips.push_back((MapChipDoor*)this->mapData[i][j]);
					break;
				case MapCommon::MapChipType::JEWELRY:
					this->jewelryMapChip = (MapChipJewelry*)this->mapData[i][j];
					break;
				default:
					break;
			}
		}
	}

	// �v���C���[����ʒ����ɂȂ�ꍇ�̍��W���v�Z
	POINT playerChipPos = pStage->GetPlayerFirstChipPos();
	POINT centerXY;
	centerXY.x = AppCommon::GetWindowWidth() / 2;
	centerXY.y = AppCommon::GetWindowHeight() / 2;
	this->topLeft.x = centerXY.x - (playerChipPos.x * MapChip::WIDTH);
	this->topLeft.y = centerXY.y - (playerChipPos.y * MapChip::HEIGHT);

	// ���オ�]���ɂȂ�ꍇ�͗]����0�ɂȂ�悤�ɒ���
	if (this->topLeft.x > 0) {
		this->topLeft.x = 0;
	}
	if (this->topLeft.y > 0) {
		this->topLeft.y = 0;
	}

	// �E�����]���ɂȂ�ꍇ�͗]����0�ɂȂ�悤�ɒ���
	POINT minTopLeft;
	minTopLeft.x = AppCommon::GetWindowWidth() - (this->xChipCount * MapChip::WIDTH);
	minTopLeft.y = AppCommon::GetWindowHeight() - (this->yChipCount * MapChip::HEIGHT);
	if (this->topLeft.x < minTopLeft.x) {
		this->topLeft.x = minTopLeft.x;
	}
	if (this->topLeft.y < minTopLeft.y) {
		this->topLeft.y = minTopLeft.y;
	}
	this->defaultTopLeft = this->topLeft;

	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			if (this->mapData[i][j]->GetChipType() == MapCommon::MapChipType::WALL) {
				MapChipWall* chip = (MapChipWall*)this->mapData[i][j];
				// �ǂƂ���ȊO�̃`�b�v�̊Ԃɐ�������
				if (i == 0) {
					chip->SetNeedsTopLine();
				} else {
					if (this->mapData[i - 1][j]->GetChipType() != MapCommon::MapChipType::WALL) {
						chip->SetNeedsTopLine();
					}
				}

				if (j == 0) {
					chip->SetNeedsLeftLine();
				} else {
					if (this->mapData[i][j - 1]->GetChipType() != MapCommon::MapChipType::WALL) {
						chip->SetNeedsLeftLine();
					}
				}

				if (i == yChipCount - 1) {
					chip->SetNeedsBottomLine();
				} else {
					if (this->mapData[i + 1][j]->GetChipType() != MapCommon::MapChipType::WALL) {
						chip->SetNeedsBottomLine();
					}
				}

				if (j == xChipCount - 1) {
					chip->SetNeedsRightLine();
				} else {
					if (this->mapData[i][j + 1]->GetChipType() != MapCommon::MapChipType::WALL) {
						chip->SetNeedsRightLine();
					}
				}
			}
			this->mapData[i][j]->SetChipNumber();
		}
	}
	SetChipXY();
}

void Map::Draw() {
	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			this->pDrawer->Draw(mapData[i][j]->GetVertex(), Drawer::TextureType::MAP);
		}
	}
}

void Map::Move(POINT xy) {
	this->topLeft.x += xy.x;
	this->topLeft.y += xy.y;
	SetChipXY();
}

void Map::MoveToDefault() {
	this->topLeft = this->defaultTopLeft;
	SetChipXY();
}

POINT Map::GetTopLeftXYonChip(POINT mapChipPos) {
	return this->mapData[mapChipPos.y][mapChipPos.x]->GetTopLeftXY();
}

bool Map::IsOnRoad(Vertices<POINT> xy) {
	// 4���_�̃`�b�v�ʒu���擾
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
		if (this->topLeft.x + this->xChipCount * MapChip::WIDTH <= AppCommon::GetWindowWidth()) {
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
		if (this->topLeft.y + this->yChipCount * MapChip::HEIGHT <= AppCommon::GetWindowHeight()) {
			return false;
		}
	}
	return true;
}

void Map::KeepOpeningDoors() {
	// �J�������̃h�A������΃A�j���[�V����
	for (int i = 0; i < (int)this->doorMapChips.size(); ++i) {
		switch (this->doorMapChips[i]->state) {
			case MapChipDoor::State::START_OPENING:
			case MapChipDoor::State::OPENING:
				this->doorMapChips[i]->OpenDoor();
				break;
			default:
				break;
		}
	}
}

POINT Map::GetFrontMapChipPos(Vertices<POINT> playerXY, AppCommon::Direction headingDirection) {
	// �v���C���[�̖ڂ̑O�̃}�b�v�`�b�v�擾
	POINT centerXY;
	centerXY.x = (playerXY.bottomRight.x + playerXY.topLeft.x) / 2;
	centerXY.y = (playerXY.bottomRight.y + playerXY.topLeft.y) / 2;
	POINT chipPos = GetMapChipPos(centerXY);

	switch (headingDirection) {
		case AppCommon::Direction::TOP:
			if (chipPos.y > 0) {
				--chipPos.y;
			}
			break;
		case AppCommon::Direction::RIGHT:
			if (chipPos.x < this->xChipCount - 1) {
				++chipPos.x;
			}
			break;
		case AppCommon::Direction::BOTTOM:
			if (chipPos.y < this->yChipCount - 1) {
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

bool Map::StartOpeningDoor(POINT mapChipPos) {
	bool ret = false;
	switch (this->mapData[mapChipPos.y][mapChipPos.x]->GetChipType()) {
		case MapCommon::MapChipType::DOOR:
		case MapCommon::MapChipType::GOLD_DOOR:
		{
			MapChipDoor* pMapChipDoor = (MapChipDoor*)this->mapData[mapChipPos.y][mapChipPos.x];
			switch (pMapChipDoor->state) {
				case MapChipDoor::State::CLOSED:
					pMapChipDoor->StartOpeningDoor();
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

MapCommon::MapChipType Map::GetMapChipType(POINT mapChipPos) {
	return this->mapData[mapChipPos.y][mapChipPos.x]->GetChipType();
}

bool Map::IsDoorOpened(POINT mapChipPos) {
	bool ret = false;
	MapChip* pChip = this->mapData[mapChipPos.y][mapChipPos.x];
	switch (pChip->GetChipType()) {
		case MapCommon::MapChipType::DOOR:
		case MapCommon::MapChipType::GOLD_DOOR:
			ret = ((MapChipDoor*)pChip)->state == MapChipDoor::State::OPENED;
			break;
		default:
			break;
	}
	return ret;
}

bool Map::ExistsWallBetween(POINT xy1, POINT xy2) {
	bool ret = false;
	POINT p1 = GetMapChipPos(xy1);
	POINT p2 = GetMapChipPos(xy2);

	if (p1.x != p2.x && p1.y != p2.y) {
		return true;
	}

	POINT checkPos = p1;
	while (p1.x == p2.x ? checkPos.y != p2.y : checkPos.x != p2.x) {
		switch (this->mapData[checkPos.y][checkPos.x]->GetChipType()) {
			case MapCommon::MapChipType::WALL:
			case MapCommon::MapChipType::WALL_SIDE:
			case MapCommon::MapChipType::JEWELRY:
				ret = true;
				break;
			case MapCommon::MapChipType::DOOR:
			case MapCommon::MapChipType::GOLD_DOOR:
				if (!IsDoorOpened(checkPos)) {
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
	this->jewelryMapChip->OpenBox();
}

POINT Map::GetMapChipPos(POINT xy) {
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


/* Private Functions  ------------------------------------------------------------------------------- */
void Map::SetChipXY() {
	POINT point;
	for (int i = 0; i < this->yChipCount; ++i) {
		for (int j = 0; j < this->xChipCount; ++j) {
			point.x = this->topLeft.x + (j * MapChip::WIDTH);
			point.y = this->topLeft.y + (i * MapChip::HEIGHT);
			this->mapData[i][j]->SetXY(point);
		}
	}
}

bool Map::IsOnRoad(POINT mapChipPos) {
	if (mapChipPos.x < 0 || mapChipPos.x >= xChipCount || mapChipPos.y < 0 || mapChipPos.y >= yChipCount) {
		return false;
	}
	switch (this->mapData[mapChipPos.y][mapChipPos.x]->GetChipType()) {
		case MapCommon::MapChipType::WALL:
		case MapCommon::MapChipType::WALL_SIDE:
		case MapCommon::MapChipType::JEWELRY:
			return false;
		case MapCommon::MapChipType::DOOR:
		case MapCommon::MapChipType::GOLD_DOOR:
			if (((MapChipDoor*)this->mapData[mapChipPos.y][mapChipPos.x])->state != MapChipDoor::OPENED) {
				return false;
			}
			break;
	}
	return true;
}
}
}