#ifndef STAGE1_H_
#define STAGE1_H_

#include "IStage.h"

namespace BestStealReplica {

class Stage1 : public IStage {

public:
	static const int Y_CHIP_COUNT = 21;
	static const int X_CHIP_COUNT = 30;
	static const int ENERMY_COUNT = 7;

	POINT GetPlayerFirstChipPos() const;
	Map::MapCommon::MapChipType GetMapChipType(int y, int x) const;
	int GetEnermyCount() const;
	POINT GetEnermyChipPos(int enermyNum) const;
	Character::Enermy::EnermyInfo* GetEnermiesInfo() const ;
	int GetEnermySearchableRadius() const;
};

}

#endif