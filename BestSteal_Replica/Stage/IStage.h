#ifndef ISTAGE_H_
#define ISTAGE_H_

#include <vector>
#include <windows.h>

#include "MapCommon.h"
#include "Enemy.h"


namespace BestStealReplica {
namespace Stage {

class IStage {
public:
	/* Getters / Setters -------------------------------------------------------------------------------- */
	virtual int GetYChipCount() const = 0;
	virtual int GetXChipCount() const = 0;
	virtual Map::MapCommon::MapChipType GetMapChipType(int y, int x) const = 0;
	virtual POINT GetPlayerFirstChipPos() const = 0;
	virtual std::vector<Character::Enemy::EnemyInfo> GetEnemiesInfo() const = 0;
	virtual int GetEnemyScoutableRadius() const = 0;
	virtual int GetMaxStoneCount() const = 0;
};

}
}

#endif