#include <math.h>

#include "Stone.h"
#include "Drawer.h"
#include "MapChip.h"


namespace BestStealReplica {
namespace Map {


/* Constructor / Destructor ------------------------------------------------------------------------- */
Stone::Stone(Drawer* pDrawer, POINT topLeftXY, AppCommon::Direction direction) : pDrawer(pDrawer), defaultTopleftXY(topLeftXY), direction(direction), thrownElapsedCount(0), state(Stone::State::BEING_THROWN) {
	this->tutv = MapChip::GetTuTvs(Stone::MAP_CHIP_NUMBER);
}


/* Getters / Setters -------------------------------------------------------------------------------- */
void Stone::SetTopLeftXY(POINT xy) {
	this->topLeftXY = xy;
}


/* Public Functions  -------------------------------------------------------------------------------- */
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
				this->topLeftXY = this->defaultTopleftXY;
			} else {
				int velocity = Stone::INITIAL_VELOCITY;
				int g = Stone::GRAVITY;
				int distance = Stone::INITIAL_VELOCITY * this->thrownElapsedCount;
				int height = Stone::INITIAL_VELOCITY * this->thrownElapsedCount - (Stone::GRAVITY * (int)pow(double(this->thrownElapsedCount), 2) / 2);
				bool isDropped = false;

				switch (this->direction) {
					case AppCommon::Direction::TOP:
						this->topLeftXY.x = this->defaultTopleftXY.x - (height / 2);
						this->topLeftXY.y = this->defaultTopleftXY.y - distance;
						isDropped = (this->topLeftXY.x >= this->defaultTopleftXY.x);
						break;
					case AppCommon::Direction::RIGHT:
						this->topLeftXY.x = this->defaultTopleftXY.x + distance;
						this->topLeftXY.y = this->defaultTopleftXY.y - height;
						isDropped = (this->topLeftXY.y >= this->defaultTopleftXY.y);
						break;
					case AppCommon::Direction::BOTTOM:
						this->topLeftXY.x = this->defaultTopleftXY.x - (height / 2);
						this->topLeftXY.y = this->defaultTopleftXY.y + distance;
						isDropped = (this->topLeftXY.x >= this->defaultTopleftXY.x);
						break;
					case AppCommon::Direction::LEFT:
						this->topLeftXY.x = this->defaultTopleftXY.x - distance;
						this->topLeftXY.y = this->defaultTopleftXY.y - height;
						isDropped = (this->topLeftXY.y >= this->defaultTopleftXY.y);
						break;
				}

				// 地面に達した場合
				if (isDropped) {
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

void Stone::Draw() {
	Vertices<DrawingVertex> vertex = GetVertex();
	switch (this->state) {
		case Stone::State::BEING_THROWN:
		case Stone::State::DROPPED:
			this->pDrawer->Draw(vertex, Drawer::TextureType::MAP);
			break;
		case Stone::State::DISAPPEARING:
		{
			UINT16 alpha = Drawer::GetAlphaOnBlinking(this->restTime);
			this->pDrawer->Draw(vertex, Drawer::TextureType::MAP, alpha);
			break;
		}
		default:
			break;
	}
}

bool Stone::Exists() {
	return (this->state != Stone::State::DiSAPPEARED);
}

void Stone::Move(POINT xy) {
	this->topLeftXY.x += xy.x;
	this->topLeftXY.y += xy.y;
}

Vertices<POINT> Stone::GetXYsOnGround() {
	Vertices<POINT> ret;
	ret.topLeft = this->topLeftXY;

	if (this->state == Stone::State::BEING_THROWN) {
		switch (this->direction) {
			case AppCommon::Direction::TOP:
			case AppCommon::Direction::BOTTOM:
				ret.topLeft.x = this->defaultTopleftXY.x;
				break;
			case AppCommon::Direction::RIGHT:
			case AppCommon::Direction::LEFT:
				ret.topLeft.y = this->defaultTopleftXY.y;
				break;
		}
	}
	ret.bottomRight.x = this->topLeftXY.x + Stone::WIDTH;
	ret.bottomRight.y = this->topLeftXY.y + Stone::HEIGHT;

	return ret;
}

void Stone::SetDropped() {
	this->state = Stone::State::DROPPED;
	this->restTime = Stone::STAYING_DURATION;
}


/* Private Functions  ------------------------------------------------------------------------------- */
Vertices<DrawingVertex> Stone::GetVertex() {
	Vertices<DrawingVertex> ret;

	DrawingVertex* pVerticesArr[] = { &ret.topLeft, &ret.bottomRight };
	FloatPoint tutv[] = { this->tutv.topLeft, this->tutv.bottomRight };

	POINT bottomRightXY;
	bottomRightXY.x = this->topLeftXY.x + Stone::WIDTH;
	bottomRightXY.y = this->topLeftXY.y + Stone::HEIGHT;
	POINT xyArr[] = { this->topLeftXY, bottomRightXY };

	for (int i = 0; i < 2; ++i) {
		pVerticesArr[i]->x = xyArr[i].x;
		pVerticesArr[i]->y = xyArr[i].y;
		pVerticesArr[i]->tu = tutv[i].x;
		pVerticesArr[i]->tv = tutv[i].y;
	}
	return ret;
}

}
}