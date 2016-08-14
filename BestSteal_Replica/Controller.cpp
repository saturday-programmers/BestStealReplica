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
	// �}�b�v���
	this->pStage = new Stage1();
	this->pMap = new Map::Map(Stage1::Y_CHIP_COUNT, Stage1::X_CHIP_COUNT, this->pDrawer);
	this->pMap->Load(this->pStage);

	// �v���C���[���
	POINT playerChipPos = this->pStage->GetPlayerFirstChipPos();
	this->pPlayer = new Player(this->pMap->GetTopLeftXYonChip(playerChipPos), this->pDrawer);

	// �G���
	int enermyCount = this->pStage->GetEnermyCount();
	POINT enermiesXY[Enermy::MAX_ENERMY_COUNT];
	for (int i = 0; i < enermyCount; ++i) {
		enermiesXY[i] = this->pMap->GetTopLeftXYonChip(this->pStage->GetEnermyChipPos(i));
	}
	this->pEnermy = new Enermy(enermiesXY, this->pStage->GetEnermiesInfo(), this->pStage->GetEnermyCount(), this->pDrawer);
}

void Controller::Control(Key key) {
	// �v���C���[�A�j���[�V����
	int movingPixel = ControlPlayer(key);

	// �G�A�j���[�W����
	this->pEnermy->Stay();

	// ���ޏ���
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	this->pPlayer->holdingKeyCount += this->pEnermy->GetStolen(playerXY, this->pPlayer->isStealing);

	// �G���v���C���[�𔭌�������
	if (movingPixel == 0) {
		// �ړ����Ă��Ȃ��ꍇ�͑���/�����̏�Ԃ͑O��̏�Ԃ������p��
		key.isWalking = lastTimeKey.isWalking;
	}
	if (this->pPlayer->isStealing) {
		key.isWalking = true;
	}
	this->pEnermy->ScoutPlayer(playerXY, this->pStage->GetEnermySearchableRadius(), key.isWalking);

	// �h�A���J����A�j���[�V����
	this->pMap->KeepOpeningDoors();

	// �v���C���[���E�B���h�E�O�ɏo���O�̏ꍇ�̓}�b�v���ړ�������
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
			// �v���C���[�̖ڂ̑O�̃}�b�v�`�b�v�擾
			Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
			POINT frontMapChipPos = this->pMap->GetFrontMapChipPos(playerXY, this->pPlayer->headingDirection);
			bool isStartStealing = true;
			switch (this->pMap->GetMapChipType(frontMapChipPos)) {
				case MapCommon::MapChipType::DOOR:
				case MapCommon::MapChipType::GOLD_DOOR:
					if (!this->pMap->IsDoorOpened(frontMapChipPos)) {
						if (pPlayer->holdingKeyCount > 0) {
							// �h�A���J����
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
				// �ڂ̑O���J���Ă��Ȃ��h�A�łȂ��ꍇ�͓��ރA�N�V����
				this->pPlayer->StartStealing();
			}

		}
	}

	// ���ރA�N�V�����p��
	if (pPlayer->isStealing) {
		this->pPlayer->KeepStealing();
	}

	// �v���C���[�ړ�����
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

	// �v���C���[�ړ�����
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

	// �v���C���[�A�j���[�V����
	if (!this->pPlayer->isStealing) {
		if (movingPixel > 0) {
			this->pPlayer->Walk(movingPoint);
		} else {
			this->pPlayer->Stay();
		}
	}

	// �ړ��\�͈͊m�F
	if (movingPixel > 0) {
		POINT reversePoint;
		reversePoint.x = -1 * (movingPoint.x / movingPixel);
		reversePoint.y = -1 * (movingPoint.y / movingPixel);
		while (!this->pMap->IsOnRoad(this->pPlayer->GetPlayerXY())) {
			// �ړ��s�\�ȏꏊ�Ɉړ������ꍇ�͌��̈ʒu�ɖ߂�
			this->pPlayer->Move(reversePoint);
		}
	}

	return movingPixel;
}

void Controller::MoveMap(int playerMovingPixel) {
	POINT mapMovingPoint;
	mapMovingPoint.x = 0;
	mapMovingPoint.y = 0;

	// ��U�ړ������Ă݂�
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

	// �}�b�v�O���\������Ă��܂��ꍇ�͌��ɖ߂�
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