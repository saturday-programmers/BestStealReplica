#ifndef MAP_CHIP_GATE_H_
#define MAP_CHIP_GATE_H_

#include <windows.h>

#include "../Map/MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipGate : public MapChip {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum State {
		CLOSED,
		START_OPENING,
		OPENING,
		OPENED
	};

	/* Getters / Setters -------------------------------------------------------------------------------- */
	MapChipGate::State GetState() const;

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	explicit MapChipGate(MapChipType chipType);
	MapChipGate(const MapChipGate&) = delete;

	/* Operator Overloads ------------------------------------------------------------------------------- */
	MapChipGate& operator=(const MapChipGate&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	void StartOpeningGate();
	void OpenGate();

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
