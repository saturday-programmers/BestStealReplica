#include <math.h>

#include "Stone.h"
#include "../Drawing/Drawer.h"
#include "../Map/MapChip.h"


namespace BestStealReplica {
namespace Map {


/* Constructor / Destructor ------------------------------------------------------------------------- */
Stone::Stone(const POINT& rTopLeftPoint, AppCommon::Direction direction) : direction(direction), thrownElapsedCount(0), state(Stone::State::BEING_THROWN) {
	this->defaultTopLeftPoint.x = rTopLeftPoint.x;
	this->defaultTopLeftPoint.y = rTopLeftPoint.y;

	MapChip::ConvertChipNumberToTexRect(Stone::MAP_CHIP_NUMBER, &this->texRect);
	// 枠線をなくすために一回り小さくする
	this->texRect.topLeft.x += 0.01f;
	this->texRect.topLeft.y += 0.01f;
	this->texRect.bottomRight.x -= 0.01f;
	this->texRect.bottomRight.y -= 0.01f;
}


/* Getters / Setters -------------------------------------------------------------------------------- */	
void Stone::SetTopLeftPoint(const POINT& rPoint) {
	this->topLeftPoint.x = rPoint.x;
	this->topLeftPoint.y = rPoint.y;
	switch (this->direction) {
		case AppCommon::Direction::TOP:
		case AppCommon::Direction::BOTTOM:
			this->topLeftPointOnGnd.y = rPoint.y;
			break;
		case AppCommon::Direction::RIGHT:
		case AppCommon::Direction::LEFT:
			this->topLeftPointOnGnd.x = rPoint.x;
	}
}

Stone::State Stone::GetState() const {
	return this->state;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void Stone::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const {
	Drawing::DrawingContext context;
	context.textureType = Drawing::TextureType::MAP;
	CreateDrawingVertexRect(&context.rect);
	switch (this->state) {
		case Stone::State::BEING_THROWN:
		case Stone::State::DROPPED:
			pDrawingContexts->push_back(context);
			break;
		case Stone::State::DISAPPEARING:
			context.alpha = Drawing::Drawer::GetAlphaOnBlinking(this->restTime);
			pDrawingContexts->push_back(context);
			break;
		default:
			break;
	}
}

void Stone::KeepBeingThrown() {
	/*
	 *  放物線の位置の求め方
	 *
	 *  *** X座標 ***
	 *  Xt = V0 * T ... 速度V0のときのT秒後の移動距離（等速度運動）
	 *
	 *  *** Y座標 ***
	 *  Yt = V0 * T - 1/2 * G * T^2 ... 初速V0のときのT秒後の移動距離
	 */ 

	switch (this->state) {
		case Stone::State::BEING_THROWN:
			if (thrownElapsedCount == 0) {
				this->topLeftPoint = this->defaultTopLeftPoint;
				this->topLeftPointOnGnd = this->defaultTopLeftPoint;
			} else {
				int velocity = Stone::INITIAL_VELOCITY;
				int g = Stone::GRAVITY;
				int distance = Stone::INITIAL_VELOCITY * this->thrownElapsedCount;
				int height = Stone::INITIAL_VELOCITY * this->thrownElapsedCount - (Stone::GRAVITY * (int)pow(double(this->thrownElapsedCount), 2) / 2);

				switch (this->direction) {
					case AppCommon::Direction::TOP:
					{
						this->topLeftPointOnGnd.y = this->defaultTopLeftPoint.y - distance;
						float r = 100.0f / distance;	// 100:カメラからスクリーンへの距離
						// 透視図の計算 y’ = d / y * zをアレンジ
						this->topLeftPoint.y = this->topLeftPointOnGnd.y - (int)(r * height);
						break;
					}
					case AppCommon::Direction::RIGHT:
						this->topLeftPoint.x = this->defaultTopLeftPoint.x + distance;
						this->topLeftPoint.y = this->defaultTopLeftPoint.y - height;
						this->topLeftPointOnGnd.x = this->topLeftPoint.x;
						break;
					case AppCommon::Direction::BOTTOM:
					{
						this->topLeftPointOnGnd.y = this->defaultTopLeftPoint.y + distance;
						float r = 50.0f / distance;	// 50:カメラからスクリーンへの距離
						// 透視図の計算 y’ = d / y * zをアレンジ
						this->topLeftPoint.y = this->topLeftPointOnGnd.y - (int)(r * height);
						break;
					}
					case AppCommon::Direction::LEFT:
						this->topLeftPoint.x = this->defaultTopLeftPoint.x - distance;
						this->topLeftPoint.y = this->defaultTopLeftPoint.y - height;
						this->topLeftPointOnGnd.x = this->topLeftPoint.x;
						break;
				}

				// 地面に達した場合
				if (height <= 0) {
					SetDropped();
				} 
			}
			++this->thrownElapsedCount;
			break;

		case Stone::State::DROPPED:
			--this->restTime;
			if (this->restTime == 0) {
				this->state = Stone::State::DISAPPEARING;
				this->restTime = Stone::DISAPPEARING_DURATION;
			}
			break;

		case Stone::State::DISAPPEARING:
			--this->restTime;
			if (this->restTime == 0) {
				this->state = Stone::State::DiSAPPEARED;
			}
			break;
	}
}

bool Stone::Exists() const {
	return (this->state != Stone::State::DiSAPPEARED);
}

void Stone::Move(const POINT& rPixel) {
	this->topLeftPoint.x += rPixel.x;
	this->topLeftPoint.y += rPixel.y;
}

void Stone::CalcRectOnGround(Rectangle<POINT>* pRet) const {
	pRet->topLeft = this->topLeftPointOnGnd;
	pRet->bottomRight.x = pRet->topLeft.x + Stone::WIDTH;
	pRet->bottomRight.y = pRet->topLeft.y + Stone::HEIGHT;
}

void Stone::SetDropped() {
	this->topLeftPoint = topLeftPointOnGnd;
	this->state = Stone::State::DROPPED;
	this->restTime = Stone::STAYING_DURATION;
}

void Stone::BackOnePixel() {
	switch (this->direction) {
		case AppCommon::TOP:
			this->topLeftPoint.y += 1;
			break;
		case AppCommon::Direction::RIGHT:
			this->topLeftPoint.x -= 1;
			break;
		case AppCommon::Direction::BOTTOM:
			this->topLeftPoint.y -= 1;
			break;
		case AppCommon::Direction::LEFT:
			this->topLeftPoint.x += 1;
			break;
	}
	this->topLeftPointOnGnd = this->topLeftPoint;
}


/* Private Functions  ------------------------------------------------------------------------------- */
void Stone::CreateDrawingVertexRect(Rectangle<Drawing::DrawingVertex>* pRet) const {
	Drawing::DrawingVertex* pRetArr[] = { &pRet->topLeft, &pRet->bottomRight };
	FloatPoint texPointArr[] = { this->texRect.topLeft, this->texRect.bottomRight };

	POINT bottomRightPoint;
	bottomRightPoint.x = this->topLeftPoint.x + Stone::WIDTH;
	bottomRightPoint.y = this->topLeftPoint.y + Stone::HEIGHT;
	POINT pointArr[] = { this->topLeftPoint, bottomRightPoint };

	for (int i = 0; i < 2; ++i) {
		pRetArr[i]->x = pointArr[i].x;
		pRetArr[i]->y = pointArr[i].y;
		pRetArr[i]->tu = texPointArr[i].x;
		pRetArr[i]->tv = texPointArr[i].y;
	}
}

}
}