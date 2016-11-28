#ifndef MAP_CHIP_DOOR_H_
#define MAP_CHIP_DOOR_H_

#include <windows.h>

#include "MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipDoor : public MapChip {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum State {
		CLOSED,
		START_OPENING,
		OPENING,
		OPENED
	};

	/* Getters / Setters -------------------------------------------------------------------------------- */
	MapChipDoor::State GetState() const;

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	explicit MapChipDoor(MapCommon::MapChipType chipType);

	/* Functions ---------------------------------------------------------------------------------------- */
	void StartOpeningDoor();
	void OpenDoor();

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int OPENING_CHIP_COUNT = 3;

	/* Variables ---------------------------------------------------------------------------------------- */
	State state;
	int openingCount;
};

}
}

#endif
