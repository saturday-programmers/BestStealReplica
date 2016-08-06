#ifndef PLAYER_H_
#define PLAYER_H_

#include <windows.h>

#include "CharacterCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Character {

class Player {
public:
	static const int MAX_COUNT_OF_AFTERIMAGE = 4;

	Player(POINT topLeftXY, Drawer* pDrawer);
	void Draw();
	void WalkX(int x);
	void WalkY(int y);
	void Stay();
	void Steal(int afterImageCount);
	void Move(POINT xy);
	bool IsStayingNearlyWindowTop();
	bool IsStayingNearlyWindowRight();
	bool IsStayingNearlyWindowBottom();
	bool IsStayingNearlyWindowLeft();
	Vertices<POINT> GetPlayerXY();

private:
	static const int CHIP_COUNT_PER_DIRECTION = 3;
	static const int PLAYER_HEIGHT = 60;
	static const int PLAYER_WIDTH = 50;

	static const int ROW_NUM_OF_HEADING_BOTTOM = 0;
	static const int COL_NUM_OF_HEADING_BOTTOM = 0;
	static const int ROW_NUM_OF_HEADING_TOP = 0;
	static const int COL_NUM_OF_HEADING_TOP = 3;
	static const int ROW_NUM_OF_HEADING_LEFT = 1;
	static const int COL_NUM_OF_HEADING_LEFT = 0;
	static const int ROW_NUM_OF_HEADING_RIGHT = 1;
	static const int COL_NUM_OF_HEADING_RIGHT = 3;

	static const int ROW_NUM_OF_STEALING = 2;
	static const int COL_NUM_OF_STEALING_LEFT = 0;
	static const int COL_NUM_OF_STEALING_BOTTOM = 1;
	static const int COL_NUM_OF_STEALING_TOP = 2;
	static const int COL_NUM_OF_STEALING_RIGHT = 3;


	static const int MAP_BUFFER = CharacterCommon::WIDTH;

	const char* FILE_PATH;

	Drawer* pDrawer;
	CharacterCommon::Direction headingDirection;
	int currentAnimationCnt;
	POINT topLeftXY;
	bool isStealing;
	int afterimageCount = 0;

	Vertices<FloatPoint> headingBottomChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingTopChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingLeftChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingRightChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> StealingBottomChip;
	Vertices<FloatPoint> StealingTopChip;
	Vertices<FloatPoint> StealingLeftChip;
	Vertices<FloatPoint> StealingRightChip;
	
	Vertices<DrawingVertex> GetVertex();
	Vertices<DrawingVertex>* GetVerticesOnStealing(int afterimageCount);
	void Walk(CharacterCommon::Direction nextDir);
};

}
}

#endif