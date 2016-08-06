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

	// �v���C���[�A�j���[�V����
	if (movingPoint.x != 0) {
		this->pPlayer->WalkX(movingPoint.x);
	} else if (movingPoint.y != 0) {
		this->pPlayer->WalkY(movingPoint.y);
	} else {
		this->pPlayer->Stay();
	}

	// �ړ��\�͈͊m�F
	if (movingPixel > 0) {
		Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
		if (!this->pMap->IsOnRoad(playerXY)) {
			// �ړ��s�\�ȏꏊ�Ɉړ������ꍇ�͌��̈ʒu�ɖ߂�
			POINT reversePoint;
			reversePoint.x = movingPoint.x * -1;
			reversePoint.y = movingPoint.y * -1;
			this->pPlayer->Move(reversePoint);
		}
	}

	// �G�A�j���[�W����
	this->pEnermy->Stay();

	// �G���v���C���[�𔭌�������
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	if (movingPixel == 0) {
		// �ړ����Ă��Ȃ��ꍇ�͑���/�����̏�Ԃ͑O��̏�Ԃ������p��
		key.isWalking = lastTimeKey.isWalking;
	}
	this->pEnermy->ScoutPlayer(playerXY, this->pStage->GetEnermySearchableRadius(), key.isWalking);

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