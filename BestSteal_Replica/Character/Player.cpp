#include "Player.h"
#include "Drawer.h"


namespace BestStealReplica {
namespace Character {

const TCHAR* Player::FILE_PATH = TEXT("image\\character.png");


/* Constructor / Destructor ------------------------------------------------------------------------- */
Player::Player(POINT topLeftXY, Drawer* pDrawer):
	pDrawer(pDrawer),
	defaultTopLeftXY(topLeftXY),
	holdingSilverKeyCount(0),
	holdingGoldKeyCount(0)
{
	CharacterCommon::SetTuTvs(this->headingTopChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_TOP, Player::COL_NUM_OF_HEADING_TOP);
	CharacterCommon::SetTuTvs(this->headingRightChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_RIGHT, Player::COL_NUM_OF_HEADING_RIGHT);
	CharacterCommon::SetTuTvs(this->headingBottomChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_BOTTOM, Player::COL_NUM_OF_HEADING_BOTTOM);
	CharacterCommon::SetTuTvs(this->headingLeftChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_LEFT, Player::COL_NUM_OF_HEADING_LEFT);

	this->StealingTopChip = CharacterCommon::GetTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_TOP);
	this->StealingRightChip = CharacterCommon::GetTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_RIGHT);
	this->StealingBottomChip = CharacterCommon::GetTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_BOTTOM);
	this->StealingLeftChip = CharacterCommon::GetTuTv(Player::ROW_NUM_OF_STEALING, Player::COL_NUM_OF_STEALING_LEFT);

	SetDefaultProperty();
	this->pDrawer->CreateTexture(this->FILE_PATH, Drawer::TextureType::CHARACTER);
}


/* Getters / Setters -------------------------------------------------------------------------------- */
bool Player::IsStealing() const {
	return this->isStealing;
}

AppCommon::Direction Player::GetHeadingDirection() const {
	return this->headingDirection;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Player::Draw() const {
	if (isStealing) {
		for (int i = this->currentKeepingStealingNum; i > 0; --i) {
			Vertices<DrawingVertex> vertex = GetVerticesOnStealing(i - 1);
			this->pDrawer->Draw(vertex, Drawer::TextureType::CHARACTER);
		}
	} else {
		this->pDrawer->Draw(GetVertex(), Drawer::TextureType::CHARACTER);
	}
}

void Player::SetDirection(AppCommon::Direction direction) {
	this->isDirectionChanged = (this->headingDirection != direction);
	this->headingDirection = direction;
}

void Player::Walk(POINT movingPoint) {
	if (movingPoint.x == 0 && movingPoint.y == 0) {
		return;
	}

	this->topLeftXY.x += movingPoint.x;
	this->topLeftXY.y += movingPoint.y;
	
	if (this->isDirectionChanged) {
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
	int holdingKeyCnt = GetHoldingKeyCnt(key);
	return (holdingKeyCnt > 0);
}

void Player::AddKey(AppCommon::KeyType key) {
	int* pHoldingKeyCnt = GetHoldingKeyCnt(key);
	if (pHoldingKeyCnt != NULL) {
		++(*pHoldingKeyCnt);
	}
}	

void Player::SubtractKey(AppCommon::KeyType key) {
	int* pHoldingKeyCnt = GetHoldingKeyCnt(key);
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
	this->isDirectionChanged = false;
}

Vertices<DrawingVertex> Player::GetVertex() const {
	Vertices<DrawingVertex> ret;

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

	ret = CharacterCommon::GetVertex(this->topLeftXY, &CharacterCommon::GetChipXY, chip);

	return ret;
}

/**
 * 盗み処理中の画像情報を取得する 
 *
 * @param afterImageNum 残像番号
 */
Vertices<DrawingVertex> Player::GetVerticesOnStealing(int afterImageNum) const {
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
	Vertices<DrawingVertex> ret = CharacterCommon::GetVertex(topLeftXY, &CharacterCommon::GetChipXY, chip);
	return ret;
}

int* Player::GetHoldingKeyCnt(AppCommon::KeyType key) {
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

int Player::GetHoldingKeyCnt(AppCommon::KeyType key) const {
	int ret = 0;
	switch (key) {
		case AppCommon::KeyType::Silver:
			ret = this->holdingSilverKeyCount;
			break;
		case AppCommon::KeyType::Gold:
			ret = this->holdingGoldKeyCount;
			break;
		default:
			break;
	}
	return ret;
}

}
}
