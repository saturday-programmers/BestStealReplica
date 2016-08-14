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
	// プレイヤーアニメーション
	int movingPixel = ControlPlayer(key);

	// 敵アニメージョン
	this->pEnermy->Stay();

	// 盗む処理
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	this->pPlayer->holdingKeyCount += this->pEnermy->GetStolen(playerXY, this->pPlayer->isStealing);

	// 敵がプレイヤーを発見したか
	if (movingPixel == 0) {
		// 移動していない場合は走る/歩くの状態は前回の状態を引き継ぐ
		key.isWalking = lastTimeKey.isWalking;
	}
	if (this->pPlayer->isStealing) {
		key.isWalking = true;
	}
	this->pEnermy->ScoutPlayer(playerXY, this->pStage->GetEnermySearchableRadius(), key.isWalking);

	// ドアを開けるアニメーション
	this->pMap->KeepOpeningDoors();

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


int Controller::ControlPlayer(Key key) {
	int movingPixel;

	if (key.keyType == Key::KeyType::STEAL_OR_OPEN) {
		if (!this->pPlayer->isStealing) {
			// プレイヤーの目の前のマップチップ取得
			Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
			POINT frontMapChipPos = this->pMap->GetFrontMapChipPos(playerXY, this->pPlayer->headingDirection);
			bool isStartStealing = true;
			switch (this->pMap->GetMapChipType(frontMapChipPos)) {
				case MapCommon::MapChipType::DOOR:
				case MapCommon::MapChipType::GOLD_DOOR:
					if (!this->pMap->IsDoorOpened(frontMapChipPos)) {
						if (pPlayer->holdingKeyCount > 0) {
							// ドアを開ける
							if (this->pMap->StartOpeningDoor(frontMapChipPos)) {
								--this->pPlayer->holdingKeyCount;
							}
						}
						isStartStealing = false;
					}
					break;
				default:
					break;
			}

			if (isStartStealing) {
				// 目の前が開いていないドアでない場合は盗むアクション
				this->pPlayer->StartStealing();
			}

		}
	}

	// 盗むアクション継続
	if (pPlayer->isStealing) {
		this->pPlayer->KeepStealing();
	}

	// プレイヤー移動方向
	switch (key.keyType) {
		case Key::KeyType::RIGHT:
			pPlayer->SetDirection(AppCommon::Direction::RIGHT);
			break;
		case Key::KeyType::LEFT:
			pPlayer->SetDirection(AppCommon::Direction::LEFT);
			break;
		case Key::KeyType::UP:
			pPlayer->SetDirection(AppCommon::Direction::TOP);
			break;
		case Key::KeyType::DOWN:
			pPlayer->SetDirection(AppCommon::Direction::BOTTOM);
			break;
		default:
			break;
	}

	// プレイヤー移動距離
	if (this->pPlayer->isStealing) {
		movingPixel = Player::MOVING_PIXEL_ON_STEALING;
	} else if (key.keyType == Key::KeyType::NONE || key.keyType == Key::KeyType::STEAL_OR_OPEN) {
		movingPixel = 0;
	} else {
		movingPixel = key.isWalking ? Controller::MOVING_PIXEL_ON_WALKING : Controller::MOVING_PIXEL_ON_RUNNING;
	}
	POINT movingPoint;
	movingPoint.x = 0;
	movingPoint.y = 0;
	switch (pPlayer->headingDirection) {
		case AppCommon::Direction::RIGHT:
			movingPoint.x = movingPixel;
			break;
		case AppCommon::Direction::LEFT:
			movingPoint.x = movingPixel * -1;
			break;
		case AppCommon::Direction::TOP:
			movingPoint.y = movingPixel * -1;
			break;
		case AppCommon::Direction::BOTTOM:
			movingPoint.y = movingPixel;
			break;
	}

	// プレイヤーアニメーション
	if (!this->pPlayer->isStealing) {
		if (movingPixel > 0) {
			this->pPlayer->Walk(movingPoint);
		} else {
			this->pPlayer->Stay();
		}
	}

	// 移動可能範囲確認
	if (movingPixel > 0) {
		POINT reversePoint;
		reversePoint.x = -1 * (movingPoint.x / movingPixel);
		reversePoint.y = -1 * (movingPoint.y / movingPixel);
		while (!this->pMap->IsOnRoad(this->pPlayer->GetPlayerXY())) {
			// 移動不可能な場所に移動した場合は元の位置に戻す
			this->pPlayer->Move(reversePoint);
		}
	}

	return movingPixel;
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