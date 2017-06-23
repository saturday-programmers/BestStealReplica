#ifndef MAP_CHIP_WALL_H_
#define MAP_CHIP_WALL_H_

#include <windows.h>

#include "../Map/MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipWall : public MapChip {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	MapChipWall();
	MapChipWall(const MapChipWall&) = delete;

	/* Operator Overloads ------------------------------------------------------------------------------- */
	MapChipWall& operator=(const MapChipWall&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	void AssignChipNumber();
	void SetNeedsTopLine();
	void SetNeedsRightLine();
	void SetNeedsBottomLine();
	void SetNeedsLeftLine();

private:
	/* Variables ---------------------------------------------------------------------------------------- */
	bool needsTopLine;
	bool needsRightLine;
	bool needsBottomLine;
	bool needsLeftLine;

};

}
}

#endif
