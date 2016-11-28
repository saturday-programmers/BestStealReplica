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

	/* Getters / Setters -------------------------------------------------------------------------------- */
	POINT GetTopLeftXYonChip(POINT mapChipPos) const;
	MapCommon::MapChipType GetMapChipType(POINT mapChipPos) const;

	/* Functions ---------------------------------------------------------------------------------------- */
	void Load(const Stage::IStage& rStage);
	void Draw() const;
	void Move(POINT xy);
	void MoveToDefault();
	bool IsOnRoad(Vertices<POINT> xy) const;
	bool IsMovableX(int x) const;
	bool IsMovableY(int y) const;
	void KeepOpeningDoors();
	POINT GetFrontMapChipPos(Vertices<POINT> playerXY, AppCommon::Direction headingDirection) const;
	bool StartOpeningDoor(POINT mapChipPos);
	bool IsDoorOpened(POINT mapChipPos) const;
	bool ExistsWallBetween(POINT xy1, POINT xy2) const;
	void OpenJewelryBox();
	POINT ConvertToMapChipPos(POINT xy) const;
	void AddStone(POINT topLeftXY, AppCommon::Direction direction);
	void AnimateStones();
	std::vector<Vertices<POINT>> GetDroppedStoneXYs() const;

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MAX_Y_CHIP_COUNT = 21;
	static const int MAX_X_CHIP_COUNT = 30;

	static const TCHAR* FILE_PATH;

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
	bool IsOnRoad(POINT mapChipPos) const;
};

}
}

#endif