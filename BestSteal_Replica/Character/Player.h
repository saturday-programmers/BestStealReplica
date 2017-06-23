#ifndef PLAYER_H_
#define PLAYER_H_

#include "Character/CharacterCommon.h"
#include "Drawing/IDrawable.h"


namespace BestStealReplica {
class Drawer;

namespace Character {

class Player : public Drawing::IDrawable {
public:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MOVING_PIXEL_ON_STEALING = 50;


	/* Constructor / Destructor ------------------------------------------------------------------------- */
	explicit Player(const POINT& rTopLeftPoint);
	Player(const Player&) = delete;


	/* Getters / Setters -------------------------------------------------------------------------------- */
	bool IsStealing() const;
	AppCommon::Direction GetHeadingDirection() const;
	void SetDirection(AppCommon::Direction direction);


	/* Operator Overloads ------------------------------------------------------------------------------- */
	Player& operator=(const Player&) = delete;


	/* Functions ---------------------------------------------------------------------------------------- */
	void ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const;
	void CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const;

	void Walk(const POINT& rMovingPoint);
	void Stay();
	void StartStealing();
	void KeepStealing();
	void Move(const POINT& rPixel);
	bool IsStayingNearlyWindowTop() const;
	bool IsStayingNearlyWindowRight() const;
	bool IsStayingNearlyWindowBottom() const;
	bool IsStayingNearlyWindowLeft() const;
	void CalcPlayerRect(Rectangle<POINT>* pRet) const;
	void CalcCenter(POINT* pRet) const;
	bool HasGateKey(AppCommon::GateKeyType key) const;
	void GainGateKey(AppCommon::GateKeyType key);
	void LoseGateKey(AppCommon::GateKeyType key);
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

	static const TCHAR* FILE_PATH;


	/* Variables ---------------------------------------------------------------------------------------- */
	int currentAnimationCnt;
	POINT topLeftPoint;
	POINT defaultTopLeftPoint;
	int currentKeepingStealingNum;
	bool hasDirectionChanged;
	bool isStealing;
	AppCommon::Direction headingDirection;
	int holdingGoldGateKeyCount;
	int holdingSilverGateKeyCount;

	Rectangle<Point<float>> texRectOfHeadingBottomChips[Player::CHIP_COUNT_PER_DIRECTION];
	Rectangle<Point<float>> texRectOfHeadingTopChips[Player::CHIP_COUNT_PER_DIRECTION];
	Rectangle<Point<float>> texRectOfHeadingLeftChips[Player::CHIP_COUNT_PER_DIRECTION];
	Rectangle<Point<float>> texRectOfHeadingRightChips[Player::CHIP_COUNT_PER_DIRECTION];
	Rectangle<Point<float>> texRectOfStealingBottomChip;
	Rectangle<Point<float>> texRectOfStealingTopChip;
	Rectangle<Point<float>> texRectOfStealingLeftChip;
	Rectangle<Point<float>> texRectOfStealingRightChip;


	/* Functions ---------------------------------------------------------------------------------------- */
	void ReturnPropertiesToDefault();
	void CreateDrawingVertexRect(Rectangle<Drawing::DrawingVertex>* pRet) const;
	void CreateDrawingVertexRectOnStealing(int afterImageNum, Rectangle<Drawing::DrawingVertex>* pRet) const;
	const int* GetHoldingGateKeyCnt(AppCommon::GateKeyType key) const;

};

}
}

#endif