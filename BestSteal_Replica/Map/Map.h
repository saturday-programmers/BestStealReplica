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
	~Map();


	/* Getters / Setters -------------------------------------------------------------------------------- */
	std::vector<Drawing::TextureType> GetTextureTypes() const;

	POINT GetTopLeftXYonChip(POINT mapChipPos) const;
	MapChipType GetMapChipType(POINT mapChipPos) const;


	/* Functions ---------------------------------------------------------------------------------------- */
	void CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const;

	void Load(const Stage::IStage& rStage);
	void Move(POINT xy);
	void MoveToDefault();
	bool IsOnRoad(Vertices<POINT> xy) const;
	bool IsMovableX(int x) const;
	bool IsMovableY(int y) const;
	void KeepOpeningGates();
	POINT GetFrontMapChipPos(Vertices<POINT> playerXY, AppCommon::Direction headingDirection) const;
	bool StartOpeningGate(POINT mapChipPos);
	bool IsGateOpened(POINT mapChipPos) const;
	bool ExistsWallBetween(POINT xy1, POINT xy2) const;
	void OpenJewelryBox();
	POINT ConvertToMapChipPos(POINT xy) const;
	void AddStone(POINT topLeftXY, AppCommon::Direction direction);
	void AnimateStones();
	std::vector<Vertices<POINT>> GetDroppedStoneXYs() const;


private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const TCHAR* FILE_PATH;


	/* Variables ---------------------------------------------------------------------------------------- */
	POINT defaultTopLeft;
	POINT topLeft;
	DataTable<MapChip*> pMapData;
	std::vector<MapChipGate*> pGateMapChips;
	MapChipJewelry* pJewelryMapChip;
	std::vector<Stone*> pStones;


	/* Functions ---------------------------------------------------------------------------------------- */
	void SetChipXY();
	bool IsOnRoad(POINT mapChipPos) const;
};

}
}

#endif