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
			None,
			Up,
			Down,
			Right,
			Left,
			Steal
		};

		KeyType keyType;
		bool isKWalking;
	};


	Controller(Drawer* pDrawer);
	void LoadStage(IStage* pStage);
	void Control(Key key);
	void Draw();
	void Release();

private:
	Drawer* pDrawer;
	IStage* pStage;
	Map::Map* pMap;
	Character::Player* pPlayer;
	Character::Enermy* pEnermy;
};
}

#endif
