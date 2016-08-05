#ifndef ENERMY_H_
#define ENERMY_H_

#include <windows.h>

#include "CharacterCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Character {

class Enermy {
public:
	struct EnermyInfo {
		POINT chipPos;
		POINT topLeftXY;
		CharacterCommon::Direction defaultDirection;
		CharacterCommon::Direction headingDirection;
		bool hasKey;
		bool hasFoundPlayer = false;
		int currentAnimationCnt = 0;
		int restTimeForCancelFinding = 0;
		int restTimeForBackingToNormal = 0;

		EnermyInfo();
		EnermyInfo(int chipPosX, int chipPosY, CharacterCommon::Direction defaultDirection, bool hasKey);
	};

	static const int MAX_ENERMY_COUNT = 7;

	Enermy(const POINT topLeftXY[Enermy::MAX_ENERMY_COUNT], EnermyInfo enermiesInfo[Enermy::MAX_ENERMY_COUNT], int enermyCount, Drawer* pDrawer);
	void Draw();
	void Stay();
	void Move(POINT xy);
	Vertices<POINT> GetEnermyXY(int enermyNum);
//			int GetEnermyCount();
	void ScoutPlayer(Vertices<POINT> playerXY, int scoutableRadius);

private:
	static const int CHIP_COUNT_PER_DIRECTION = 3;
	static const int ENERMY_HEIGHT = 60;
	static const int ENERMY_WIDTH = 50;
	static const int TIME_FOR_CANCELING_FINDING = 120;
	static const int TIME_FOR_BACKING_TO_NORMAL = 120;

	static const int ROW_NUM_OF_HEADING_BOTTOM = 4;
	static const int COL_NUM_OF_HEADING_BOTTOM = 0;
	static const int ROW_NUM_OF_HEADING_TOP = 4;
	static const int COL_NUM_OF_HEADING_TOP = 3;
	static const int ROW_NUM_OF_HEADING_LEFT = 5;
	static const int COL_NUM_OF_HEADING_LEFT = 0;
	static const int ROW_NUM_OF_HEADING_RIGHT = 5;
	static const int COL_NUM_OF_HEADING_RIGHT = 3;
	static const int MAP_CHIP_NUMBER_OF_EXCL = 66;

	const char* FILE_PATH;

	Vertices<FloatPoint> headingBottomChips[Enermy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingTopChips[Enermy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingLeftChips[Enermy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingRightChips[Enermy::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> exclamationMarkChip;

	int enermyCount;
	EnermyInfo enermiesInfo[MAX_ENERMY_COUNT];
	Drawer* pDrawer;

	Vertices<DrawingVertex> GetVertex(int enermyNum);

};

}
}

#endif