﻿#ifndef ISTAGE_H_
#define ISTAGE_H_

#include <vector>
#include <windows.h>

#include "../Map/MapCommon.h"
#include "../Character/Enemy.h"


namespace BestStealReplica {
namespace Stage {

class IStage {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	IStage() = default;
	IStage(const IStage&) = delete;

	/* Operator Overloads ------------------------------------------------------------------------------- */
	IStage& operator=(const IStage&) = delete;

	/* Getters / Setters -------------------------------------------------------------------------------- */
	virtual int GetYChipCount() const = 0;
	virtual int GetXChipCount() const = 0;
	virtual Map::MapChipType GetMapChipType(int y, int x) const = 0;
	virtual void GetPlayerFirstChipPos(POINT* pRet) const = 0;
	virtual void GetEnemiesInfo(std::vector<Character::Enemy::EnemyInfo>* pRet) const = 0;
	virtual int GetEnemyScoutableRadius() const = 0;
	virtual int GetMaxStoneCount() const = 0;

};

}
}

#endif