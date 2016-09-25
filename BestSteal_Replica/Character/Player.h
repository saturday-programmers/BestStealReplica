﻿#ifndef PLAYER_H_
#define PLAYER_H_

#include <windows.h>

#include "CharacterCommon.h"


namespace BestStealReplica {
class Drawer;

namespace Character {

class Player {
public:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MOVING_PIXEL_ON_STEALING = 50;

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Player(POINT topLeftXY, Drawer* pDrawer);

	/* Getters / Setters -------------------------------------------------------------------------------- */
	bool GetIsStealing();
	AppCommon::Direction GetHeadingDirection();

	/* Functions ---------------------------------------------------------------------------------------- */
	void Draw();
	void SetDirection(AppCommon::Direction direction);
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
	bool HasKey(AppCommon::KeyType key);
	void AddKey(AppCommon::KeyType key);
	void SubtractKey(AppCommon::KeyType key);
	void GetKilled();

private:
	/* Constants ---------------------------------------------------------------------------------------- */
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

	const TCHAR* FILE_PATH;

	/* Variables ---------------------------------------------------------------------------------------- */
	Drawer* pDrawer;
	int currentAnimationCnt;
	POINT topLeftXY;
	POINT defaultTopLeftXY;
	int currentKeepingStealingNum;
	bool isDirectionChanged;
	bool isStealing;
	AppCommon::Direction headingDirection;
	int holdingGoldKeyCount;
	int holdingSilverKeyCount;

	Vertices<FloatPoint> headingBottomChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingTopChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingLeftChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> headingRightChips[Player::CHIP_COUNT_PER_DIRECTION];
	Vertices<FloatPoint> StealingBottomChip;
	Vertices<FloatPoint> StealingTopChip;
	Vertices<FloatPoint> StealingLeftChip;
	Vertices<FloatPoint> StealingRightChip;
	
	/* Functions ---------------------------------------------------------------------------------------- */
	void SetDefaultProperty();
	Vertices<DrawingVertex> GetVertex();
	Vertices<DrawingVertex> GetVerticesOnStealing(int afterimageNum);
	int* GetHoldingKeyCnt(AppCommon::KeyType key);
};

}
}

#endif