#include "Player.h"
#include "../Drawing/Drawer.h"


namespace BestStealReplica {
namespace Character {

/* Constructor / Destructor ------------------------------------------------------------------------- */
Player::Player(POINT topLeftXY) :
	defaultTopLeftXY(topLeftXY),
	holdingSilverKeyCount(0),
	holdingGoldKeyCount(0)
{
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_TOP, Player::COL_NUM_OF_HEADING_TOP, this->headingTopChips);
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_RIGHT, Player::COL_NUM_OF_HEADING_RIGHT, this->headingRightChips);
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_BOTTOM, Player::COL_NUM_OF_HEADING_BOTTOM, this->headingBottomChips);
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_LEFT, Player::COL_NUM_OF_HEADING_LEFT, this->headingLeftChips);

	this->StealingTopChip = CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_TOP);
	this->StealingRightChip = CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_RIGHT);
	this->StealingBottomChip = CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_BOTTOM);
	this->StealingLeftChip = CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_LEFT);

	SetDefaultProperty();
}


/* Getters / Setters -------------------------------------------------------------------------------- */
std::vector<Drawing::TextureType> Player::GetTextureTypes() const {
	std::vector<Drawing::TextureType> ret;
	ret.push_back(Drawing::TextureType::CHARACTER);
	return ret;
}

bool Player::IsStealing() const {
	return this->isStealing;
}

AppCommon::Direction Player::GetHeadingDirection() const {
	return this->headingDirection;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Player::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const {
	if (isStealing) {
		for (int i = this->currentKeepingStealingNum; i > 0; --i) {
			Drawing::DrawingContext context;
			context.textureType = Drawing::TextureType::CHARACTER;
			context.vertices = GetVerticesOnStealing(i - 1);
			pDrawingContexts->push_back(context);
		}
	} else {
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::CHARACTER;
		context.vertices = CreateVertex();
		pDrawingContexts->push_back(context);
	}
}

void Player::SetDirection(AppCommon::Direction direction) {
	this->hasDirectionChanged = (this->headingDirection != direction);
	this->headingDirection = direction;
}

void Player::Walk(POINT movingPoint) {
	if (movingPoint.x == 0 && movingPoint.y == 0) {
		return;
	}

	this->topLeftXY.x += movingPoint.x;
	this->topLeftXY.y += movingPoint.y;
	
	if (this->hasDirectionChanged) {
		// 向きが変わったらアニメーション番号を最初に戻す
		this->currentAnimationCnt = 0;
	} else {
		CharacterCommon::CountUpAnimationCnt(&this->currentAnimationCnt, Player::CHIP_COUNT_PER_DIRECTION);
	}
}

void Player::Stay() {
	this->isStealing = false;
	CharacterCommon::CountUpAnimationCnt(&this->currentAnimationCnt, Player::CHIP_COUNT_PER_DIRECTION);
}

void Player::StartStealing() {
	this->isStealing = true;
	this->currentKeepingStealingNum = 1;
}

void Player::KeepStealing() {
	++this->currentKeepingStealingNum;

	if (this->currentKeepingStealingNum > Player::COUNT_OF_KEEPING_STEALING) {
		// 盗み状態終了
		this->isStealing = false;
		this->currentKeepingStealingNum = 0;
	} else {
		switch (this->headingDirection) {
			case AppCommon::Direction::TOP:
				this->topLeftXY.y -= Player::MOVING_PIXEL_ON_STEALING;
				break;
			case AppCommon::Direction::RIGHT:
				this->topLeftXY.x += Player::MOVING_PIXEL_ON_STEALING;
				break;
			case AppCommon::Direction::BOTTOM:
				this->topLeftXY.y += Player::MOVING_PIXEL_ON_STEALING;
				break;
			case AppCommon::Direction::LEFT:
				this->topLeftXY.x -= Player::MOVING_PIXEL_ON_STEALING;
				break;
			default:
				break;
		}
	}
}

void Player::Move(POINT xy) {
	this->topLeftXY.x += xy.x;
	this->topLeftXY.y += xy.y;
}

bool Player::IsStayingNearlyWindowTop() const {
	return (this->topLeftXY.y < Player::MAP_BUFFER);
}

bool Player::IsStayingNearlyWindowRight() const {
	return (this->topLeftXY.x + CharacterCommon::WIDTH + Player::MAP_BUFFER >= AppCommon::GetWindowWidth());
}

bool Player::IsStayingNearlyWindowBottom() const {
	return (this->topLeftXY.y + CharacterCommon::HEIGHT + Player::MAP_BUFFER >= AppCommon::GetWindowHeight());
}

bool Player::IsStayingNearlyWindowLeft() const {
	return (this->topLeftXY.x < Player::MAP_BUFFER);
}

Vertices<POINT> Player::GetPlayerXY() const {
	Vertices<POINT> ret = CharacterCommon::GetChipXY(this->topLeftXY);
	int xDiff = (CharacterCommon::WIDTH - Player::PLAYER_WIDTH) / 2;
	int yDiff = (CharacterCommon::HEIGHT - Player::PLAYER_HEIGHT) / 2;
	ret.topLeft.x += xDiff;
	ret.topLeft.y += yDiff;
	ret.bottomRight.x -= xDiff;
	ret.bottomRight.y -= yDiff;
	return ret;
}

bool Player::HasKey(AppCommon::KeyType key) const {
	const int* pHoldingKeyCnt = GetHoldingKeyCnt(key);
	return (pHoldingKeyCnt == NULL ? 0 : (*pHoldingKeyCnt > 0));
}

void Player::AddKey(AppCommon::KeyType key) {
	int* pHoldingKeyCnt = (int*)GetHoldingKeyCnt(key);
	if (pHoldingKeyCnt != NULL) {
		++(*pHoldingKeyCnt);
	}
}	

void Player::SubtractKey(AppCommon::KeyType key) {
	int* pHoldingKeyCnt = (int*)GetHoldingKeyCnt(key);
	if (pHoldingKeyCnt != NULL) {
		--(*pHoldingKeyCnt);
	}
}

void Player::GetKilled() {
	SetDefaultProperty();
}


/* Private Functions  ------------------------------------------------------------------------------- */
void Player::SetDefaultProperty() {
	this->currentAnimationCnt = 0;
	this->headingDirection = AppCommon::Direction::BOTTOM;
	this->topLeftXY = defaultTopLeftXY;
	this->isStealing = false;
	this->currentKeepingStealingNum = 0;
	this->hasDirectionChanged = false;
}

Vertices<Drawing::DrawingVertex> Player::CreateVertex() const {
	Vertices<Drawing::DrawingVertex> ret;

	Vertices<FloatPoint> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->currentAnimationCnt);
	switch (this->headingDirection) {
		case AppCommon::Direction::TOP:
			chip = this->headingTopChips[animationNum];
			break;
		case AppCommon::Direction::RIGHT:
			chip = this->headingRightChips[animationNum];
			break;
		case AppCommon::Direction::BOTTOM:
			chip = this->headingBottomChips[animationNum];
			break;
		case AppCommon::Direction::LEFT:
			chip = this->headingLeftChips[animationNum];
			break;
	}

	ret = CharacterCommon::CreateVertex(this->topLeftXY, &CharacterCommon::GetChipXY, chip);

	return ret;
}

