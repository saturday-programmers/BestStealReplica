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
	/**
	 * キー操作コマンドを示す構造体
	 * 
	 * 当構造体はコピー可能とする。
	 */
	struct Handling {
		enum struct HandlingType {
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
	StageController();
	StageController(const StageController&) = delete;
	~StageController();

	/* Operator Overloads ------------------------------------------------------------------------------- */
	StageController& operator=(const StageController&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	void LoadStage(const Stage::IStage& rStage);
	void Control(const AppCommon::Key& rKey);

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int MOVING_PIXEL_ON_RUNNING = 5;
	static const int MOVING_PIXEL_ON_WALKING = 3;

	/* Variables ---------------------------------------------------------------------------------------- */
	const Stage::IStage* pStage;
	Map::Map* pMap;
	Character::Player* pPlayer;
	Character::Enemy* pEnemy;
	StageController::Handling lastTimeHandling;

	/* Functions ---------------------------------------------------------------------------------------- */
	void ConvertKeyToHandling(const AppCommon::Key& rKey, StageController::Handling* pRet) const;
	int ControlPlayer(StageController::Handling* pHandling);
	void ControlEnemy(const StageController::Handling& rHandling);
	void ControlMap(int playerMovingPixel);
	void MoveMap(int playerMovingPixel);
	void RevertStage();

};

}

#endif
