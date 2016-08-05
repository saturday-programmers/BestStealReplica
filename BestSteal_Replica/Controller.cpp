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
	POINT movePoint;
	movePoint.x = 0;
	movePoint.y = 0;

	switch (key.keyType) {
		case Key::KeyType::Right:
			movePoint.x = 5;
			break;
		case Key::KeyType::Left:
			movePoint.x = -5;
			break;
		case Key::KeyType::Up:
			movePoint.y = -5;
			break;
		case Key::KeyType::Down:
			movePoint.y = 5;
			break;
		default:
			break;
	}

	// �v���C���[�A�j���[�V����
	if (movePoint.x != 0) {
		this->pPlayer->WalkX(movePoint.x);
	} else if (movePoint.y != 0) {
		this->pPlayer->WalkY(movePoint.y);
	} else {
		this->pPlayer->Stay();
	}

	// �G�A�j���[�W����
	this->pEnermy->Stay();

	// �ړ��\�͈͊m�F
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	if (!this->pMap->IsOnRoad(playerXY)) {
		// �ړ��s�\�ȏꏊ�Ɉړ������ꍇ�͌��̈ʒu�ɖ߂�
		POINT reversePoint;
		reversePoint.x = movePoint.x * -1;
		reversePoint.y = movePoint.y * -1;
		this->pPlayer->Move(reversePoint);
	}

	// �G���v���C���[�𔭌�������
	playerXY = this->pPlayer->GetPlayerXY();
	this->pEnermy->ScoutPlayer(playerXY, this->pStage->GetEnermySearchableRadius());

	// �v���C���[���E�B���h�E�O�ɏo���O�̏ꍇ�̓}�b�v���ړ�������
	POINT mapMovePoint;
	mapMovePoint.x = 0;
	mapMovePoint.y = 0;
	if (this->pPlayer->IsStayingNearlyWindowTop()) {
		mapMovePoint.y = 5;
	} else if (this->pPlayer->IsStayingNearlyWindowBottom()) {
		mapMovePoint.y = -5;
	}
	if (this->pPlayer->IsStayingNearlyWindowRight()) {
		mapMovePoint.x = -5;
	} else if (this->pPlayer->IsStayingNearlyWindowLeft()) {
		mapMovePoint.x = 5;
	}
	// �}�b�v�O���\������Ă��܂��ꍇ�̓}�b�v�ړ��͂��Ȃ�
	if (!this->pMap->IsMovableX(mapMovePoint.x)) {
		mapMovePoint.x = 0;
	}
	if (!this->pMap->IsMovableY(mapMovePoint.y)) {
		mapMovePoint.y = 0;
	}

	if (mapMovePoint.x != 0 || mapMovePoint.y != 0) {
		this->pMap->Move(mapMovePoint);
		this->pPlayer->Move(mapMovePoint);
		this->pEnermy->Move(mapMovePoint);
	}
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

}