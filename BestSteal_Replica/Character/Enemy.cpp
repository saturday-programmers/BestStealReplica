#define _USE_MATH_DEFINES
#include <math.h>

#include "Enemy.h"
#include "../Drawing/Drawer.h"
#include "../Map/MapChip.h"


namespace BestStealReplica {
namespace Character {

/* Structs ------------------------------------------------------------------------------------------ */
Enemy::EnemyInfo::EnemyInfo() {}

Enemy::EnemyInfo::EnemyInfo(int chipPosX, int chipPosY, AppCommon::Direction defaultDirection, AppCommon::KeyType holdingKey) :
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
Enemy::Enemy(std::vector<EnemyInfo> enemiesInfo, std::vector<POINT> topLeftXYs, int scoutableRadius) : scoutableRadius(scoutableRadius) {	
	for (int i = 0; i < (int)enemiesInfo.size(); ++i) {
		this->enemiesInfo.push_back(enemiesInfo[i]);
		this->enemiesInfo[i].topLeftXY = topLeftXYs[i];
		this->enemiesInfo[i].defaultTopLeftXY = topLeftXYs[i];
	}

	CharacterCommon::CreateChipTuTvs(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_TOP, Enemy::COL_NUM_OF_HEADING_TOP, this->headingTopChips);
	CharacterCommon::CreateChipTuTvs(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_RIGHT, Enemy::COL_NUM_OF_HEADING_RIGHT, this->headingRightChips);
	CharacterCommon::CreateChipTuTvs(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_BOTTOM, Enemy::COL_NUM_OF_HEADING_BOTTOM, this->headingBottomChips);
	CharacterCommon::CreateChipTuTvs(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_LEFT, Enemy::COL_NUM_OF_HEADING_LEFT, this->headingLeftChips);

	this->exclamationMarkChip = BestStealReplica::Map::MapChip::GetTuTvs(MAP_CHIP_NUMBER_OF_EXCL);
}


/* Getters / Setters -------------------------------------------------------------------------------- */
std::vector<Drawing::TextureType> Enemy::GetTextureTypes() const {
	std::vector<Drawing::TextureType> ret;
	ret.push_back(Drawing::TextureType::CHARACTER);
	ret.push_back(Drawing::TextureType::MAP);
	return ret;
}

int Enemy::GetEnermyCount() const {
	return this->enemiesInfo.size();
}

Vertices<POINT> Enemy::GetEnemyXY(int enemyNum) const {
	Vertices<POINT> ret = CharacterCommon::GetChipXY(this->enemiesInfo[enemyNum].topLeftXY);
	int xDiff = (CharacterCommon::WIDTH - Enemy::ENEMY_WIDTH) / 2;
	int yDiff = (CharacterCommon::HEIGHT - Enemy::ENEMY_HEIGHT) / 2;
	ret.topLeft.x += xDiff;
	ret.topLeft.y += yDiff;
	ret.bottomRight.x -= xDiff;
	ret.bottomRight.y -= yDiff;
	return ret;
}

AppCommon::Direction Enemy::GetHeadingDirection(int enemyNum) const {
	return this->enemiesInfo[enemyNum].headingDirection;
}

Enemy::State Enemy::GetState(int enemyNum) const {
	return this->enemiesInfo[enemyNum].state;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Enemy::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const {
	for (int i = 0; i < (int)this->enemiesInfo.size(); ++i) {
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::CHARACTER;

		if (this->enemiesInfo[i].state == State::GOT_STOLEN) {
			// 点滅
			double rad = this->enemiesInfo[i].restTimeForBackingToNormal * 18 * M_PI / 180;
			context.alpha = 0xFF * (UINT16)abs(sin(rad));

			context.vertices = CreateVertex(i);
			pDrawingContexts->push_back(context);
		} else {
			context.vertices = CreateVertex(i);
			pDrawingContexts->push_back(context);

			if (this->enemiesInfo[i].state == State::FOUND_PLAYER || this->enemiesInfo[i].state == State::ATTACKING) {
				// びっくりマーク表示
				POINT enemyTopLeftXY = this->enemiesInfo[i].topLeftXY;
				POINT exclXY;
				if (enemyTopLeftXY.y >= Map::MapChip::HEIGHT) {
					// 上が空いている場合は上に表示
					exclXY.x = enemyTopLeftXY.x;
					exclXY.y = enemyTopLeftXY.y - Map::MapChip::HEIGHT;
				} else if (enemyTopLeftXY.x >= Map::MapChip::WIDTH) {
					// 左が空いている場合は左に表示
					exclXY.x = enemyTopLeftXY.x - Map::MapChip::WIDTH;
					exclXY.y = enemyTopLeftXY.y;
				} else {
					// 敵が左上にいる場合は右に表示
					exclXY.x = enemyTopLeftXY.x + Map::MapChip::WIDTH;
					exclXY.y = enemyTopLeftXY.y;
				}
				Drawing::DrawingContext context;
				context.textureType = Drawing::TextureType::MAP;
				context.vertices = CharacterCommon::CreateVertex(exclXY, &Map::MapChip::GetXY, this->exclamationMarkChip);
				pDrawingContexts->push_back(context);
			}
		}
	}
}

void Enemy::Stay() {
	for (int i = 0; i < (int)this->enemiesInfo.size(); ++i) {
		CharacterCommon::CountUpAnimationCnt(&this->enemiesInfo[i].currentAnimationCnt, Enemy::CHIP_COUNT_PER_DIRECTION);
	}
}

void Enemy::Move(POINT xy) {
	for (int i = 0; i < (int)this->enemiesInfo.size(); ++i) {
		this->enemiesInfo[i].topLeftXY.x += xy.x;
		this->enemiesInfo[i].topLeftXY.y += xy.y;
	}
}

void Enemy::ScoutStone(const std::vector<Vertices<POINT>>& rStoneXYs) {
	for (int i = 0; i < (int)this->enemiesInfo.size(); ++i) {
		if (this->enemiesInfo[i].state == Enemy::State::ATTACKING || this->enemiesInfo[i].state == Enemy::State::GOT_STOLEN) {
			return;
		}

		bool hasFound = false;
		POINT enemyCenter = CharacterCommon::CalcCenter(GetEnemyXY(i));

		for (int j = 0; j < (int)rStoneXYs.size(); ++j) {
			POINT stoneCenter = CharacterCommon::CalcCenter(rStoneXYs.at(j));
			double distance = CharacterCommon::CalcDistance(enemyCenter, stoneCenter);

			if (distance <= this->scoutableRadius) {
				hasFound = true;
				this->enemiesInfo[i].state = State::FOUND_STONE;

				// 石の方を向く
				TurnTo(stoneCenter, i);
				break;
			}
		}

		if (!hasFound && this->enemiesInfo[i].state == State::FOUND_STONE) {
			// 索敵範囲内の石がなくなった場合
			this->enemiesInfo[i].state = State::NORMAL;
			this->enemiesInfo[i].headingDirection = this->enemiesInfo[i].defaultDirection;
		} 
	}
}

void Enemy::ScoutPlayer(Vertices<POINT> playerXY, bool isPlayerWalking) {
	POINT playerCenter = CharacterCommon::CalcCenter(playerXY);

	for (int i = 0; i < (int)this->enemiesInfo.size(); ++i) {
		POINT enemyCenter = CharacterCommon::CalcCenter(GetEnemyXY(i));
		double distance = CharacterCommon::CalcDistance(playerCenter, enemyCenter);

		if (distance <= this->scoutableRadius && !isPlayerWalking) {
			this->enemiesInfo[i].state = State::FOUND_PLAYER;
			this->enemiesInfo[i].restTimeForCancelFinding = TIME_FOR_CANCELING_FINDING;

			// プレイヤーの方を向く
			TurnTo(playerCenter, i);
		} else {
			switch (this->enemiesInfo[i].state) {
				case State::FOUND_PLAYER:
					--this->enemiesInfo[i].restTimeForCancelFinding;
					if (this->enemiesInfo[i].restTimeForCancelFinding == 0) {
						// プレイヤー発見状態解除
						this->enemiesInfo[i].state = State::LOST_PLAYER;
						this->enemiesInfo[i].restTimeForBackingToNormal = TIME_FOR_BACKING_TO_NORMAL;
					}
					break;
				case State::LOST_PLAYER:
					--this->enemiesInfo[i].restTimeForBackingToNormal;
					if (this->enemiesInfo[i].restTimeForBackingToNormal == 0) {
						this->enemiesInfo[i].state = State::NORMAL;
						// 元の向きに戻る
						this->enemiesInfo[i].headingDirection = this->enemiesInfo[i].defaultDirection;
					}
					break;
				default:
					break;
			}
		}
	}
}

AppCommon::KeyType Enemy::GetStolen(Vertices<POINT> playerXY, bool isPlayerStealing) {
	AppCommon::KeyType ret = AppCommon::KeyType::None;
	for (int i = 0; i < (int)this->enemiesInfo.size(); ++i) {
		if (this->enemiesInfo[i].state == State::GOT_STOLEN) {
			--this->enemiesInfo[i].restTimeForBackingToNormal;
			if (this->enemiesInfo[i].restTimeForBackingToNormal == 0) {
				this->enemiesInfo[i].state = State::NORMAL;
				// 向きが変わっている間に盗まれた場合はこのタイミングで元の向きに戻る
				this->enemiesInfo[i].headingDirection = this->enemiesInfo[i].defaultDirection;
			}
			continue;
		}

		if (isPlayerStealing) {
			Vertices<POINT> enemyXY = GetEnemyXY(i);
			if (((playerXY.topLeft.x < enemyXY.bottomRight.x && playerXY.bottomRight.x > enemyXY.topLeft.x)
				|| (enemyXY.topLeft.x < playerXY.bottomRight.x && enemyXY.bottomRight.x > playerXY.topLeft.x))
				&& ((playerXY.topLeft.y < enemyXY.bottomRight.y && playerXY.bottomRight.y > enemyXY.topLeft.y)
				|| (enemyXY.topLeft.y < playerXY.bottomRight.y && enemyXY.bottomRight.y > playerXY.topLeft.y))) {

				// 盗み成功
				this->enemiesInfo[i].state = State::GOT_STOLEN;
				this->enemiesInfo[i].restTimeForBackingToNormal = Enemy::TIME_FOR_BACKING_TO_NORMAL;
				ret = this->enemiesInfo[i].holdingKey;
				this->enemiesInfo[i].holdingKey = AppCommon::KeyType::None;
				break;
			}
		}
	}
	return ret;
}

void Enemy::Attack(int enemyNum, bool canSeePlayer) {
	EnemyInfo* pEnemyInfo = &this->enemiesInfo[enemyNum];
	if (pEnemyInfo->state == Enemy::State::GOT_STOLEN) {
		// 盗まれ中の場合は動かない
		return;
	}

	// ステート変更
	if (canSeePlayer) {
		pEnemyInfo->state = Enemy::State::ATTACKING;
	} else if (pEnemyInfo->state == Enemy::State::ATTACKING) {
		pEnemyInfo->state = Enemy::State::NORMAL;
	}

	// 突進
	if (pEnemyInfo->state == Enemy::State::ATTACKING) {
		switch (pEnemyInfo->headingDirection) {
			case AppCommon::Direction::TOP:
				pEnemyInfo->topLeftXY.y -= Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::RIGHT:
				pEnemyInfo->topLeftXY.x += Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::BOTTOM:
				pEnemyInfo->topLeftXY.y += Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::LEFT:
				pEnemyInfo->topLeftXY.x -= Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
		}
	}
}

bool Enemy::CanKillPlayer(Vertices<POINT> playerXY) const {
	bool ret = false;
	for (int i = 0; i < (int)this->enemiesInfo.size(); i++) {
		if (this->enemiesInfo[i].state == Enemy::State::GOT_STOLEN) {
			continue;
		}

		Vertices<POINT> enemyXY = GetEnemyXY(i);
		if (enemyXY.topLeft.x <= playerXY.bottomRight.x && playerXY.topLeft.x <= enemyXY.bottomRight.x
				&& enemyXY.topLeft.y <= playerXY.bottomRight.y && playerXY.topLeft.y <= enemyXY.bottomRight.y) {
			ret = true;
			break;
		}
	}
	return ret;
}

void Enemy:: BackToDefaultPosition() {
	for (int i = 0; i < (int)this->enemiesInfo.size(); i++) {
		this->enemiesInfo[i].topLeftXY = this->enemiesInfo[i].defaultTopLeftXY;
		this->enemiesInfo[i].headingDirection = this->enemiesInfo[i].defaultDirection;
		this->enemiesInfo[i].state = Enemy::State::NORMAL;
	}
}


/* Private Functions  ------------------------------------------------------------------------------- */
Vertices<Drawing::DrawingVertex> Enemy::CreateVertex(int enemyNum) const {
	Vertices<Drawing::DrawingVertex> ret;

	Vertices<FloatPoint> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->enemiesInfo[enemyNum].currentAnimationCnt);
	switch (this->enemiesInfo[enemyNum].headingDirection) {
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

	ret = CharacterCommon::CreateVertex(this->enemiesInfo[enemyNum].topLeftXY, &CharacterCommon::GetChipXY, chip);
	return ret;
}

void Enemy::TurnTo(POINT targetXY, int enemyNum) {
	POINT enemyCenter = CharacterCommon::CalcCenter(GetEnemyXY(enemyNum));
	POINT diff;
	diff.x = enemyCenter.x - targetXY.x;
	diff.y = enemyCenter.y - targetXY.y;
	if (fabs((double)diff.x) > fabs((double)diff.y)) {
		if (diff.x > 0) {
			this->enemiesInfo[enemyNum].headingDirection = AppCommon::Direction::LEFT;
		} else {
			this->enemiesInfo[enemyNum].headingDirection = AppCommon::Direction::RIGHT;
		}
	} else {
		if (diff.y > 0) {
			this->enemiesInfo[enemyNum].headingDirection = AppCommon::Direction::TOP;
		} else {
			this->enemiesInfo[enemyNum].headingDirection = AppCommon::Direction::BOTTOM;
		}
	}
}

}
}