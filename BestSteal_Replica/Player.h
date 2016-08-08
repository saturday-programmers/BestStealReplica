#ifndef PLAYER_H_
#define PLAYER_H_

#include <windows.h>

#include "CharacterCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Character {

class Player {
public:
	static const int MOVING_PIXEL_ON_STEALING = 50;

	bool isStealing;
	CharacterCommon::Direction headingDirection;

	Player(POINT topLeftXY, Drawer* pDrawer);
	void Draw();
	void SetDirection(CharacterCommon::Direction direction);
	void Walk(POINT movingPoint);
	void Stay();
	void StartStealing();
	void KeepStealing();
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
	static const int COUNT_OF_KEEPING_STEALING = 5;

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
	int currentAnimationCnt;
	POINT topLeftXY;
	int currentKeepingStealingNum;
	bool isDirectionChanged;

	Vertices<FloatPoint> headingBottomChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingTopChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingLeftChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingRightChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> StealingBottomChip;
	Vertices<FloatPoint> StealingTopChip;
	Vertices<FloatPoint> StealingLeftChip;
	Vertices<FloatPoint> StealingRightChip;
	
	Vertices<DrawingVertex> GetVertex();
	Vertices<DrawingVertex> GetVerticesOnStealing(int afterimageNum);
};

}
}

#endif