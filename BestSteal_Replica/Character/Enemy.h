#ifndef ENEMY_H_
#define ENEMY_H_

#include <vector>
#include <windows.h>

#include "../Character/CharacterCommon.h"
#include "../Drawing/IDrawable.h" 


namespace BestStealReplica {
class Drawer;

namespace Character {

class Enemy : public Drawing::IDrawable {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum State {
		NORMAL,
		FOUND_STONE,
		FOUND_PLAYER,
		LOST_PLAYER,
		GOT_STOLEN,
		ATTACKING
	};


	/* Structs ------------------------------------------------------------------------------------------ */
	struct EnemyInfo {
		POINT chipPos;
		POINT topLeftXY;
		POINT defaultTopLeftXY;
		AppCommon::Direction defaultDirection;
		AppCommon::Direction headingDirection;
		AppCommon::GateKeyType holdingGateKey;
		State state;
		int currentAnimationCnt;
		int restTimeForCancelFinding;
		int restTimeForBackingToNormal;

		EnemyInfo();
		EnemyInfo(int chipPosX, int chipPosY, AppCommon::Direction defaultDirection, AppCommon::GateKeyType holdingGateKey);
	};


	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Enemy(std::vector<EnemyInfo> enemiesInfo, std::vector<POINT> topLeftXYs, int scoutableRadius);


	/* Getters / Setters -------------------------------------------------------------------------------- */
	std::vector<Drawing::TextureType> GetTextureTypes() const;

	int GetEnermyCount() const;
	Vertices<POINT> GetEnemyXY(int enemyIdx) const;
	AppCommon::Direction GetHeadingDirection(int enemyIdx) const;
	Enemy::State GetState(int enemyIdx) const;


	/* Functions ---------------------------------------------------------------------------------------- */
	void CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const;

	void Stay();
	void Move(POINT xy);
	void ScoutStone(const std::vector<Vertices<POINT>>& rStonesXY);
	void ScoutPlayer(Vertices<POINT> playerXY, bool isPlayerWalking);
	AppCommon::GateKeyType GetStolen(Vertices<POINT> playerXY, bool isPlayerStealing);
	void Attack(int enemyIdx, bool canSeePlayer);
	bool CanKillPlayer(Vertices<POINT> playerXY) const;
	void BackToDefaultPosition();


private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int CHIP_COUNT_PER_DIRECTION = 3;
	static const int ENEMY_HEIGHT = 60;
	static const int ENEMY_WIDTH = 50;
	static const int TIME_FOR_CANCELING_FINDING = 120;
	static const int TIME_FOR_BACKING_TO_NORMAL = 60;
	static const int MOVING_PIXEL_ON_ATTACKING = 20;

	static const int ROW_NUM_OF_HEADING_BOTTOM = 4;
	static const int COL_NUM_OF_HEADING_BOTTOM = 0;
	static const int ROW_NUM_OF_HEADING_TOP = 4;
	static const int COL_NUM_OF_HEADING_TOP = 3;
	static const int ROW_NUM_OF_HEADING_LEFT = 5;
	static const int COL_NUM_OF_HEADING_LEFT = 0;
	static const int ROW_NUM_OF_HEADING_RIGHT = 5;
	static const int COL_NUM_OF_HEADING_RIGHT = 3;
	static const int MAP_CHIP_NUMBER_OF_EXCL = 66;


	/* Variables ---------------------------------------------------------------------------------------- */
	Vertices<FloatPoint> headingBottomChips[Enemy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingTopChips[Enemy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingLeftChips[Enemy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingRightChips[Enemy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> exclamationMarkChip;

	std::vector<EnemyInfo> enemiesInfo;
	int scoutableRadius;


	/* Functions ---------------------------------------------------------------------------------------- */
	Vertices<Drawing::DrawingVertex> CreateVertex(int enemyIdx) const;
	void TurnTo(POINT targetXY, int enemyIdx);
};

}
}

#endif