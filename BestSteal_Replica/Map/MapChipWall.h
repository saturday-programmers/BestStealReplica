#ifndef MAP_CHIP_WALL_H_
#define MAP_CHIP_WALL_H_

#include <windows.h>

#include "MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipWall : public MapChip {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	MapChipWall();

	/* Functions ---------------------------------------------------------------------------------------- */
	void SetChipNumber();
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
