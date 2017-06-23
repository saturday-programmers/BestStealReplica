#include "Player.h"
#include "Drawing/Drawer.h"


namespace BestStealReplica {
namespace Character {

/* Constructor / Destructor ------------------------------------------------------------------------- */
Player::Player(const POINT& rTopLeftPoint) : holdingSilverGateKeyCount(0), holdingGoldGateKeyCount(0) {
	this->defaultTopLeftPoint.x = rTopLeftPoint.x;
	this->defaultTopLeftPoint.y = rTopLeftPoint.y;
	this->topLeftPoint.x = rTopLeftPoint.x;
	this->topLeftPoint.y = rTopLeftPoint.y;

	CharacterCommon::CreateTexRect(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_TOP, Player::COL_NUM_OF_HEADING_TOP, this->texRectOfHeadingTopChips);
	CharacterCommon::CreateTexRect(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_RIGHT, Player::COL_NUM_OF_HEADING_RIGHT, this->texRectOfHeadingRightChips);
	CharacterCommon::CreateTexRect(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_BOTTOM, Player::COL_NUM_OF_HEADING_BOTTOM, this->texRectOfHeadingBottomChips);
	CharacterCommon::CreateTexRect(Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_LEFT, Player::COL_NUM_OF_HEADING_LEFT, this->texRectOfHeadingLeftChips);

	CharacterCommon::CreateTexRect(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_TOP, &this->texRectOfStealingTopChip);
	CharacterCommon::CreateTexRect(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_RIGHT, &this->texRectOfStealingRightChip);
	CharacterCommon::CreateTexRect(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_BOTTOM, &this->texRectOfStealingBottomChip);
	CharacterCommon::CreateTexRect(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_LEFT, &this->texRectOfStealingLeftChip);

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
			CreateDrawingVertexRectOnStealing(i - 1, &context.rect);
			pRet->push_back(context);
		}
	} else {
		Drawing::DrawingContext context;
		context.textureType = Drawing::TextureType::CHARACTER;
		CreateDrawingVertexRect(&context.rect);
		pRet->push_back(context);
	}
}

void Player::Walk(const POINT& rMovingPoint) {
	if (rMovingPoint.x == 0 && rMovingPoint.y == 0) {
		return;
	}

	this->topLeftPoint.x += rMovingPoint.x;
	this->topLeftPoint.y += rMovingPoint.y;
	
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
				this->topLeftPoint.y -= Player::MOVING_PIXEL_ON_STEALING;
				break;
			case AppCommon::Direction::RIGHT:
				this->topLeftPoint.x += Player::MOVING_PIXEL_ON_STEALING;
				break;
			case AppCommon::Direction::BOTTOM:
				this->topLeftPoint.y += Player::MOVING_PIXEL_ON_STEALING;
				break;
			case AppCommon::Direction::LEFT:
				this->topLeftPoint.x -= Player::MOVING_PIXEL_ON_STEALING;
				break;
			default:
				break;
		}
	}
}

void Player::Move(const POINT& rPixel) {
	this->topLeftPoint.x += rPixel.x;
	this->topLeftPoint.y += rPixel.y;
}

bool Player::IsStayingNearlyWindowTop() const {
	return (this->topLeftPoint.y < Player::MAP_BUFFER);
}

bool Player::IsStayingNearlyWindowRight() const {
	return (this->topLeftPoint.x + CharacterCommon::WIDTH + Player::MAP_BUFFER >= AppCommon::GetWindowWidth());
}

bool Player::IsStayingNearlyWindowBottom() const {
	return (this->topLeftPoint.y + CharacterCommon::HEIGHT + Player::MAP_BUFFER >= AppCommon::GetWindowHeight());
}

bool Player::IsStayingNearlyWindowLeft() const {
	return (this->topLeftPoint.x < Player::MAP_BUFFER);
}

void Player::CalcPlayerRect(Rectangle<POINT>* pRet) const {
	CharacterCommon::CalcCharacterRect(this->topLeftPoint, Player::PLAYER_WIDTH, Player::PLAYER_HEIGHT, pRet);
}

void Player::CalcCenter(POINT* pRet) const {
	Rectangle<POINT> playerRect;
	CalcPlayerRect(&playerRect);
	CharacterCommon::CalcCenter(playerRect, pRet);
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
	this->topLeftPoint = defaultTopLeftPoint;
	this->isStealing = false;
	this->currentKeepingStealingNum = 0;
	this->hasDirectionChanged = false;
}

void Player::CreateDrawingVertexRect(Rectangle<Drawing::DrawingVertex>* pRet) const {
	Rectangle<Point<float>> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->currentAnimationCnt);
	switch (this->headingDirection) {
		case AppCommon::Direction::TOP:
			chip = this->texRectOfHeadingTopChips[animationNum];
			break;
		case AppCommon::Direction::RIGHT:
			chip = this->texRectOfHeadingRightChips[animationNum];
			break;
		case AppCommon::Direction::BOTTOM:
			chip = this->texRectOfHeadingBottomChips[animationNum];
			break;
		case AppCommon::Direction::LEFT:
			chip = this->texRectOfHeadingLeftChips[animationNum];
			break;
	}

	CharacterCommon::CreateDrawingVertexRect(this->topLeftPoint, &CharacterCommon::ConvertTopLeftPointToRect, chip, pRet);
}

/**
 * 盗み処理中の画像情報を取得する 
 *
 * @param [in] afterImageNum 残像番号
 * @param [out] pRet 盗み処理中の頂点情報を格納して返却
 */
void Player::CreateDrawingVertexRectOnStealing(int afterImageNum, Rectangle<Drawing::DrawingVertex>* pRet) const {
	POINT afterImageTopLeftPoint;
	Rectangle<Point<float>> chipTexRect;
	switch (this->headingDirection) {
		case AppCommon::Direction::TOP:
			afterImageTopLeftPoint.x = this->topLeftPoint.x;
			afterImageTopLeftPoint.y = this->topLeftPoint.y + Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			chipTexRect = this->texRectOfStealingTopChip;
			break;
		case AppCommon::Direction::RIGHT:
			afterImageTopLeftPoint.x = this->topLeftPoint.x - Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			afterImageTopLeftPoint.y = this->topLeftPoint.y;
			chipTexRect = this->texRectOfStealingRightChip;
			break;
		case AppCommon::Direction::BOTTOM:
			afterImageTopLeftPoint.x = this->topLeftPoint.x;
			afterImageTopLeftPoint.y = this->topLeftPoint.y - Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			chipTexRect = this->texRectOfStealingBottomChip;
			break;
		case AppCommon::Direction::LEFT:
			afterImageTopLeftPoint.x = this->topLeftPoint.x + Player::MOVING_PIXEL_ON_STEALING * afterImageNum;
			afterImageTopLeftPoint.y = this->topLeftPoint.y;
			chipTexRect = this->texRectOfStealingLeftChip;
			break;
	}
	CharacterCommon::CreateDrawingVertexRect(afterImageTopLeftPoint, &CharacterCommon::ConvertTopLeftPointToRect, chipTexRect, pRet);
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
