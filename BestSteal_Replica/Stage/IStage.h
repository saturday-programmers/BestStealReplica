#ifndef ISTAGE_H_
#define ISTAGE_H_

#include <vector>
#include <windows.h>

#include "../Map/MapCommon.h"
#include "../Character/Enemy.h"


namespace BestStealReplica {
namespace Stage {

class IStage {
public:
	/* Getters / Setters -------------------------------------------------------------------------------- */
	virtual int GetYChipCount() const = 0;
	virtual int GetXChipCount() const = 0;
	virtual Map::MapChipType GetMapChipType(int y, int x) const = 0;
	virtual void GetPlayerFirstChipPos(POINT* pRet) const = 0;
	virtual std::vector<Character::Enemy::EnemyInfo> GetEnemiesInfo() const = 0;
	virtual int GetEnemyScoutableRadius() const = 0;
	virtual int GetMaxStoneCount() const = 0;
};

}
}

#endif