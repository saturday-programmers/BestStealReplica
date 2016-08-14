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

class Drawer;
class IStage;

class Controller {
public:
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


	Controller(Drawer* pDrawer);
	void LoadStage(const IStage* pStage);
	void Control(Key key);
	void Draw();
	void Release();

private:
	static const int MOVING_PIXEL_ON_RUNNING = 5;
	static const int MOVING_PIXEL_ON_WALKING = 3;

	Drawer* pDrawer;
	const IStage* pStage;
	Map::Map* pMap;
	Character::Player* pPlayer;
	Character::Enermy* pEnermy;
	Key lastTimeKey;

	int ControlPlayer(Key key);
	void MoveMap(int playerMovingPixel);
};

}

#endif
