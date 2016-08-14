#ifndef MAP_H_
#define MAP_H_

#include <windows.h>
#include <vector>

#include "AppCommon.h"
#include "MapChip.h"
#include "MapChipDoor.h"

namespace BestStealReplica {
class Drawer;
class IStage;

namespace Map {
class MapChpDoor;

class Map {
public:
	Map(int yChipCount, int xChipCount, Drawer* pDrawer);
	~Map();
	void Load(const IStage* pStage);
	void Draw();
	void Move(POINT xy);
	POINT GetTopLeftXYonChip(POINT mapChipPos);
	bool IsOnRoad(Vertices<POINT> xy);
	bool IsMovableX(int x);
	bool IsMovableY(int y);
	void KeepOpeningDoors();
	POINT GetFrontMapChipPos(Vertices<POINT> playerXY, AppCommon::Direction headingDirection);
	bool StartOpeningDoor(POINT mapChipPos);
	MapCommon::MapChipType GetMapChipType(POINT mapChipPos);
	bool IsDoorOpened(POINT mapChipPos);

private:
	static const int MAX_Y_CHIP_COUNT = 21;
	static const int MAX_X_CHIP_COUNT = 30;

	const char* FILE_PATH;

	int yChipCount;
	int xChipCount;
	Drawer* pDrawer;
	POINT topLeft;
	MapChip* mapData[MAX_Y_CHIP_COUNT][MAX_X_CHIP_COUNT];
	std::vector<MapChipDoor*> doorMapChips;

	void SetChipXY();
	POINT GetMapChipPos(POINT xy);
	bool IsOnRoad(POINT mapChipPos);
};

}
}

#endif