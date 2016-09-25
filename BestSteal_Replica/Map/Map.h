#ifndef MAP_H_
#define MAP_H_

#include <windows.h>
#include <vector>

#include "AppCommon.h"
#include "MapChip.h"
#include "MapChipDoor.h"

namespace BestStealReplica {
class Drawer;

namespace Stage {
class IStage;
}

namespace Map {
class MapChpDoor;
class MapChipJewelry;
class Stone;

class Map {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Map(int yChipCount, int xChipCount, Drawer* pDrawer);
	~Map();

	/* Functions ---------------------------------------------------------------------------------------- */
	void Load(const Stage::IStage* pStage);
	void Draw();
	void Move(POINT xy);
	void MoveToDefault();
	POINT GetTopLeftXYonChip(POINT mapChipPos);
	bool IsOnRoad(Vertices<POINT> xy);
	bool IsMovableX(int x);
	bool IsMovableY(int y);
	void KeepOpeningDoors();
	POINT GetFrontMapChipPos(Vertices<POINT> playerXY, AppCommon::Direction headingDirection);
	bool StartOpeningDoor(POINT mapChipPos);
	MapCommon::MapChipType GetMapChipType(POINT mapChipPos);
	bool IsDoorOpened(POINT mapChipPos);
	bool ExistsWallBetween(POINT xy1, POINT xy2);
	void OpenJewelryBox();
	POINT GetMapChipPos(POINT xy);
	void AddStone(POINT topLeftXY, AppCommon::Direction direction);
	void AnimateStones();

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MAX_Y_CHIP_COUNT = 21;
	static const int MAX_X_CHIP_COUNT = 30;

	const TCHAR* FILE_PATH;

	/* Variables ---------------------------------------------------------------------------------------- */
	int yChipCount;
	int xChipCount;
	Drawer* pDrawer;
	POINT defaultTopLeft;
	POINT topLeft;
	MapChip* pMapData[MAX_Y_CHIP_COUNT][MAX_X_CHIP_COUNT];
	std::vector<MapChipDoor*> pDoorMapChips;
	MapChipJewelry* pJewelryMapChip;
	std::vector<Stone*> pStones;

	/* Functions ---------------------------------------------------------------------------------------- */
	void SetChipXY();
	bool IsOnRoad(POINT mapChipPos);
};

}
}

#endif