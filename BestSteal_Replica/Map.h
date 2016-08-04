#ifndef MAP_H_
#define MAP_H_

#include <windows.h>

#include "MapChipSetting.h"


namespace BestStealReplica {
	class Drawer;
	class IStage;

	namespace Map {
		class Map {
		public:
			Map(int yChipCount, int xChipCount, Drawer* pDrawer);
			void Load(const IStage* pStage);
			void Draw();
			void Move(POINT xy);
			POINT GetTopLeftXYonChip(POINT mapChipPos);
			bool IsOnRoad(Vertices<POINT> xy);
			bool IsMovableX(int x);
			bool IsMovableY(int y);

		private:
			static const int MAX_Y_CHIP_COUNT = 21;
			static const int MAX_X_CHIP_COUNT = 30;

			const char* FILE_PATH;

			int yChipCount;
			int xChipCount;
			Drawer* pDrawer;
			POINT topLeft;
			MapChipSetting mapData[MAX_Y_CHIP_COUNT][MAX_X_CHIP_COUNT];

			void SetChipXY();
			POINT GetMapChipPos(POINT xy);
			bool IsOnRoad(POINT mapChipPos);
		};
	};
};

#endif