#include "Player.h"
#include "Drawer.h"


namespace BestStealReplica {
namespace Character {

Player::Player(POINT topLeftXY, Drawer* pDrawer):
	FILE_PATH("image\\character.png"),
	pDrawer(pDrawer),
	currentAnimationCnt(0),
	headingDirection(CharacterCommon::Direction::BOTTOM),
	topLeftXY(topLeftXY)
{
	CharacterCommon::SetTuTvs(this->headingTopChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_TOP, Player::COL_NUM_OF_HEADING_TOP);
	CharacterCommon::SetTuTvs(this->headingRightChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_RIGHT, Player::COL_NUM_OF_HEADING_RIGHT);
	CharacterCommon::SetTuTvs(this->headingBottomChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_BOTTOM, Player::COL_NUM_OF_HEADING_BOTTOM);
	CharacterCommon::SetTuTvs(this->headingLeftChips, Player::CHIP_COUNT_PER_DIRECTION, Player::ROW_NUM_OF_HEADING_LEFT, Player::COL_NUM_OF_HEADING_LEFT);

	pDrawer->CreateTexture(this->FILE_PATH, Drawer::TextureType::CHARACTER);
}

void Player::Draw() {
	this->pDrawer->Draw(GetVertex(), Drawer::TextureType::CHARACTER);
}

void Player::WalkX(int x) {
	if (x == 0) {
		return;
	}

	this->topLeftXY.x += x;

	CharacterCommon::Direction dir;
	if (x > 0) {
		dir = CharacterCommon::Direction::RIGHT;
	} else {
		dir = CharacterCommon::Direction::LEFT;
	}

	Walk(dir);
}

void Player::WalkY(int y) {
	if (y == 0) {
		return;
	}

	this->topLeftXY.y += y;

	CharacterCommon::Direction dir;
	if (y > 0) {
		dir = CharacterCommon::Direction::BOTTOM;
	} else {
		dir = CharacterCommon::Direction::TOP;
	}

	Walk(dir);
}

void Player::Stay() {
	CharacterCommon::CountUpAnimationCnt(&this->currentAnimationCnt, Player::CHIP_COUNT_PER_DIRECTION);
}

void Player::Move(POINT xy) {
	this->topLeftXY.x += xy.x;
	this->topLeftXY.y += xy.y;
}

bool Player::IsStayingNearlyWindowTop() {
	return (this->topLeftXY.y < Player::MAP_BUFFER);
}

bool Player::IsStayingNearlyWindowRight() {
	return (this->topLeftXY.x + CharacterCommon::WIDTH + Player::MAP_BUFFER >= AppCommon::GetWindowWidth());
}

bool Player::IsStayingNearlyWindowBottom() {
	return (this->topLeftXY.y + CharacterCommon::HEIGHT + Player::MAP_BUFFER >= AppCommon::GetWindowHeight());
}

bool Player::IsStayingNearlyWindowLeft() {
	return (this->topLeftXY.x < Player::MAP_BUFFER);
}

Vertices<POINT> Player::GetPlayerXY() {
	Vertices<POINT> ret = CharacterCommon::GetChipXY(this->topLeftXY);
	int xDiff = (CharacterCommon::WIDTH - Player::PLAYER_WIDTH) / 2;
	int yDiff = (CharacterCommon::HEIGHT - Player::PLAYER_HEIGHT) / 2;
	ret.topLeft.x += xDiff;
	ret.topLeft.y += yDiff;
	ret.bottomRight.x -= xDiff;
	ret.bottomRight.y -= yDiff;
	return ret;
}

Vertices<DrawingVertex> Player::GetVertex() {
	Vertices<DrawingVertex> ret;

	Vertices<FloatPoint> chip;
	int animationNum = CharacterCommon::GetAnimationNumber(this->currentAnimationCnt);
	switch (this->headingDirection) {
		case CharacterCommon::Direction::TOP:
			chip = this->headingTopChips[animationNum];
			break;
		case CharacterCommon::Direction::RIGHT:
			chip = this->headingRightChips[animationNum];
			break;
		case CharacterCommon::Direction::BOTTOM:
			chip = this->headingBottomChips[animationNum];
			break;
		case CharacterCommon::Direction::LEFT:
			chip = this->headingLeftChips[animationNum];
			break;
	}

	ret = CharacterCommon::GetVertex(this->topLeftXY, &CharacterCommon::GetChipXY, chip);
	return ret;
}

void Player::Walk(CharacterCommon::Direction nextDir) {
	if (nextDir != this->headingDirection) {
		// 向きが変わったらアニメーション番号を最初に戻す
		this->currentAnimationCnt = 0;
	} else {
		CharacterCommon::CountUpAnimationCnt(&this->currentAnimationCnt, Player::CHIP_COUNT_PER_DIRECTION);
	}
	this->headingDirection = nextDir;
}

}
}