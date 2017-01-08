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
Enemy::Enemy(const std::vector<EnemyInfo>& rEnemiesInfo, const std::vector<POINT>& rTopLeftPoints, int scoutableRadius) : scoutableRadius(scoutableRadius) {	
	for (int i = 0; i < (int)rEnemiesInfo.size(); ++i) {
		this->enemiesInfo.push_back(rEnemiesInfo[i]);
		this->enemiesInfo.back().topLeftPoint = rTopLeftPoints[i];
		this->enemiesInfo.back().defaultTopLeftPoint = rTopLeftPoints[i];
	}

	CharacterCommon::CreateTexRect(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_TOP, Enemy::COL_NUM_OF_HEADING_TOP, this->texRectOfHeadingTopChips);
	CharacterCommon::CreateTexRect(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_RIGHT, Enemy::COL_NUM_OF_HEADING_RIGHT, this->texRectOfHeadingRightChips);
	CharacterCommon::CreateTexRect(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_BOTTOM, Enemy::COL_NUM_OF_HEADING_BOTTOM, this->texRectOfHeadingBottomChips);
	CharacterCommon::CreateTexRect(Enemy::CHIP_COUNT_PER_DIRECTION, Enemy::ROW_NUM_OF_HEADING_LEFT, Enemy::COL_NUM_OF_HEADING_LEFT, this->texRectOfHeadingLeftChips);

	Map::MapChip::ConvertChipNumberToTexRect(Enemy::MAP_CHIP_NUMBER_OF_EXCL, &this->texRectOfExclamationMarkChip);
}


/* Getters / Setters -------------------------------------------------------------------------------- */
int Enemy::GetEnermyCount() const {
	return this->enemiesInfo.size();
}

AppCommon::Direction Enemy::GetHeadingDirection(int enemyIdx) const {
	return this->enemiesInfo[enemyIdx].headingDirection;
}

Enemy::State Enemy::GetState(int enemyIdx) const {
	return this->enemiesInfo[enemyIdx].state;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Enemy::ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const {
	pRet->push_back(Drawing::TextureType::CHARACTER);
	pRet->push_back(Drawing::TextureType::MAP);
}

void Enemy::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const {
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); ++enemyIdx) {
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::CHARACTER;
		CreateDrawingVertexRect(enemyIdx, &context.rect);

		if (this->enemiesInfo[enemyIdx].state == State::GOT_STOLEN) {
			// 点滅
			double rad = this->enemiesInfo[enemyIdx].restTimeForBackingToNormal * 18 * M_PI / 180;
			context.alpha = 0xFF * (UINT16)abs(sin(rad));

			pRet->push_back(context);
		} else {
			pRet->push_back(context);

			if (this->enemiesInfo[enemyIdx].state == State::FOUND_PLAYER || this->enemiesInfo[enemyIdx].state == State::ATTACKING) {
				// びっくりマーク表示
				POINT enemyTopLeft = this->enemiesInfo[enemyIdx].topLeftPoint;
				POINT exclTopLeft;
				if (enemyTopLeft.y >= Map::MapChip::HEIGHT) {
					// 上が空いている場合は上に表示
					exclTopLeft.x = enemyTopLeft.x;
					exclTopLeft.y = enemyTopLeft.y - Map::MapChip::HEIGHT;
				} else if (enemyTopLeft.x >= Map::MapChip::WIDTH) {
					// 左が空いている場合は左に表示
					exclTopLeft.x = enemyTopLeft.x - Map::MapChip::WIDTH;
					exclTopLeft.y = enemyTopLeft.y;
				} else {
					// 敵が左上にいる場合は右に表示
					exclTopLeft.x = enemyTopLeft.x + Map::MapChip::WIDTH;
					exclTopLeft.y = enemyTopLeft.y;
				}
				Drawing::DrawingContext context;
				context.textureType = Drawing::TextureType::MAP;
				CharacterCommon::CreateDrawingVertexRect(exclTopLeft, &Map::MapChip::ConvertTopLeftPointToRect, this->texRectOfExclamationMarkChip, &context.rect);
				pRet->push_back(context);
			}
		}
	}
}

void Enemy::Stay() {
	for (auto& enemy : this->enemiesInfo) {
		CharacterCommon::CountUpAnimationCnt(&enemy.currentAnimationCnt, Enemy::CHIP_COUNT_PER_DIRECTION);
	}
}

void Enemy::Move(const POINT& rPixel) {
	for (auto& enemy : this->enemiesInfo) {
		enemy.topLeftPoint.x += rPixel.x;
		enemy.topLeftPoint.y += rPixel.y;
	}
}

void Enemy::CalcEnemyRect(int enemyIdx, Rectangle<POINT>* pRet) const {
	CharacterCommon::CalcCharacterRect(this->enemiesInfo[enemyIdx].topLeftPoint, Enemy::ENEMY_WIDTH, Enemy::ENEMY_HEIGHT, pRet);
}

