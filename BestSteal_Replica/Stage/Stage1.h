#ifndef STAGE1_H_
#define STAGE1_H_

#include "IStage.h"

namespace BestStealReplica {
namespace Stage {

class Stage1 : public IStage {

public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Stage1() = default;
	Stage1(const Stage1&) = delete;

	/* Getters / Setters -------------------------------------------------------------------------------- */
	UINT16 GetYChipCount() const;
	UINT16 GetXChipCount() const;
	Map::MapChipType GetMapChipType(UINT16 y, UINT16 x) const;
	void GetPlayerFirstChipPos(Point<UINT16>* pRet) const;
	void GetEnemiesInfo(std::vector<Character::Enemy::EnemyInfo>* pRet) const;
	int GetEnemyScoutableRadius() const;
	UINT8 GetMaxStoneCount() const;

	/* Operator Overloads ------------------------------------------------------------------------------- */
	Stage1& operator=(const Stage1&) = delete;

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const UINT8 MAX_STONE_COUNT = 3;

};

}
}

#endif