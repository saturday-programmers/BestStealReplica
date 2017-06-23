#ifndef ISTAGE_H_
#define ISTAGE_H_

#include "Map/MapCommon.h"
#include "Character/Enemy.h"


namespace BestStealReplica {
namespace Stage {

class IStage {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	IStage() = default;
	IStage(const IStage&) = delete;
	virtual ~IStage() = default;

	/* Operator Overloads ------------------------------------------------------------------------------- */
	IStage& operator=(const IStage&) = delete;

	/* Getters / Setters -------------------------------------------------------------------------------- */
	virtual UINT16 GetYChipCount() const = 0;
	virtual UINT16 GetXChipCount() const = 0;
	virtual Map::MapChipType GetMapChipType(UINT16 y, UINT16 x) const = 0;
	virtual void GetPlayerFirstChipPos(Point<UINT16>* pRet) const = 0;
	virtual void GetEnemiesInfo(std::vector<Character::Enemy::EnemyInfo>* pRet) const = 0;
	virtual int GetEnemyScoutableRadius() const = 0;
	virtual UINT8 GetMaxStoneCount() const = 0;

};

}
}

#endif