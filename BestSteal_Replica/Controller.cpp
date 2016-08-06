#include "Controller.h"

#include "Stage1.h"
#include "Drawer.h"
#include "Map.h"
#include "Player.h"
#include "Enermy.h"

using namespace BestStealReplica::Map;
using namespace BestStealReplica::Character;


namespace BestStealReplica {

Controller::Controller(Drawer* pDrawer) :
	pDrawer(pDrawer)
{}

void Controller::LoadStage(const IStage* pStage) {
	// マップ情報
	this->pStage = new Stage1();
	this->pMap = new Map::Map(Stage1::Y_CHIP_COUNT, Stage1::X_CHIP_COUNT, this->pDrawer);
	this->pMap->Load(this->pStage);

	// プレイヤー情報
	POINT playerChipPos = this->pStage->GetPlayerFirstChipPos();
	this->pPlayer = new Player(this->pMap->GetTopLeftXYonChip(playerChipPos), this->pDrawer);

	// 敵情報
	int enermyCount = this->pStage->GetEnermyCount();
	POINT enermiesXY[Enermy::MAX_ENERMY_COUNT];
	for (int i = 0; i < enermyCount; ++i) {
		enermiesXY[i] = this->pMap->GetTopLeftXYonChip(this->pStage->GetEnermyChipPos(i));
	}
	this->pEnermy = new Enermy(enermiesXY, this->pStage->GetEnermiesInfo(), this->pStage->GetEnermyCount(), this->pDrawer);
}

void Controller::Control(Key key) {
	POINT movingPoint;
	movingPoint.x = 0;
	movingPoint.y = 0;

	int movingPixel = key.isWalking ? Controller::MOVING_PIXEL_ON_WALKING : Controller::MOVING_PIXEL_ON_RUNNING;
	switch (key.keyType) {
		case Key::KeyType::Right:
			movingPoint.x = movingPixel;
			break;
		case Key::KeyType::Left:
			movingPoint.x = movingPixel * -1;
			break;
		case Key::KeyType::Up:
			movingPoint.y = movingPixel * -1;
			break;
		case Key::KeyType::Down:
			movingPoint.y = movingPixel;
			break;
		default:
			movingPixel = 0;
			break;
	}

	// プレイヤーアニメーション
	if (movingPoint.x != 0) {
		this->pPlayer->WalkX(movingPoint.x);
	} else if (movingPoint.y != 0) {
		this->pPlayer->WalkY(movingPoint.y);
	} else {
		this->pPlayer->Stay();
	}

	// 移動可能範囲確認
	if (movingPixel > 0) {
		Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
		if (!this->pMap->IsOnRoad(playerXY)) {
			// 移動不可能な場所に移動した場合は元の位置に戻す
			POINT reversePoint;
			reversePoint.x = movingPoint.x * -1;
			reversePoint.y = movingPoint.y * -1;
			this->pPlayer->Move(reversePoint);
		}
	}

	// 敵アニメージョン
	this->pEnermy->Stay();

	// 敵がプレイヤーを発見したか
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	if (movingPixel == 0) {
		// 移動していない場合は走る/歩くの状態は前回の状態を引き継ぐ
		key.isWalking = lastTimeKey.isWalking;
	}
	this->pEnermy->ScoutPlayer(playerXY, this->pStage->GetEnermySearchableRadius(), key.isWalking);

	// プレイヤーがウィンドウ外に出る手前の場合はマップを移動させる
	if (movingPixel > 0) {
		MoveMap(movingPixel);
	}

	this->lastTimeKey = key;
}

void Controller::Draw() {
	this->pDrawer->BeginDraw();
	this->pMap->Draw();
	this->pPlayer->Draw();
	this->pEnermy->Draw();
	this->pDrawer->EndDraw();
}

void Controller::Release() {
	delete this->pStage;
	delete this->pMap;
	delete this->pPlayer;
	delete this->pEnermy;
}


void Controller::MoveMap(int playerMovingPixel) {
	POINT mapMovingPoint;
	mapMovingPoint.x = 0;
	mapMovingPoint.y = 0;

	// 一旦移動させてみる
	if (this->pPlayer->IsStayingNearlyWindowTop()) {
		mapMovingPoint.y = playerMovingPixel;
	} else if (this->pPlayer->IsStayingNearlyWindowBottom()) {
		mapMovingPoint.y = playerMovingPixel * -1;
	}
	if (this->pPlayer->IsStayingNearlyWindowRight()) {
		mapMovingPoint.x = playerMovingPixel * -1;
	} else if (this->pPlayer->IsStayingNearlyWindowLeft()) {
		mapMovingPoint.x = playerMovingPixel;
	}

	// マップ外が表示されてしまう場合は元に戻す
	if (!this->pMap->IsMovableX(mapMovingPoint.x)) {
		mapMovingPoint.x = 0;
	}
	if (!this->pMap->IsMovableY(mapMovingPoint.y)) {
		mapMovingPoint.y = 0;
	}

	if (mapMovingPoint.x != 0 || mapMovingPoint.y != 0) {
		this->pMap->Move(mapMovingPoint);
		this->pPlayer->Move(mapMovingPoint);
		this->pEnermy->Move(mapMovingPoint);
	}
}

}