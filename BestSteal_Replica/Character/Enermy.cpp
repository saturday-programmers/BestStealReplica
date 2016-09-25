#define _USE_MATH_DEFINES
#include <math.h>

#include "Enermy.h"
#include "Drawer.h"
#include "MapChip.h"


namespace BestStealReplica {
namespace Character {

/* Structs ------------------------------------------------------------------------------------------ */
Enermy::EnermyInfo::EnermyInfo() {}

Enermy::EnermyInfo::EnermyInfo(int chipPosX, int chipPosY, AppCommon::Direction defaultDirection, AppCommon::KeyType holdingKey) :
	defaultDirection(defaultDirection),
	headingDirection(defaultDirection),
	holdingKey(holdingKey),
	state(State::NORMAL),
	currentAnimationCnt(0),
	restTimeForCancelFinding(0),
	restTimeForBackingToNormal(0)
{
	this->chipPos.x = chipPosX;
	this->chipPos.y = chipPosY;
}

/* Constructor / Destructor ------------------------------------------------------------------------- */
Enermy::Enermy(const POINT topLeftXY[], EnermyInfo enermiesInfo[], int enermyCount, Drawer* pDrawer) : 
	enermyCount(enermyCount),
	pDrawer(pDrawer)
{	
	for (int i = 0; i < enermyCount; ++i) {
		this->enermiesInfo[i] = enermiesInfo[i];
		this->enermiesInfo[i].topLeftXY = topLeftXY[i];
		this->enermiesInfo[i].defaultTopLeftXY = topLeftXY[i];
	}

	CharacterCommon::SetTuTvs(this->headingTopChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_TOP, Enermy::COL_NUM_OF_HEADING_TOP);
	CharacterCommon::SetTuTvs(this->headingRightChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_RIGHT, Enermy::COL_NUM_OF_HEADING_RIGHT);
	CharacterCommon::SetTuTvs(this->headingBottomChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_BOTTOM, Enermy::COL_NUM_OF_HEADING_BOTTOM);
	CharacterCommon::SetTuTvs(this->headingLeftChips, Enermy::CHIP_COUNT_PER_DIRECTION, Enermy::ROW_NUM_OF_HEADING_LEFT, Enermy::COL_NUM_OF_HEADING_LEFT);

	this->exclamationMarkChip = BestStealReplica::Map::MapChip::GetTuTvs(MAP_CHIP_NUMBER_OF_EXCL);
}


/* Getters / Setters -------------------------------------------------------------------------------- */
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

AppCommon::Direction Enermy::GetHeadingDirection(int enermyNum) {
	return this->enermiesInfo[enermyNum].headingDirection;
}

Enermy::State Enermy::GetState(int enermyNum) {
	return this->enermiesInfo[enermyNum].state;
}

/* Public Functions  -------------------------------------------------------------------------------- */
void Enermy::Draw() {
	for (int i = 0; i < this->enermyCount; ++i) {
		if (this->enermiesInfo[i].state == State::GOT_STOLEN) {
			// 点滅
			double rad = this->enermiesInfo[i].restTimeForBackingToNormal * 18 * M_PI / 180;
			double alpha = 0xFF * abs(sin(rad));
			this->pDrawer->Draw(GetVertex(i), Drawer::TextureType::CHARACTER, (UINT16)alpha);
		} else {
			this->pDrawer->Draw(GetVertex(i), Drawer::TextureType::CHARACTER);

			if (this->enermiesInfo[i].state == State::FOUND_PLAYER || this->enermiesInfo[i].state == State::ATTACKING) {
				// びっくりマーク表示
				POINT enermyTopLeftXY = this->enermiesInfo[i].topLeftXY;
				POINT exclXY;
				if (enermyTopLeftXY.y >= Map::MapChip::HEIGHT) {
					// 上が空いている場合は上に表示
					exclXY.x = enermyTopLeftXY.x;
					exclXY.y = enermyTopLeftXY.y - Map::MapChip::HEIGHT;
				} else if (enermyTopLeftXY.x >= Map::MapChip::WIDTH) {
					// 左が空いている場合は左に表示
					exclXY.x = enermyTopLeftXY.x - Map::MapChip::WIDTH;
					exclXY.y = enermyTopLeftXY.y;
				} else {
					// 敵が左上にいる場合は右に表示
					exclXY.x = enermyTopLeftXY.x + Map::MapChip::WIDTH;
					exclXY.y = enermyTopLeftXY.y;
				}
				Vertices<DrawingVertex> exclVertex = CharacterCommon::GetVertex(exclXY, &Map::MapChip::GetXY, this->exclamationMarkChip);
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

void Enermy::ScoutPlayer(Vertices<POINT> playerXY, int scoutableRadius, bool isPlayerWalking) {
	POINT playerCenter = CharacterCommon::CalcCenter(playerXY);

	for (int i = 0; i < this->enermyCount; ++i) {
		POINT enermyCenter = CharacterCommon::CalcCenter(GetEnermyXY(i));
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
					this->enermiesInfo[i].headingDirection = AppCommon::Direction::LEFT;
				} else {
					this->enermiesInfo[i].headingDirection = AppCommon::Direction::RIGHT;
				}
			} else {
				if (diff.y > 0) {
					this->enermiesInfo[i].headingDirection = AppCommon::Direction::TOP;
				} else {
					this->enermiesInfo[i].headingDirection = AppCommon::Direction::BOTTOM;
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

AppCommon::KeyType Enermy::GetStolen(Vertices<POINT> playerXY, bool isPlayerStealing) {
	AppCommon::KeyType ret = AppCommon::KeyType::None;
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
				|| (enermyXY.topLeft.y < playerXY.bottomRight.y && enermyXY.bottomRight.y > playerXY.topLeft.y))) {

				// 盗み成功
				this->enermiesInfo[i].state = State::GOT_STOLEN;
				this->enermiesInfo[i].restTimeForBackingToNormal = Enermy::TIME_FOR_BACKING_TO_NORMAL;
				ret = this->enermiesInfo[i].holdingKey;
				this->enermiesInfo[i].holdingKey = AppCommon::KeyType::None;
				break;
			}
		}
	}
	return ret;
}

void Enermy::Attack(int enermyNum, bool canSeePlayer) {
	EnermyInfo* pEnermyInfo = &(this->enermiesInfo[enermyNum]);
	if (pEnermyInfo->state == Enermy::State::GOT_STOLEN) {
		// 盗まれ中の場合は動かない
		return;
	}

	// ステート変更
	if (canSeePlayer) {
		pEnermyInfo->state = Enermy::State::ATTACKING;
	} else if (pEnermyInfo->state == Enermy::State::ATTACKING) {
		pEnermyInfo->state = Enermy::State::NORMAL;
	}

	// 突進
	if (pEnermyInfo->state == Enermy::State::ATTACKING) {
		switch (pEnermyInfo->headingDirection) {
			case AppCommon::Direction::TOP:
				pEnermyInfo->topLeftXY.y -= Enermy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::RIGHT:
				pEnermyInfo->topLeftXY.x += Enermy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::BOTTOM:
				pEnermyInfo->topLeftXY.y += Enermy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::LEFT:
				pEnermyInfo->topLeftXY.x -= Enermy::MOVING_PIXEL_ON_ATTACKING;
				break;
		}
	}
}

bool Enermy::CanKillPlayer(Vertices<POINT> playerXY) {
	bool ret = false;
	for (int i = 0; i < this->enermyCount; i++) {
		if (this->enermiesInfo[i].state == Enermy::State::GOT_STOLEN) {
			continue;
		}

		Vertices<POINT> enermyXY = GetEnermyXY(i);
		if (enermyXY.topLeft.x <= playerXY.bottomRight.x && playerXY.topLeft.x <= enermyXY.bottomRight.x
				&& enermyXY.topLeft.y <= playerXY.bottomRight.y && playerXY.topLeft.y <= enermyXY.bottomRight.y) {
			ret = true;
			break;
		}
	}
	return ret;
}

void Enermy:: BackToDefaultPosition() {
	for (int i = 0; i < this->enermyCount; i++) {
		this->enermiesInfo[i].topLeftXY = this->enermiesInfo[i].defaultTopLeftXY;
		this->enermiesInfo[i].headingDirection = this->enermiesInfo[i].defaultDirection;
		this->enermiesInfo[i].state = Enermy::State::NORMAL;
	}
}


/* Private Functions  ------------------------------------------------------------------------------- */
Vertices<DrawingVertex> Enermy::GetVertex(int enermyNum) {
	Vertices<DrawingVertex> ret;

	Vertices<FloatPoint> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->enermiesInfo[enermyNum].currentAnimationCnt);
	switch (this->enermiesInfo[enermyNum].headingDirection) {
		case AppCommon::Direction::TOP:
			chip = this->headingTopChips[animationNum];
			break;
		case AppCommon::Direction::RIGHT:
			chip = this->headingRightChips[animationNum];
			break;
		case AppCommon::Direction::BOTTOM:
			chip = this->headingBottomChips[animationNum];
			break;
		case AppCommon::Direction::LEFT:
			chip = this->headingLeftChips[animationNum];
			break;
	}

	ret = CharacterCommon::GetVertex(this->enermiesInfo[enermyNum].topLeftXY, &CharacterCommon::GetChipXY, chip);
	return ret;
}

}
}