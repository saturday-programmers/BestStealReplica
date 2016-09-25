#ifndef MAP_CHIP_WALL_H_
#define MAP_CHIP_WALL_H_

#include <windows.h>

#include "MapChip.h"


namespace BestStealReplica {
namespace Map {

class MapChipWall : public MapChip {
public:
	MapChipWall();
	void SetChipNumber();
	void SetNeedsTopLine();
	void SetNeedsRightLine();
	void SetNeedsBottomLine();
	void SetNeedsLeftLine();

private:
	bool needsTopLine = false;
	bool needsRightLine = false;
	bool needsBottomLine = false;
	bool needsLeftLine = false;
};

}
}

#endif
