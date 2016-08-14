#ifndef ENERMY_H_
#define ENERMY_H_

#include <windows.h>

#include "CharacterCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Character {

class Enermy {
public:
	enum State {
		NORMAL,
		FOUND_PLAYER,
		LOST_PLAYER,
		GOT_STOLEN
	};

	enum KeyType {
		None,
		Silver,
		Gold
	};

	struct EnermyInfo {
		POINT chipPos;
		POINT topLeftXY;
		AppCommon::Direction defaultDirection;
		AppCommon::Direction headingDirection;
		Enermy::KeyType holdingKey;
		State state;
		int currentAnimationCnt;
		int restTimeForCancelFinding;
		int restTimeForBackingToNormal;

		EnermyInfo();
		EnermyInfo(int chipPosX, int chipPosY, AppCommon::Direction defaultDirection, KeyType holdingKey);
	};


	static const int MAX_ENERMY_COUNT = 7;

	Enermy(const POINT topLeftXY[Enermy::MAX_ENERMY_COUNT], EnermyInfo enermiesInfo[Enermy::MAX_ENERMY_COUNT], int enermyCount, Drawer* pDrawer);
	void Draw();
	void Stay();
	void Move(POINT xy);
	Vertices<POINT> GetEnermyXY(int enermyNum);
	void ScoutPlayer(Vertices<POINT> playerXY, int scoutableRadius, bool isPlayerWalking);
	KeyType GetStolen(Vertices<POINT> playerXY, bool isPlayerStealing);

private:
	static const int CHIP_COUNT_PER_DIRECTION = 3;
	static const int ENERMY_HEIGHT = 60;
	static const int ENERMY_WIDTH = 50;
	static const int TIME_FOR_CANCELING_FINDING = 120;
	static const int TIME_FOR_BACKING_TO_NORMAL = 80;

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