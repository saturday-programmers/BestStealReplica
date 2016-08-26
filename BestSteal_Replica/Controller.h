#ifndef CONTROLLER_H_
#define CONTROLLER_H_

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

class Controller {
public:
	/* Structs ------------------------------------------------------------------------------------------ */
	struct Key {
		enum KeyType {
			NONE,
			UP,
			DOWN,
			RIGHT,
			LEFT,
			STEAL_OR_OPEN
		};

		KeyType keyType = KeyType::NONE;
		bool isWalking = false;
	};

	/* Enums -------------------------------------------------------------------------------------------- */
	enum State {
		DrawingMap,
		Blackout
	};

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	Controller(Drawer* pDrawer);

	/* Getters / Setters -------------------------------------------------------------------------------- */
	Controller::State GetState();
	void SetState(Controller::State state);

	/* Functions ---------------------------------------------------------------------------------------- */
	void LoadStage(const Stage::IStage* pStage);
	void Control(Key key);
	void Draw();
	void Release();

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
	Controller::State state;
	Key lastTimeKey;

	/* Functions ---------------------------------------------------------------------------------------- */
	int ControlPlayer(Key key);
	void ControlEnermy(int playerMovingPixel, Key* pKey);
	void MoveMap(int playerMovingPixel);
	void RevertStage(void);
};

}

#endif
