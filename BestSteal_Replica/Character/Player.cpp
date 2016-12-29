#include "Player.h"
#include "../Drawing/Drawer.h"


namespace BestStealReplica {
namespace Character {

/* Constructor / Destructor ------------------------------------------------------------------------- */
Player::Player(POINT topLeftXY) :
	defaultTopLeftXY(topLeftXY),
	holdingSilverGateKeyCount(0),
	holdingGoldGateKeyCount(0)
{
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_TOP, Player::COL_NUM_OF_HEADING_TOP, this->headingTopChips);
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_RIGHT, Player::COL_NUM_OF_HEADING_RIGHT, this->headingRightChips);
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_BOTTOM, Player::COL_NUM_OF_HEADING_BOTTOM, this->headingBottomChips);
	CharacterCommon::CreateChipTuTvs(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_LEFT, Player::COL_NUM_OF_HEADING_LEFT, this->headingLeftChips);

	CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_TOP, &this->StealingTopChip);
	CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_RIGHT, &this->StealingRightChip);
	CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_BOTTOM, &this->StealingBottomChip);
	CharacterCommon::CreateTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_LEFT, &this->StealingLeftChip);

	ReturnPropertiesToDefault();
}


/* Getters / Setters -------------------------------------------------------------------------------- */
void Player::ConfigureTextureTypes(std::vector<Drawing::TextureType>* pRet) const {
	pRet->push_back(Drawing::TextureType::CHARACTER);
}

bool Player::IsStealing() const {
	return this->isStealing;
}

AppCommon::Direction Player::GetHeadingDirection() const {
	return this->headingDirection;
}

void Player::SetDirection(AppCommon::Direction direction) {
	this->hasDirectionChanged = (this->headingDirection != direction);
	this->headingDirection = direction;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Player::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pRet) const {
	if (isStealing) {
		for (int i = this->currentKeepingStealingNum; i > 0; --i) {
			Drawing::DrawingContext context;
			context.textureType = Drawing::TextureType::CHARACTER;
			CalcVerticesOnStealing(i - 1, &context.vertices);
			pRet->push_back(context);
		}
	} else {
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::CHARACTER;
		CreateVertices(&context.vertices);
		pRet->push_back(context);
	}
}

void Player::Walk(const POINT& rMovingPoint) {
	if (rMovingPoint.x == 0 && rMovingPoint.y == 0) {
		return;
	}

	this->topLeftXY.x += rMovingPoint.x;
	this->topLeftXY.y += rMovingPoint.y;
	
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

void Player::Move(const POINT& rXY) {
	this->topLeftXY.x += rXY.x;
	this->topLeftXY.y += rXY.y;
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

void Player::CalcPlayerXY(Vertices<POINT>* pRet) const {
	CharacterCommon::CalcCharacterXY(this->topLeftXY, Player::PLAYER_WIDTH, Player::PLAYER_HEIGHT, pRet);
}

void Player::CalcCenterXY(POINT* pRet) const {
	Vertices<POINT> playerXY;
	CalcPlayerXY(&playerXY);
	CharacterCommon::CalcCenter(playerXY, pRet);
}

bool Player::HasGateKey(AppCommon::GateKeyType gateKey) const {
	const int* pHoldingGateKeyCnt = GetHoldingGateKeyCnt(gateKey);
	return (pHoldingGateKeyCnt == NULL ? 0 : (*pHoldingGateKeyCnt > 0));
}

void Player::GainGateKey(AppCommon::GateKeyType gateKey) {
	int* pHoldingGateKeyCnt = (int*)GetHoldingGateKeyCnt(gateKey);
	if (pHoldingGateKeyCnt != NULL) {
		++(*pHoldingGateKeyCnt);
	}
}	

void Player::LoseGateKey(AppCommon::GateKeyType gateKey) {
	int* pHoldingGateKeyCnt = (int*)GetHoldingGateKeyCnt(gateKey);
	if (pHoldingGateKeyCnt != NULL) {
		--(*pHoldingGateKeyCnt);
	}
}

void Player::GetKilled() {
	ReturnPropertiesToDefault();
}


/* Private Functions  ------------------------------------------------------------------------------- */
void Player::ReturnPropertiesToDefault() {
	this->currentAnimationCnt = 0;
	this->headingDirection = AppCommon::Direction::BOTTOM;
	this->topLeftXY = defaultTopLeftXY;
	this->isStealing = false;
	this->currentKeepingStealingNum = 0;
	this->hasDirectionChanged = false;
}

void Player::CreateVertices(Vertices<Drawing::DrawingVertex>* pRet) const {
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

	CharacterCommon::CreateDrawingVertices(this->topLeftXY, &CharacterCommon::ConvertTopLeftXYToVertices, chip, pRet);
}

/**
 * 盗み処理中の画像情報を取得する 
 *
 * @param afterImageNum 残像番号
 */
void Player::CalcVerticesOnStealing(int afterImageNum, Vertices<Drawing::DrawingVertex>* pRet) const {
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
	CharacterCommon::CreateDrawingVertices(topLeftXY, &CharacterCommon::ConvertTopLeftXYToVertices, chip, pRet);
}

const int* Player::GetHoldingGateKeyCnt(AppCommon::GateKeyType gateKey) const {
	switch (gateKey) {
		case AppCommon::GateKeyType::Silver:
			return &this->holdingSilverGateKeyCount;
			break;
		case AppCommon::GateKeyType::Gold:
			return &this->holdingGoldGateKeyCount;
			break;
		default:
			return NULL;
	}
}

}
}
