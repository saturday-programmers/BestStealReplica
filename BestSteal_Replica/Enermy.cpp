#define _USE_MATH_DEFINES
#include <math.h>

#include "Enermy.h"
#include "Drawer.h"
#include "MapChipSetting.h"


namespace BestStealReplica {
namespace Character {

Enermy::EnermyInfo::EnermyInfo() {}

Enermy::EnermyInfo::EnermyInfo(int chipPosX, int chipPosY, CharacterCommon::Direction defaultDirection, bool hasKey) :
	defaultDirection(defaultDirection),
	headingDirection(defaultDirection),
	hasKey(hasKey),
	state(State::NORMAL),
	currentAnimationCnt(0),
	restTimeForCancelFinding(0),
	restTimeForBackingToNormal(0)
{
	this->chipPos.x = chipPosX;
	this->chipPos.y = chipPosY;
}

Enermy::Enermy(const POINT topLeftXY[], EnermyInfo enermiesInfo[], int enermyCount, Drawer* pDrawer) : 
	enermyCount(enermyCount),
	pDrawer(pDrawer)
{	
	for (int i = 0; i < enermyCount; ++i) {
		this->enermiesInfo[i] = enermiesInfo[i];
		this->enermiesInfo[i].topLeftXY = topLeftXY[i];
	}

	CharacterCommon::SetTuTvs(this->headingTopChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_TOP, Enermy::COL_NUM_OF_HEADING_TOP);
	CharacterCommon::SetTuTvs(this->headingRightChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_RIGHT, Enermy::COL_NUM_OF_HEADING_RIGHT);
	CharacterCommon::SetTuTvs(this->headingBottomChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_BOTTOM, Enermy::COL_NUM_OF_HEADING_BOTTOM);
	CharacterCommon::SetTuTvs(this->headingLeftChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_LEFT, Enermy::COL_NUM_OF_HEADING_LEFT);

	this->exclamationMarkChip = BestStealReplica::Map::MapChipSetting::GetTuTvs(MAP_CHIP_NUMBER_OF_EXCL);
}

void Enermy::Draw() {
	for (int i = 0; i < this->enermyCount; ++i) {
		if (this->enermiesInfo[i].state == State::GOT_STOLEN) {
			// 点滅
			double rad = this->enermiesInfo[i].restTimeForBackingToNormal * 18 * M_PI / 180;
			double alpha = 0xFF * abs(sin(rad));
			this->pDrawer->Draw(GetVertex(i), Drawer::TextureType::CHARACTER, (UINT16)alpha);
		} else {
			this->pDrawer->Draw(GetVertex(i), Drawer::TextureType::CHARACTER);

			if (this->enermiesInfo[i].state == State::FOUND_PLAYER) {
				// びっくりマーク表示
				POINT enermyTopLeftXY = this->enermiesInfo[i].topLeftXY;
				POINT exclXY;
				if (enermyTopLeftXY.y >= Map::MapChipSetting::HEIGHT) {
					// 上が空いている場合は上に表示
					exclXY.x = enermyTopLeftXY.x;
					exclXY.y = enermyTopLeftXY.y - Map::MapChipSetting::HEIGHT;
				} else if (enermyTopLeftXY.x >= Map::MapChipSetting::WIDTH) {
					// 左が空いている場合は左に表示
					exclXY.x = enermyTopLeftXY.x - Map::MapChipSetting::WIDTH;
					exclXY.y = enermyTopLeftXY.y;
				} else {
					// 敵が左上にいる場合は右に表示
					exclXY.x = enermyTopLeftXY.x + Map::MapChipSetting::WIDTH;
					exclXY.y = enermyTopLeftXY.y;
				}
				Vertices<DrawingVertex> exclVertex = CharacterCommon::GetVertex(exclXY, &Map::MapChipSetting::GetXY, this->exclamationMarkChip);
				this->pDrawer->Draw(exclVertex, Drawer::TextureType::MAP);
			}
		}
	}
}

void Enermy::Stay() {
	for (int i = 0; i < this->enermyCount; ++i) {
		CharacterCommon::CountUpAnimationCnt(&this->enermiesInfo[i].currentAnimationCnt, Enermy::CHIP_COUNT_PER_DIRECTION);
	}
}

void Enermy::Move(POINT xy) {
	for (int i = 0; i < this->enermyCount; ++i) {
		this->enermiesInfo[i].topLeftXY.x += xy.x;
		this->enermiesInfo[i].topLeftXY.y += xy.y;
	}
}

Vertices<POINT> Enermy::GetEnermyXY(int enermyNum) {
	Vertices<POINT> ret = CharacterCommon::GetChipXY(this->enermiesInfo[enermyNum].topLeftXY);
	int xDiff = (CharacterCommon::WIDTH - Enermy::ENERMY_WIDTH) / 2;
	int yDiff = (CharacterCommon::HEIGHT - Enermy::ENERMY_HEIGHT) / 2;
	ret.topLeft.x += xDiff;
	ret.topLeft.y += yDiff;
	ret.bottomRight.x -= xDiff;
	ret.bottomRight.y -= yDiff;
	return ret;
}

void Enermy::ScoutPlayer(Vertices<POINT> playerXY, int scoutableRadius, bool isPlayerWalking) {
	POINT playerCenter;
	playerCenter.x = playerXY.topLeft.x + (playerXY.bottomRight.x - playerXY.topLeft.x) / 2;
	playerCenter.y = playerXY.topLeft.y + (playerXY.bottomRight.y - playerXY.topLeft.y) / 2;

	for (int i = 0; i < this->enermyCount; ++i) {
		Vertices<POINT> enermyXY = GetEnermyXY(i);
		POINT enermyCenter;
		enermyCenter.x = enermyXY.topLeft.x + (enermyXY.bottomRight.x - enermyXY.topLeft.x) / 2;
		enermyCenter.y = enermyXY.topLeft.y + (enermyXY.bottomRight.y - enermyXY.topLeft.y) / 2;
		double distance = sqrt(pow(playerCenter.x - enermyCenter.x, 2.0) + pow(playerCenter.y - enermyCenter.y, 2.0));

		if (distance <= scoutableRadius && !isPlayerWalking) {
			this->enermiesInfo[i].state = State::FOUND_PLAYER;
			this->enermiesInfo[i].restTimeForCancelFinding = TIME_FOR_CANCELING_FINDING;

			// プレイヤーの方を向く
			POINT diff;
			diff.x = enermyCenter.x - playerCenter.x;
			diff.y = enermyCenter.y - playerCenter.y;
			if (fabs((double)diff.x) > fabs((double)diff.y)) {
				if (diff.x > 0) {
					this->enermiesInfo[i].headingDirection = CharacterCommon::Direction::LEFT;
				} else {
					this->enermiesInfo[i].headingDirection = CharacterCommon::Direction::RIGHT;
				}
			} else {
				if (diff.y > 0) {
					this->enermiesInfo[i].headingDirection = CharacterCommon::Direction::TOP;
				} else {
					this->enermiesInfo[i].headingDirection = CharacterCommon::Direction::BOTTOM;
				}
			}
		} else {
			switch (this->enermiesInfo[i].state) {
				case State::FOUND_PLAYER:
					--this->enermiesInfo[i].restTimeForCancelFinding;
					if (this->enermiesInfo[i].restTimeForCancelFinding == 0) {
						// プレイヤー発見状態解除
						this->enermiesInfo[i].state = State::LOST_PLAYER;
						this->enermiesInfo[i].restTimeForBackingToNormal = TIME_FOR_BACKING_TO_NORMAL;
					}
					break;
				case State::LOST_PLAYER:
					--this->enermiesInfo[i].restTimeForBackingToNormal;
					if (this->enermiesInfo[i].restTimeForBackingToNormal == 0) {
						this->enermiesInfo[i].state = State::NORMAL;
						// 元の向きに戻る
						this->enermiesInfo[i].headingDirection = this->enermiesInfo[i].defaultDirection;
					}
					break;
				default:
					break;
			}
		}
	}
}

int Enermy::GetStolen(Vertices<POINT> playerXY, bool isPlayerStealing) {
	int ret = 0;
	for (int i = 0; i < this->enermyCount; ++i) {
		if (this->enermiesInfo[i].state == State::GOT_STOLEN) {
			--this->enermiesInfo[i].restTimeForBackingToNormal;
			if (this->enermiesInfo[i].restTimeForBackingToNormal == 0) {
				this->enermiesInfo[i].state = State::NORMAL;
				// 向きが変わっている間に盗まれた場合はこのタイミングで元の向きに戻る
				this->enermiesInfo[i].headingDirection = this->enermiesInfo[i].defaultDirection;
			}
			continue;
		}

		if (isPlayerStealing) {
			Vertices<POINT> enermyXY = GetEnermyXY(i);
			if (((playerXY.topLeft.x < enermyXY.bottomRight.x && playerXY.bottomRight.x > enermyXY.topLeft.x)
				|| (enermyXY.topLeft.x < playerXY.bottomRight.x && enermyXY.bottomRight.x > playerXY.topLeft.x))
				&& ((playerXY.topLeft.y < enermyXY.bottomRight.y && playerXY.bottomRight.y > enermyXY.topLeft.y)
				|| (enermyXY.topLeft.y < playerXY.bottomRight.y && enermyXY.bottomRight.y > enermyXY.topLeft.y))) {

				// 盗み成功
				this->enermiesInfo[i].state = State::GOT_STOLEN;
				this->enermiesInfo[i].restTimeForBackingToNormal = Enermy::TIME_FOR_BACKING_TO_NORMAL;
				if (this->enermiesInfo[i].hasKey) {
					++ret;
					this->enermiesInfo[i].hasKey = false;
				}
			}
		}
	}
	return ret;
}


Vertices<DrawingVertex> Enermy::GetVertex(int enermyNum) {
	Vertices<DrawingVertex> ret;

	Vertices<FloatPoint> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->enermiesInfo[enermyNum].currentAnimationCnt);
	switch (this->enermiesInfo[enermyNum].headingDirection) {
		case CharacterCommon::Direction::TOP:
			chip = this->headingTopChips[animationNum];
			break;
		case CharacterCommon::Direction::RIGHT:
			chip = this->headingRightChips[animationNum];
			break;
		case CharacterCommon::Direction::BOTTOM:
			chip = this->headingBottomChips[animationNum];
			break;
		case CharacterCommon::Direction::LEFT:
			chip = this->headingLeftChips[animationNum];
			break;
	}

	ret = CharacterCommon::GetVertex(this->enermiesInfo[enermyNum].topLeftXY, &CharacterCommon::GetChipXY, chip);
	return ret;
}

}
}