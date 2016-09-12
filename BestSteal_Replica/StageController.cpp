#include "StageController.h"

#include "Stage1.h"
#include "Drawer.h"
#include "Map.h"
#include "Player.h"
#include "Enermy.h"

using namespace BestStealReplica::Map;
using namespace BestStealReplica::Character;


namespace BestStealReplica {

/* Constructor / Destructor ------------------------------------------------------------------------- */
StageController::StageController(Drawer* pDrawer) :
	pDrawer(pDrawer)
{}

StageController::~StageController() {
	delete this->pEnermy;
	delete this->pPlayer;
	delete this->pMap;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void StageController::LoadStage(const Stage::IStage* pStage) {
	// マップ情報
	this->pStage = pStage;
	this->pMap = new Map::Map(this->pStage->GetYChipCount(), this->pStage->GetXChipCount(), this->pDrawer);
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

void StageController::Control(AppCommon::Key key) {
	StageController::Handling handling = ConvertKeyToHandling(key);

	// プレイヤー死亡
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	if (this->pEnermy->CanKillPlayer(playerXY)) {
		RevertStage();
		AppCommon::SetScene(AppCommon::SceneType::BLACKOUT);
		return;
	}

	// プレイヤーアニメーション
	int movingPixel = ControlPlayer(&handling);

	// 敵アニメージョン
	ControlEnermy(movingPixel, &handling);

	// マップアニメーション
	ControlMap(movingPixel, &handling);

	this->lastTimeHandling = handling;
}

void StageController::Draw() {
	this->pDrawer->BeginDraw();
	this->pMap->Draw();
	this->pPlayer->Draw();
	this->pEnermy->Draw();
	this->pDrawer->EndDraw();
}


/* Private Functions  ------------------------------------------------------------------------------- */
StageController::Handling StageController::ConvertKeyToHandling(AppCommon::Key key) {
	StageController::Handling ret;
	if (key.z) {
		ret.handlingType = StageController::Handling::HandlingType::STEAL_OR_OPEN;
	} else if (key.x) {
		ret.handlingType = StageController::Handling::HandlingType::THROW;
	} else {
		if (key.up) {
			ret.handlingType = StageController::Handling::HandlingType::UP;
		} else if (key.right) {
			ret.handlingType = StageController::Handling::HandlingType::RIGHT;
		} else if (key.down) {
			ret.handlingType = StageController::Handling::HandlingType::DOWN;
		} else if (key.left) {
			ret.handlingType = StageController::Handling::HandlingType::LEFT;
		} 
		if (ret.handlingType != StageController::Handling::HandlingType::NONE) {
			if (key.shift) {
				ret.isWalking = true;
			}
		}
	}
	return ret;
}

int StageController::ControlPlayer(Handling* pHandling) {
	int movingPixel;

	if (this->pPlayer->GetIsStealing()) {
		pHandling->handlingType = Handling::HandlingType::NONE;
	}

	if (pHandling->handlingType == Handling::HandlingType::STEAL_OR_OPEN) {
		// プレイヤーの目の前のマップチップ取得
		Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
		POINT frontMapChipPos = this->pMap->GetFrontMapChipPos(playerXY, this->pPlayer->GetHeadingDirection());
		bool canStartStealing = true;
		MapCommon::MapChipType mapChipType = this->pMap->GetMapChipType(frontMapChipPos);
		switch (mapChipType) {
			case MapCommon::MapChipType::DOOR:
			case MapCommon::MapChipType::GOLD_DOOR: {
				int* pKeyCount = (mapChipType == MapCommon::MapChipType::DOOR) ? &this->pPlayer->holdingSilverKeyCount : &this->pPlayer->holdingGoldKeyCount;
				if (!this->pMap->IsDoorOpened(frontMapChipPos)) {
					if (*pKeyCount > 0) {
						// ドアを開ける
						if (this->pMap->StartOpeningDoor(frontMapChipPos)) {
							--(*pKeyCount);
						}
					}
					canStartStealing = false;
				}
				break;
			}
			case MapCommon::MapChipType::JEWELRY:
				// 宝箱を開ける
				this->pMap->OpenJewelryBox();
				canStartStealing = false;
				break;
			default:
				break;
		}

		if (canStartStealing) {
			// 目の前が開いていないドア以外の場合は盗むアクション
			this->pPlayer->StartStealing();
		}
	} else if (pHandling->handlingType == Handling::HandlingType::THROW && pHandling->handlingType != this->lastTimeHandling.handlingType) {
		// 石を投げる
		Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
		this->pMap->AddStone(playerXY.topLeft, this->pPlayer->GetHeadingDirection());
	}

	// 盗むアクション継続
	if (this->pPlayer->GetIsStealing()) {
		this->pPlayer->KeepStealing();
	} else {
		// プレイヤー移動方向
		switch (pHandling->handlingType) {
			case Handling::HandlingType::RIGHT:
				this->pPlayer->SetDirection(AppCommon::Direction::RIGHT);
				break;
			case Handling::HandlingType::LEFT:
				this->pPlayer->SetDirection(AppCommon::Direction::LEFT);
				break;
			case Handling::HandlingType::UP:
				this->pPlayer->SetDirection(AppCommon::Direction::TOP);
				break;
			case Handling::HandlingType::DOWN:
				this->pPlayer->SetDirection(AppCommon::Direction::BOTTOM);
				break;
			default:
				break;
		}
	}

	// プレイヤー移動距離
	if (this->pPlayer->GetIsStealing()) {
		movingPixel = Player::MOVING_PIXEL_ON_STEALING;
	} else {
		switch (pHandling->handlingType) {
			case Handling::HandlingType::NONE:
			case Handling::HandlingType::STEAL_OR_OPEN:
			case Handling::HandlingType::THROW:
				movingPixel = 0;
				break;
			default:
				movingPixel = pHandling->isWalking ? StageController::MOVING_PIXEL_ON_WALKING : StageController::MOVING_PIXEL_ON_RUNNING;
				break;
		}
	}
	POINT movingPoint;
	movingPoint.x = 0;
	movingPoint.y = 0;
	switch (pPlayer->GetHeadingDirection()) {
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
	if (!this->pPlayer->GetIsStealing()) {
		if (movingPixel > 0) {
			this->pPlayer->Walk(movingPoint);
		} else {
			this->pPlayer->Stay();
		}
	}

	// 移動可能範囲確認
	if (movingPixel > 0) {
		POINT reversePoint;
		reversePoint.x = 0;
		reversePoint.y = 0;
		if (movingPoint.x != 0) {
			reversePoint.x = -1 * (movingPoint.x / movingPixel);
		} else if (movingPoint.y != 0) {
			reversePoint.y = -1 * (movingPoint.y / movingPixel);
		}
		while (!this->pMap->IsOnRoad(this->pPlayer->GetPlayerXY()) && movingPixel > 0) {
			// 移動不可能な場所に移動した場合は元の位置に戻す
			this->pPlayer->Move(reversePoint);
			--movingPixel;
		}
	}

	return movingPixel;
}

void StageController::ControlEnermy(int playerMovingPixel, Handling* pHandling) {
	this->pEnermy->Stay();

	// 盗まれる処理
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	Enermy::KeyType keyType = this->pEnermy->GetStolen(playerXY, this->pPlayer->GetIsStealing());
	switch (keyType) {
		case Enermy::KeyType::Silver:
			++this->pPlayer->holdingSilverKeyCount;
			break;
		case Enermy::KeyType::Gold:
			++this->pPlayer->holdingGoldKeyCount;
			break;
		default:
			break;
	}

	// 敵がプレイヤーを発見したか
	if (playerMovingPixel == 0) {
		// 移動していない場合は走る/歩くの状態は前回の状態を引き継ぐ
		pHandling->isWalking = lastTimeHandling.isWalking;
	}
	if (this->pPlayer->GetIsStealing()) {
		pHandling->isWalking = true;
	}
	this->pEnermy->ScoutPlayer(playerXY, this->pStage->GetEnermySearchableRadius(), pHandling->isWalking);

	// 突進可能か
	for (int i = 0; i < this->pStage->GetEnermyCount(); ++i) {
		if (this->pEnermy->GetState(i) == Enermy::State::GOT_STOLEN) {
			continue;
		}

		// プレイヤーとの距離チェック
		POINT centerPlayer = CharacterCommon::CalcCenter(playerXY);
		POINT playerPos = this->pMap->GetMapChipPos(centerPlayer);

		Vertices<POINT> enermyXY = this->pEnermy->GetEnermyXY(i);
		POINT centerEnermy = CharacterCommon::CalcCenter(enermyXY);
		POINT enermyPos = this->pMap->GetMapChipPos(centerEnermy);
		bool canSeePlayer = false;
		int distance;
		switch (this->pEnermy->GetHeadingDirection(i)) {
			case AppCommon::Direction::TOP:
				canSeePlayer = (enermyPos.x == playerPos.x && enermyPos.y >= playerPos.y);
				distance = centerEnermy.y - centerPlayer.y;
				break;
			case AppCommon::Direction::RIGHT:
				canSeePlayer = (enermyPos.y== playerPos.y && enermyPos.x <= playerPos.x);
				distance = centerPlayer.x - centerEnermy.x;
				break;
			case AppCommon::Direction::BOTTOM:
				canSeePlayer = (enermyPos.x == playerPos.x && enermyPos.y <= playerPos.y);
				distance = centerPlayer.y - centerEnermy.y;
				break;
			case AppCommon::Direction::LEFT:
				canSeePlayer = (enermyPos.y == playerPos.y && enermyPos.x >= playerPos.x);
				distance = centerEnermy.x - centerPlayer.x;
				break;
		}
		canSeePlayer = canSeePlayer && distance <= this->pStage->GetEnermySearchableRadius();

		// プレイヤーとの間に壁があるか
		canSeePlayer = canSeePlayer && !this->pMap->ExistsWallBetween(centerEnermy, centerPlayer);

		// 突進
		this->pEnermy->Attack(i, canSeePlayer);
	}
}

void StageController::ControlMap(int playerMovingPixel, StageController::Handling* pHandling) {
	// ドアを開けるアニメーション
	this->pMap->KeepOpeningDoors();

	// 石を投げるアニメーション
	this->pMap->AnimateStones();

	// プレイヤーがウィンドウ外に出る手前の場合はマップを移動させる
	if (playerMovingPixel > 0) {
		MoveMap(playerMovingPixel);
	}
}

void StageController::MoveMap(int playerMovingPixel) {
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

void StageController::RevertStage() {
	this->pMap->MoveToDefault();
	this->pPlayer->GetKilled();
	this->pEnermy->BackToDefaultPosition();
}

}