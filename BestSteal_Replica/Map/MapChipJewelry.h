#ifndef MAP_CHIP_JEWELRY_H_
#define MAP_CHIP_JEWELRY_H_

#include <windows.h>

#include "MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipJewelry : public MapChip {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum State {
		CLOSED,
		OPENED
	};

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	MapChipJewelry();

	/* Getters / Setters -------------------------------------------------------------------------------- */
	MapChipJewelry::State GetState();

	/* Functions ---------------------------------------------------------------------------------------- */
	void OpenBox();

private:
	/* Variables ---------------------------------------------------------------------------------------- */
	MapChipJewelry::State state;
};

}
}

#endif
