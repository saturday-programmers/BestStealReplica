﻿#include "Stage1.h"
#include "../AppCommon.h"
#include "../Character/Enemy.h"


namespace BestStealReplica {
namespace Stage {
namespace {

using Character::Enemy;

DataTable<Map::MapChipType> mapChipTypes = {
	// 1							2								3								4								5								6								7								8								9								10								11								12								13								14								15								16								17								18								19								20								21								22								23								24								25								26								27								28								29								30
	{ Map::MapChipType::ROAD,		Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD },		// 1
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD },		// 2
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD },		// 3
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD },		// 4
	{ Map::MapChipType::ROAD,		Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD },		// 5
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD },		// 6
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD },		// 7
	{ Map::MapChipType::ROAD,		Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL },		// 8
	{ Map::MapChipType::WALL,		Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::GATE,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE },	// 9
	{ Map::MapChipType::WALL,		Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD },		// 10
	{ Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::GATE,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL },		// 11
	{ Map::MapChipType::ROAD,		Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE },	// 12
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::JEWELRY,		Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD },		// 13
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL },		// 14
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL },		// 15
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::GOLD_GATE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL },		// 16
	{ Map::MapChipType::ROAD,		Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::GATE,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::GATE,			Map::MapChipType::WALL_SIDE },	// 17
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD },		// 18
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD },		// 19
	{ Map::MapChipType::ROAD,		Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD },		// 20
	{ Map::MapChipType::ROAD,		Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::WALL,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::WALL_SIDE,	Map::MapChipType::WALL_SIDE,	Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD,			Map::MapChipType::ROAD },		// 21
};

std::vector<Enemy::EnemyInfo> enemiesInfo = {
	Enemy::EnemyInfo(6, 4, AppCommon::Direction::RIGHT, AppCommon::GateKeyType::None),
	Enemy::EnemyInfo(24, 0, AppCommon::Direction::LEFT, AppCommon::GateKeyType::Silver),
	Enemy::EnemyInfo(1, 20, AppCommon::Direction::RIGHT, AppCommon::GateKeyType::Silver),
	Enemy::EnemyInfo(22, 7, AppCommon::Direction::LEFT, AppCommon::GateKeyType::None),
	Enemy::EnemyInfo(3, 16, AppCommon::Direction::BOTTOM, AppCommon::GateKeyType::Silver),
	Enemy::EnemyInfo(9, 20, AppCommon::Direction::RIGHT, AppCommon::GateKeyType::Silver),
	Enemy::EnemyInfo(21, 14, AppCommon::Direction::RIGHT, AppCommon::GateKeyType::Gold)
};

}

/* Getters / Setters -------------------------------------------------------------------------------- */
int Stage1::GetYChipCount() const {
	return mapChipTypes.size();
}

int Stage1::GetXChipCount() const {
	return mapChipTypes[0].size();
}

Map::MapChipType Stage1::GetMapChipType(int y, int x) const {
	return mapChipTypes[y][x];
}

void Stage1::GetPlayerFirstChipPos(POINT* pRet) const {
	pRet->x = 14;
	pRet->y = 20;
}

std::vector<Enemy::EnemyInfo> Stage1::GetEnemiesInfo() const {
	return enemiesInfo;
}

int Stage1::GetEnemyScoutableRadius() const {
	return 260;
}

int Stage1::GetMaxStoneCount() const {
	return MAX_STONE_COUNT;
}

}
}