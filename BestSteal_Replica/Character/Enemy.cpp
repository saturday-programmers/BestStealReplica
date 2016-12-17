#define _USE_MATH_DEFINES
#include <math.h>

#include "Enemy.h"
#include "../Drawing/Drawer.h"
#include "../Map/MapChip.h"


namespace BestStealReplica {
namespace Character {

/* Structs ------------------------------------------------------------------------------------------ */
Enemy::EnemyInfo::EnemyInfo(int chipPosX, int chipPosY, AppCommon::Direction defaultDirection, AppCommon::GateKeyType holdingGateKey) :
	defaultDirection(defaultDirection),
	headingDirection(defaultDirection),
	holdingGateKey(holdingGateKey),
	state(Enemy::State::NORMAL),
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
		this->enemiesInfo.back().topLeftXY = topLeftXYs[i];
		this->enemiesInfo.back().defaultTopLeftXY = topLeftXYs[i];
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

Vertices<POINT> Enemy::GetEnemyXY(int enemyIdx) const {
	Vertices<POINT> ret = CharacterCommon::GetChipXY(this->enemiesInfo[enemyIdx].topLeftXY);
	int xDiff = (CharacterCommon::WIDTH - Enemy::ENEMY_WIDTH) / 2;
	int yDiff = (CharacterCommon::HEIGHT - Enemy::ENEMY_HEIGHT) / 2;
	ret.topLeft.x += xDiff;
	ret.topLeft.y += yDiff;
	ret.bottomRight.x -= xDiff;
	ret.bottomRight.y -= yDiff;
	return ret;
}

AppCommon::Direction Enemy::GetHeadingDirection(int enemyIdx) const {
	return this->enemiesInfo[enemyIdx].headingDirection;
}

Enemy::State Enemy::GetState(int enemyIdx) const {
	return this->enemiesInfo[enemyIdx].state;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Enemy::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const {
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); ++enemyIdx) {
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::CHARACTER;

		if (this->enemiesInfo[enemyIdx].state == State::GOT_STOLEN) {
			// 点滅
			double rad = this->enemiesInfo[enemyIdx].restTimeForBackingToNormal * 18 * M_PI / 180;
			context.alpha = 0xFF * (UINT16)abs(sin(rad));

			context.vertices = CreateVertex(enemyIdx);
			pDrawingContexts->push_back(context);
		} else {
			context.vertices = CreateVertex(enemyIdx);
			pDrawingContexts->push_back(context);

			if (this->enemiesInfo[enemyIdx].state == State::FOUND_PLAYER || this->enemiesInfo[enemyIdx].state == State::ATTACKING) {
				// びっくりマーク表示
				POINT enemyTopLeftXY = this->enemiesInfo[enemyIdx].topLeftXY;
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
	for (auto& enemy : this->enemiesInfo) {
		CharacterCommon::CountUpAnimationCnt(&enemy.currentAnimationCnt, Enemy::CHIP_COUNT_PER_DIRECTION);
	}
}

void Enemy::Move(POINT xy) {
	for (auto& enemy : this->enemiesInfo) {
		enemy.topLeftXY.x += xy.x;
		enemy.topLeftXY.y += xy.y;
	}
}

void Enemy::ScoutStone(const std::vector<Vertices<POINT>>& rStoneXYs) {
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); ++enemyIdx) {
		if (this->enemiesInfo[enemyIdx].state == Enemy::State::ATTACKING || this->enemiesInfo[enemyIdx].state == Enemy::State::GOT_STOLEN) {
			return;
		}

		bool hasFound = false;
		POINT enemyCenter = CharacterCommon::CalcCenter(GetEnemyXY(enemyIdx));

		for (int stoneIdx = 0; stoneIdx < (int)rStoneXYs.size(); ++stoneIdx) {
			POINT stoneCenter = CharacterCommon::CalcCenter(rStoneXYs.at(stoneIdx));
			double distance = CharacterCommon::CalcDistance(enemyCenter, stoneCenter);

			if (distance <= this->scoutableRadius) {
				hasFound = true;
				this->enemiesInfo[enemyIdx].state = State::FOUND_STONE;

				// 石の方を向く
				TurnTo(stoneCenter, enemyIdx);
				break;
			}
		}

		if (!hasFound && this->enemiesInfo[enemyIdx].state == State::FOUND_STONE) {
			// 索敵範囲内の石がなくなった場合
			this->enemiesInfo[enemyIdx].state = State::NORMAL;
			this->enemiesInfo[enemyIdx].headingDirection = this->enemiesInfo[enemyIdx].defaultDirection;
		} 
	}
}

void Enemy::ScoutPlayer(Vertices<POINT> playerXY, bool isPlayerWalking) {
	POINT playerCenter = CharacterCommon::CalcCenter(playerXY);

	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); ++enemyIdx) {
		POINT enemyCenter = CharacterCommon::CalcCenter(GetEnemyXY(enemyIdx));
		double distance = CharacterCommon::CalcDistance(playerCenter, enemyCenter);

		if (distance <= this->scoutableRadius && !isPlayerWalking) {
			this->enemiesInfo[enemyIdx].state = State::FOUND_PLAYER;
			this->enemiesInfo[enemyIdx].restTimeForCancelFinding = TIME_FOR_CANCELING_FINDING;

			// プレイヤーの方を向く
			TurnTo(playerCenter, enemyIdx);
		} else {
			switch (this->enemiesInfo[enemyIdx].state) {
				case State::FOUND_PLAYER:
					--this->enemiesInfo[enemyIdx].restTimeForCancelFinding;
					if (this->enemiesInfo[enemyIdx].restTimeForCancelFinding == 0) {
						// プレイヤー発見状態解除
						this->enemiesInfo[enemyIdx].state = State::LOST_PLAYER;
						this->enemiesInfo[enemyIdx].restTimeForBackingToNormal = TIME_FOR_BACKING_TO_NORMAL;
					}
					break;
				case State::LOST_PLAYER:
					--this->enemiesInfo[enemyIdx].restTimeForBackingToNormal;
					if (this->enemiesInfo[enemyIdx].restTimeForBackingToNormal == 0) {
						this->enemiesInfo[enemyIdx].state = State::NORMAL;
						// 元の向きに戻る
						this->enemiesInfo[enemyIdx].headingDirection = this->enemiesInfo[enemyIdx].defaultDirection;
					}
					break;
				default:
					break;
			}
		}
	}
}

