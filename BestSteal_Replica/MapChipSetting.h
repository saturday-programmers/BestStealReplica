#ifndef MAP_CHIP_SETTING_H_
#define MAP_CHIP_SETTING_H_

#include <windows.h>

#include "AppCommon.h"
#include "MapCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Map {

class MapChipSetting {
public:
	static const int HEIGHT = 80;
	static const int WIDTH = 80;

	static Vertices<FloatPoint> GetTuTvs(int mapChipNumber);
	static Vertices<POINT> GetXY(POINT topLeftXY);

	MapChipSetting();
	explicit MapChipSetting(MapCommon::MapChipType chipType);
	MapCommon::MapChipType GetChipType();
	void SetChipNumber();
	POINT GetTopLeftXY();
	void SetXY(POINT topLeftXY);
	void SetNeedsTopLine();
	void SetNeedsRightLine();
	void SetNeedsBottomLine();
	void SetNeedsLeftLine();
	Vertices<DrawingVertex> GetVertex();

private:
	struct ChipNumber {
		static const int WALL_WITH_TOP_LINE = 21;
		static const int WALL_WITH_RIGHT_LINE = 23;
		static const int WALL_WITH_BOTTOM_LINE = 22;
		static const int WALL_WITH_LEFT_LINE = 20;

		static const int WALL_WITH_TOP_RIGHT_LINE = 37;
		static const int WALL_WITH_BOTTOM_RIGHT_LINE = 39;
		static const int WALL_WITH_BOTTOM_LEFT_LINE = 38;
		static const int WALL_WITH_TOP_LEFT_LINE = 36;

		static const int WALL_WITH_TOP_BOTTOM_LINE = 32;
		static const int WALL_WITH_RIGHT_LEFT_LINE = 33;

		static const int WALL_WITHOUT_TOP_LINE = 16;
		static const int WALL_WITHOUT_RIGHT_LINE = 14;
		static const int WALL_WITHOUT_BOTTOM_LINE = 15;
		static const int WALL_WITHOUT_LEFT_LINE = 17;

		static const int WALL_WITH_SURROUNDED_LINE = 35;
	};

	static const int CHIP_COUNT_PER_ROW = 12;
	static const int CHIP_COUNT_PER_COL = 8;

	MapCommon::MapChipType chipType;
	int chipNumber;
	Vertices<DrawingVertex> vertices;

	bool needsTopLine = false;
	bool needsRightLine = false;
	bool needsBottomLine = false;
	bool needsLeftLine = false;
};
}
}

#endif