/**
 * 盗み処理中の画像情報を取得する 
 *
 * @param afterImageNum 残像番号
 */
Vertices<Drawing::DrawingVertex> Player::GetVerticesOnStealing(int afterImageNum) const {
	POINT topLeftXY;
	Vertices<FloatPoint> chip;
	switch (this->headingDirection) {
		case AppCommon::Direction::TOP:
			topLeftXY.x = this->topLeftXY.x;
			topLeftXY.y = this->topLeftXY.y + Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			chip = this->StealingTopChip;
			break;
		case AppCommon::Direction::RIGHT:
			topLeftXY.x = this->topLeftXY.x - Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			topLeftXY.y = this->topLeftXY.y;
			chip = this->StealingRightChip;
			break;
		case AppCommon::Direction::BOTTOM:
			topLeftXY.x = this->topLeftXY.x;
			topLeftXY.y = this->topLeftXY.y - Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			chip = this->StealingBottomChip;
			break;
		case AppCommon::Direction::LEFT:
			topLeftXY.x = this->topLeftXY.x + Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			topLeftXY.y = this->topLeftXY.y;
			chip = this->StealingLeftChip;
			break;
	}
	Vertices<Drawing::DrawingVertex> ret = CharacterCommon::CreateVertex(topLeftXY, &CharacterCommon::GetChipXY, chip);
	return ret;
}

const int* Player::GetHoldingKeyCnt(AppCommon::KeyType key) const {
	switch (key) {
		case AppCommon::KeyType::Silver:
			return &this->holdingSilverKeyCount;
			break;
		case AppCommon::KeyType::Gold:
			return &this->holdingGoldKeyCount;
			break;
		default:
			return NULL;
	}
}

}
}
