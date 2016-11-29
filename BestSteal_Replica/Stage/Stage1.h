#ifndef STAGE1_H_
#define STAGE1_H_

#include "IStage.h"

namespace BestStealReplica {
namespace Stage {

class Stage1 : public IStage {

public:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int Y_CHIP_COUNT = 21;
	static const int X_CHIP_COUNT = 30;
	static const int ENEMY_COUNT = 7;
	static const int MAX_STONE_COUNT = 3;

	/* Getters / Setters -------------------------------------------------------------------------------- */
	int GetYChipCount() const;
	int GetXChipCount() const;
	Map::MapCommon::MapChipType GetMapChipType(int y, int x) const;
	POINT GetPlayerFirstChipPos() const;
	int GetEnemyCount() const;
	POINT GetEnemyChipPos(int enemyNum) const;
	Character::Enemy::EnemyInfo* GetEnemiesInfo() const;
	int GetEnemyScoutableRadius() const;
	int GetMaxStoneCount() const;
};

}
}

#endif