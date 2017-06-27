#ifndef MAP_CHIP_JEWELRY_H_
#define MAP_CHIP_JEWELRY_H_

#include "Map/MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipJewelry : public MapChip {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum struct State {
		CLOSED,
		OPENED
	};

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	MapChipJewelry();
	MapChipJewelry(const MapChipJewelry&) = delete;

	/* Getters / Setters -------------------------------------------------------------------------------- */
	MapChipJewelry::State GetState() const;

	/* Operator Overloads ------------------------------------------------------------------------------- */
	MapChipJewelry& operator=(const MapChipJewelry&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	void OpenBox();

private:
	/* Variables ---------------------------------------------------------------------------------------- */
	MapChipJewelry::State state;

};

}
}

#endif
