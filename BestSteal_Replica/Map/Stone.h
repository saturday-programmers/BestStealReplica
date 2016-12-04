#ifndef STONE_H_
#define STONE_H_

#include <windows.h>

#include "../AppCommon.h"
#include "../Drawing/DrawingCommon.h"


namespace BestStealReplica {
namespace Map {

class Stone {
public:
	/* Enums -------------------------------------------------------------------------------------------- */
	enum State {
		BEING_THROWN,
		DROPPED,
		DISAPPEARING,
		DiSAPPEARED
	};

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Stone(POINT topLeftXY, AppCommon::Direction direction);

	/* Getters / Setters -------------------------------------------------------------------------------- */
	void SetTopLeftXY(POINT xy);
	State GetState() const;

	/* Functions ---------------------------------------------------------------------------------------- */
	void Stone::CreateDrawingContexts(std::vector<Drawing::DrawingContext>* pDrawingContexts) const;
	void KeepBeingThrown();
	bool Exists() const;
	void Move(POINT xy);
	Vertices<POINT> GetXYsOnGround() const;
	void SetDropped();
	void BackOnePixcel();

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
	POINT defaultTopleftXY;
	POINT topLeftXY;
	AppCommon::Direction direction;
	Vertices<FloatPoint> tutv;
	int thrownElapsedCount;
	Stone::State state;
	int restTime;
	POINT topLeftXYOnGnd;

	/* Functions ---------------------------------------------------------------------------------------- */
	Vertices<Drawing::DrawingVertex> CreateVertex() const;

};

}
}

#endif