AppCommon::GateKeyType Enemy::GetStolen(Vertices<POINT> playerXY, bool isPlayerStealing) {
	AppCommon::GateKeyType ret = AppCommon::GateKeyType::None;
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); ++enemyIdx) {
		if (this->enemiesInfo[enemyIdx].state == State::GOT_STOLEN) {
			--this->enemiesInfo[enemyIdx].restTimeForBackingToNormal;
			if (this->enemiesInfo[enemyIdx].restTimeForBackingToNormal == 0) {
				this->enemiesInfo[enemyIdx].state = State::NORMAL;
				// 向きが変わっている間に盗まれた場合はこのタイミングで元の向きに戻る
				this->enemiesInfo[enemyIdx].headingDirection = this->enemiesInfo[enemyIdx].defaultDirection;
			}
			continue;
		}

		if (isPlayerStealing) {
			Vertices<POINT> enemyXY = GetEnemyXY(enemyIdx);
			if (((playerXY.topLeft.x < enemyXY.bottomRight.x && playerXY.bottomRight.x > enemyXY.topLeft.x)
				|| (enemyXY.topLeft.x < playerXY.bottomRight.x && enemyXY.bottomRight.x > playerXY.topLeft.x))
				&& ((playerXY.topLeft.y < enemyXY.bottomRight.y && playerXY.bottomRight.y > enemyXY.topLeft.y)
				|| (enemyXY.topLeft.y < playerXY.bottomRight.y && enemyXY.bottomRight.y > playerXY.topLeft.y))) {

				// 盗み成功
				this->enemiesInfo[enemyIdx].state = State::GOT_STOLEN;
				this->enemiesInfo[enemyIdx].restTimeForBackingToNormal = Enemy::TIME_FOR_BACKING_TO_NORMAL;
				ret = this->enemiesInfo[enemyIdx].holdingGateKey;
				this->enemiesInfo[enemyIdx].holdingGateKey = AppCommon::GateKeyType::None;
				break;
			}
		}
	}
	return ret;
}

void Enemy::Attack(int enemyIdx, bool canSeePlayer) {
	EnemyInfo* pEnemyInfo = &this->enemiesInfo[enemyIdx];
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
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); enemyIdx++) {
		if (this->enemiesInfo[enemyIdx].state == Enemy::State::GOT_STOLEN) {
			continue;
		}

		Vertices<POINT> enemyXY = GetEnemyXY(enemyIdx);
		if (enemyXY.topLeft.x <= playerXY.bottomRight.x && playerXY.topLeft.x <= enemyXY.bottomRight.x
				&& enemyXY.topLeft.y <= playerXY.bottomRight.y && playerXY.topLeft.y <= enemyXY.bottomRight.y) {
			ret = true;
			break;
		}
	}
	return ret;
}

void Enemy:: BackToDefaultPosition() {
	for (auto& enemy : this->enemiesInfo) {
		enemy.topLeftXY = enemy.defaultTopLeftXY;
		enemy.headingDirection = enemy.defaultDirection;
		enemy.state = Enemy::State::NORMAL;
	}
}


/* Private Functions  ------------------------------------------------------------------------------- */
Vertices<Drawing::DrawingVertex> Enemy::CreateVertex(int enemyIdx) const {
	Vertices<Drawing::DrawingVertex> ret;

	Vertices<FloatPoint> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->enemiesInfo[enemyIdx].currentAnimationCnt);
	switch (this->enemiesInfo[enemyIdx].headingDirection) {
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

	ret = CharacterCommon::CreateVertex(this->enemiesInfo[enemyIdx].topLeftXY, &CharacterCommon::GetChipXY, chip);
	return ret;
}

void Enemy::TurnTo(POINT targetXY, int enemyIdx) {
	POINT enemyCenter = CharacterCommon::CalcCenter(GetEnemyXY(enemyIdx));
	POINT diff;
	diff.x = enemyCenter.x - targetXY.x;
	diff.y = enemyCenter.y - targetXY.y;
	if (fabs((double)diff.x) > fabs((double)diff.y)) {
		if (diff.x > 0) {
			this->enemiesInfo[enemyIdx].headingDirection = AppCommon::Direction::LEFT;
		} else {
			this->enemiesInfo[enemyIdx].headingDirection = AppCommon::Direction::RIGHT;
		}
	} else {
		if (diff.y > 0) {
			this->enemiesInfo[enemyIdx].headingDirection = AppCommon::Direction::TOP;
		} else {
			this->enemiesInfo[enemyIdx].headingDirection = AppCommon::Direction::BOTTOM;
		}
	}
}

}
}