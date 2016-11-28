﻿#ifndef MAP_CHIP_H_
#define MAP_CHIP_H_

#include <windows.h>

#include "AppCommon.h"
#include "MapCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Map {

class MapChip {
public:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int HEIGHT = 80;
	static const int WIDTH = 80;

	/* Static Functions --------------------------------------------------------------------------------- */
	static MapChip* Create(MapCommon::MapChipType chipType);
	static Vertices<FloatPoint> GetTuTvs(int mapChipNumber);
	static Vertices<POINT> GetXY(POINT topLeftXY);

	/* Getters / Setters -------------------------------------------------------------------------------- */
	MapCommon::MapChipType GetChipType() const;
	virtual void SetChipNumber();
	POINT GetTopLeftXY() const;
	void SetXY(POINT topLeftXY);
	Vertices<DrawingVertex> GetVertex() const;

protected:
	/* Structs ------------------------------------------------------------------------------------------ */
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

	/* Constants ---------------------------------------------------------------------------------------- */
	static const int CHIP_COUNT_PER_ROW = 12;
	static const int CHIP_COUNT_PER_COL = 8;

	/* Variables ---------------------------------------------------------------------------------------- */
	MapCommon::MapChipType chipType;
	int chipNumber;
	Vertices<DrawingVertex> vertices;

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	explicit MapChip(MapCommon::MapChipType chipType);

	/* Functions ---------------------------------------------------------------------------------------- */
	void SetTuTv();
};

}
}

#endif