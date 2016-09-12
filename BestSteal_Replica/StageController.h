#ifndef STAGE_CONTROLLER_H_
#define STAGE_CONTROLLER_H_

#include "AppCommon.h"

namespace BestStealReplica {
namespace Map {
class Map;
}

namespace Character{
class Player;
class Enermy;
}

namespace Stage {
class IStage;
}

class Drawer;

class StageController {
public:
	/* Structs ------------------------------------------------------------------------------------------ */
	struct Handling {
		enum HandlingType {
			NONE,
			UP,
			DOWN,
			RIGHT,
			LEFT,
			STEAL_OR_OPEN,
			THROW
		};

		HandlingType handlingType = HandlingType::NONE;
		bool isWalking = false;
	};

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	explicit StageController(Drawer* pDrawer);
	~StageController();

	/* Functions ---------------------------------------------------------------------------------------- */
	void LoadStage(const Stage::IStage* pStage);
	void Control(AppCommon::Key key);
	void Draw();

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MOVING_PIXEL_ON_RUNNING = 5;
	static const int MOVING_PIXEL_ON_WALKING = 3;

	/* Variables ---------------------------------------------------------------------------------------- */
	Drawer* pDrawer;
	const Stage::IStage* pStage;
	Map::Map* pMap;
	Character::Player* pPlayer;
	Character::Enermy* pEnermy;
	StageController::Handling lastTimeHandling;

	/* Functions ---------------------------------------------------------------------------------------- */
	StageController::Handling ConvertKeyToHandling(AppCommon::Key key);
	int ControlPlayer(StageController::Handling* pHandling);
	void ControlEnermy(int playerMovingPixel, StageController::Handling* pHandling);
	void ControlMap(int playerMovingPixel, StageController::Handling* pHandling);
	void MoveMap(int playerMovingPixel);
	void RevertStage(void);
};

}

#endif
