#ifndef STAGE1_H_
#define STAGE1_H_

#include "../Stage/IStage.h"

namespace BestStealReplica {
namespace Stage {

class Stage1 : public IStage {

public:
	/* Getters / Setters -------------------------------------------------------------------------------- */
	int GetYChipCount() const;
	int GetXChipCount() const;
	Map::MapChipType GetMapChipType(int y, int x) const;
	POINT GetPlayerFirstChipPos() const;
	std::vector<Character::Enemy::EnemyInfo> GetEnemiesInfo() const;
	int GetEnemyScoutableRadius() const;
	int GetMaxStoneCount() const;

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MAX_STONE_COUNT = 3;
};

}
}

#endif