void Enemy::CalcCenter(int enemyIdx, POINT* pRet) const {
	Rectangle<POINT> enemyRect;
	CalcEnemyRect(enemyIdx, &enemyRect);
	CharacterCommon::CalcCenter(enemyRect, pRet);
}

void Enemy::ScoutStone(const std::vector<Rectangle<POINT>>& rStonesRect) {
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); ++enemyIdx) {
		if (this->enemiesInfo[enemyIdx].state == Enemy::State::ATTACKING || this->enemiesInfo[enemyIdx].state == Enemy::State::GOT_STOLEN) {
			return;
		}

		bool hasFound = false;
		POINT enemyCenter;
		CalcCenter(enemyIdx, &enemyCenter);

		for (int stoneIdx = 0; stoneIdx < (int)rStonesRect.size(); ++stoneIdx) {
			POINT stoneCenter;
			CharacterCommon::CalcCenter(rStonesRect[stoneIdx], &stoneCenter);
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

void Enemy::ScoutPlayer(const POINT& rPlayerCenter, bool isPlayerWalking) {
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); ++enemyIdx) {
		POINT enemyCenter;
		CalcCenter(enemyIdx, &enemyCenter);
		double distance = CharacterCommon::CalcDistance(rPlayerCenter, enemyCenter);

		if (distance <= this->scoutableRadius && !isPlayerWalking) {
			this->enemiesInfo[enemyIdx].state = State::FOUND_PLAYER;
			this->enemiesInfo[enemyIdx].restTimeForCancelFinding = TIME_FOR_CANCELING_FINDING;

			// プレイヤーの方を向く
			TurnTo(rPlayerCenter, enemyIdx);
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

AppCommon::GateKeyType Enemy::GetStolen(const Rectangle<POINT>& rPlayerRect, bool isPlayerStealing) {
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
			Rectangle<POINT> enemyRect;
			CalcEnemyRect(enemyIdx, &enemyRect);
			if (CharacterCommon::IsOverlapping(enemyRect, rPlayerRect)) {
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
				pEnemyInfo->topLeftPoint.y -= Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::RIGHT:
				pEnemyInfo->topLeftPoint.x += Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::BOTTOM:
				pEnemyInfo->topLeftPoint.y += Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
			case AppCommon::Direction::LEFT:
				pEnemyInfo->topLeftPoint.x -= Enemy::MOVING_PIXEL_ON_ATTACKING;
				break;
		}
	}
}

bool Enemy::CanKillPlayer(const Rectangle<POINT>& rPlayerRect) const {
	bool ret = false;
	for (int enemyIdx = 0; enemyIdx < (int)this->enemiesInfo.size(); enemyIdx++) {
		if (this->enemiesInfo[enemyIdx].state == Enemy::State::GOT_STOLEN) {
			continue;
		}

		Rectangle<POINT> enemyRect;
		CalcEnemyRect(enemyIdx, &enemyRect);

		ret = CharacterCommon::IsOverlapping(enemyRect, rPlayerRect);
		if (ret) {
			break;
		}
	}
	return ret;
}

void Enemy::BackToDefaultPosition() {
	for (auto& enemy : this->enemiesInfo) {
		enemy.topLeftPoint = enemy.defaultTopLeftPoint;
		enemy.headingDirection = enemy.defaultDirection;
		enemy.state = Enemy::State::NORMAL;
	}
}


/* Private Functions  ------------------------------------------------------------------------------- */
void Enemy::CreateDrawingVertexRect(int enemyIdx, Rectangle<Drawing::DrawingVertex>* pRet) const {
	Rectangle<FloatPoint> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->enemiesInfo[enemyIdx].currentAnimationCnt);
	switch (this->enemiesInfo[enemyIdx].headingDirection) {
		case AppCommon::Direction::TOP:
			chip = this->texRectOfHeadingTopChips[animationNum];
			break;
		case AppCommon::Direction::RIGHT:
			chip = this->texRectOfHeadingRightChips[animationNum];
			break;
		case AppCommon::Direction::BOTTOM:
			chip = this->texRectOfHeadingBottomChips[animationNum];
			break;
		case AppCommon::Direction::LEFT:
			chip = this->texRectOfHeadingLeftChips[animationNum];
			break;
	}

	CharacterCommon::CreateDrawingVertexRect(this->enemiesInfo[enemyIdx].topLeftPoint, &CharacterCommon::ConvertTopLeftPointToRect, chip, pRet);
}

void Enemy::TurnTo(const POINT& rTargetPoint, int enemyIdx) {
	POINT enemyCenter;
	CalcCenter(enemyIdx, &enemyCenter);

	POINT diff;
	diff.x = enemyCenter.x - rTargetPoint.x;
	diff.y = enemyCenter.y - rTargetPoint.y;
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