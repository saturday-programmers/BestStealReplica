#ifndef ISTAGE_H_
#define ISTAGE_H_

#include <windows.h>

#include "MapCommon.h"
#include "Enermy.h"


namespace BestStealReplica {
	class IStage {
	public:
		virtual Map::MapCommon::MapChipType GetMapChipType(int y, int x) const = 0;
		virtual POINT GetPlayerFirstChipPos() const = 0;
		virtual int GetEnermyCount() = 0;
		virtual POINT GetEnermyChipPos(int enermyNum) = 0;
		virtual Character::Enermy::EnermyInfo* GetEnermiesInfo() = 0;
		virtual int GetEnermySearchableRadius() = 0;
	};
};

#endif