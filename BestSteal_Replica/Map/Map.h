#ifndef MAP_H_
#define MAP_H_

#include <windows.h>
#include <vector>

#include "../AppCommon.h"
#include "../Map/MapCommon.h"
#include "../Drawing/IDrawable.h"


namespace BestStealReplica {
class Drawer;

namespace Stage {
class IStage;
}

namespace Map {
class MapChip;
class MapChipGate;
class MapChipJewelry;
class Stone;

class Map : public Drawing::IDrawable {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Map();
	Map(const Map&) = delete;
	~Map();


	/* Getters / Setters -------------------------------------------------------------------------------- */
	MapChipType GetMapChipType(const POINT& rMapChipPos) const;


	/* Operator Overloads ------------------------------------------------------------------------------- */
	Map& operator=(const Map&) = delete;


	/* Functions ---------------------------------------------------------------------------------------- */
	void ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const;
	void CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const;

	void Load(const Stage::IStage& rStage);
	void Move(const POINT& rPixel);
	void MoveToDefault();
	bool IsMovableX(int x) const;
	bool IsMovableY(int y) const;
	bool IsOnRoad(const Rectangle<POINT>& rRect) const;
	void ConvertMapChipPosToTopLeftPoint(const POINT& rCharacterRect, POINT* pRet) const;
	void KeepOpeningGates();
	void ConvertToCharacterFrontMapChipPos(const Rectangle<POINT>& rCharacterRect, AppCommon::Direction headingDirection, POINT* pRet) const;
	bool StartOpeningGate(const POINT& rMapChipPos);
	bool IsGateOpened(const POINT& rMapChipPos) const;
	bool ExistsWallBetween(const POINT& rPoint1, const POINT& rPoint2) const;
	void OpenJewelryBox();
	void ConvertToMapChipPos(const POINT& rPoint, POINT* pRet) const;
	void AddStone(const POINT& rTopLeftPoint, AppCommon::Direction direction);
	void AnimateStones();
	void CalcDroppedStonesRect(std::vector<Rectangle<POINT>>* pRet) const;


private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const TCHAR* FILE_PATH;


	/* Variables ---------------------------------------------------------------------------------------- */
	POINT defaultTopLeftPoint;
	POINT topLeftPoint;
	DataTable<MapChip*> pMapData;
	std::vector<MapChipGate*> pGateMapChips;
	MapChipJewelry* pJewelryMapChip;
	std::vector<Stone*> pStones;


	/* Functions ---------------------------------------------------------------------------------------- */
	void AssignChipNumber();
	void ConfigureChipPoint();
	bool IsOnRoad(const POINT& rMapChipPos) const;
	bool IsMovable(int targetPoint, int topLeftPoint, int mapChipCount, int mapChipSize, int windowSize) const;

};

}
}

#endif