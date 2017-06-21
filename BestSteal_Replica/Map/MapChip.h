#ifndef MAP_CHIP_H_
#define MAP_CHIP_H_

#include <windows.h>

#include "../AppCommon.h"
#include "../Map/MapCommon.h"
#include "../Drawing/DrawingCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Map {

class MapChip {
public:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int HEIGHT = 80;
	static const int WIDTH = 80;

	/* Static Functions --------------------------------------------------------------------------------- */
	static MapChip* Create(MapChipType chipType);
	static void ConvertChipNumberToTexRect(int mapChipNumber, Rectangle<FloatPoint>* pRet);
	static void ConvertTopLeftPointToRect(const POINT& rTopLeftPoint, Rectangle<POINT>* pRet);

	/* Getters / Setters -------------------------------------------------------------------------------- */
	MapChipType GetChipType() const;
	void GetTopLeftPoint(POINT* pRet) const;
	void SetTopLeftPoint(const POINT& rTopLeftPoint);

	/* Functions ---------------------------------------------------------------------------------------- */
	virtual void AssignChipNumber();
	void CreateDrawingVertexRect(Rectangle<Drawing::DrawingVertex>* pRet) const;

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

		ChipNumber() = delete;
		ChipNumber(const ChipNumber&) = delete;
	};

	/* Constants ---------------------------------------------------------------------------------------- */
	static const int CHIP_COUNT_PER_ROW = 12;
	static const int CHIP_COUNT_PER_COL = 8;

	/* Variables ---------------------------------------------------------------------------------------- */
	MapChipType chipType;
	int chipNumber;
	Rectangle<Drawing::DrawingVertex> drawingVertexRect;

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	explicit MapChip(MapChipType chipType);
	MapChip(const MapChip&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	void ConfigureTexRect();

};

}
}

#endif