﻿#ifndef STONE_H_
#define STONE_H_

#include "AppCommon.h"
#include "Drawing/DrawingCommon.h"


namespace BestStealReplica {
namespace Map {

class Stone {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum struct State {
		BEING_THROWN,
		DROPPED,
		DISAPPEARING,
		DiSAPPEARED
	};

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Stone(const POINT& rTopLeftPoint, AppCommon::Direction direction);
	Stone(const Stone&) = delete;

	/* Getters / Setters -------------------------------------------------------------------------------- */
	void SetTopLeftPoint(const POINT& rPoint);
	State GetState() const;

	/* Operator Overloads ------------------------------------------------------------------------------- */
	Stone& operator=(const Stone&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	void CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const;
	void KeepBeingThrown();
	bool Exists() const;
	void Move(const POINT& rPixel);
	void CalcRectOnGround(Rectangle<POINT>* pRet) const;
	void SetDropped();
	void BackOnePixel();

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MAP_CHIP_NUMBER = 54;
	static const int HEIGHT = 40;
	static const int WIDTH = 40;
	static const int THROWN_RADIAN = 30;
	static const int THROWN_DISTANCE = 24;
	static const int THROWN_HIGHT = 50;
	static const int STAYING_DURATION = 120;
	static const int DISAPPEARING_DURATION = 60;
	static const int INITIAL_VELOCITY = 16;
	static const int GRAVITY = 2;

	/* Variables ---------------------------------------------------------------------------------------- */
	POINT defaultTopLeftPoint;
	POINT topLeftPoint;
	AppCommon::Direction direction;
	Rectangle<Point<float>> texRect;
	int thrownElapsedCount;
	Stone::State state;
	int restTime;
	POINT topLeftPointOnGnd;

	/* Functions ---------------------------------------------------------------------------------------- */
	void CreateDrawingVertexRect(Rectangle<Drawing::DrawingVertex>* pRet) const;

};

}
}

#endif