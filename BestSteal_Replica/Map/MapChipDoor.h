#ifndef MAP_CHIP_DOOR_H_
#define MAP_CHIP_DOOR_H_

#include <windows.h>

#include "MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipDoor : public MapChip {
public:
	enum State {
		CLOSED,
		START_OPENING,
		OPENING,
		OPENED
	};

	State state;

	MapChipDoor(MapCommon::MapChipType chipType);
	void StartOpeningDoor();
	void OpenDoor();

private:
	const int OPENING_CHIP_COUNT = 3;

	int openingCount;
};

}
}

#endif
