#ifndef STAGE_CONTROLLER_H_
#define STAGE_CONTROLLER_H_

#include "AppCommon.h"

namespace BestStealReplica {
namespace Map {
class Map;
}

namespace Character{
class Player;
class Enemy;
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
	void LoadStage(const Stage::IStage& rStage);
	void Control(AppCommon::Key key);
	void Draw() const;

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MOVING_PIXEL_ON_RUNNING = 5;
	static const int MOVING_PIXEL_ON_WALKING = 3;

	/* Variables ---------------------------------------------------------------------------------------- */
	Drawer* pDrawer;
	const Stage::IStage* pStage;
	Map::Map* pMap;
	Character::Player* pPlayer;
	Character::Enemy* pEnemy;
	StageController::Handling lastTimeHandling;

	/* Functions ---------------------------------------------------------------------------------------- */
	StageController::Handling ConvertKeyToHandling(AppCommon::Key key) const;
	int ControlPlayer(StageController::Handling* pHandling);
	void ControlEnemy(int playerMovingPixel, const StageController::Handling& rHandling);
	void ControlMap(int playerMovingPixel);
	void MoveMap(int playerMovingPixel);
	void RevertStage();
};

}

#endif
