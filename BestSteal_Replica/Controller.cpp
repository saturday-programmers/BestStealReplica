#include "Controller.h"

#include "Stage1.h"
#include "Drawer.h"
#include "Map.h"
#include "Player.h"
#include "Enermy.h"

using namespace BestStealReplica::Map;
using namespace BestStealReplica::Character;


namespace BestStealReplica {

/* Constructor / Destructor ------------------------------------------------------------------------- */
Controller::Controller(Drawer* pDrawer) :
	pDrawer(pDrawer)
{}


/* Getters / Setters -------------------------------------------------------------------------------- */
Controller::State Controller::GetState() {
	return this->state;
}

void Controller::SetState(Controller::State state) {
	this->state = state;
}


/* Public Functions  -------------------------------------------------------------------------------- */
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

	this->state = Controller::State::DrawingMap;
}

void Controller::Control(Key key) {

	// �v���C���[���S
	Vertices<POINT> playerXY = this->pPlayer->GetPlayerXY();
	if (this->pEnermy->CanKillPlayer(playerXY)) {
		RevertStage();
		this->state = Controller::State::Blackout;
		return;
	}

	// �v���C���[�A�j���[�V����
	int movingPixel = ControlPlayer(key);

	// �G�A�j���[�W����
	ControlEnermy(movingPixel, &key);

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


/* Private Functions  ------------------------------------------------------------------------------- */
int Controller::ControlPlayer(Key key) {
	int movingPixel;

	if (key.keyType == Key::KeyType::STEAL_OR_OPEN) {
		if (!this->pPlayer->GetIsStealing()) {
			// �v���C���[�̖ڂ̑O�̃}�b�v�`�b�v�擾
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
							// �h�A���J����
							if (this->pMap->StartOpeningDoor(frontMapChipPos)) {
								--(*pKeyCount);
							}
						}
						canStartStealing = false;
					}
					break;
				}
				default:
					break;
			}

			if (canStartStealing) {
				// �ڂ̑O���J���Ă��Ȃ��h�A�ȊO�̏ꍇ�͓��ރA�N�V����
				this->pPlayer->StartStealing();
			}

		}
	}

	// ���ރA�N�V�����p��
	if (pPlayer->GetIsStealing()) {
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
	if (this->pPlayer->GetIsStealing()) {
		movingPixel = Player::MOVING_PIXEL_ON_STEALING;
	} else if (key.keyType == Key::KeyType::NONE || key.keyType == Key::KeyType::STEAL_OR_OPEN) {
		movingPixel = 0;
	} else {
		movingPixel = key.isWalking ? Controller::MOVING_PIXEL_ON_WALKING : Controller::MOVING_PIXEL_ON_RUNNING;
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

	// �v���C���[�A�j���[�V����
	if (!this->pPlayer->GetIsStealing()) {
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

void Controller::ControlEnermy(int playerMovingPixel, Key* pKey) {
	this->pEnermy->Stay();

	// ���ޏ���
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

	// �G���v���C���[�𔭌�������
	if (playerMovingPixel == 0) {
		// �ړ����Ă��Ȃ��ꍇ�͑���/�����̏�Ԃ͑O��̏�Ԃ������p��
		pKey->isWalking = lastTimeKey.isWalking;
	}
	if (this->pPlayer->GetIsStealing()) {
		pKey->isWalking = true;
	}
	this->pEnermy->ScoutPlayer(playerXY, this->pStage->GetEnermySearchableRadius(), pKey->isWalking);

	// �ːi�\��
	for (int i = 0; i < this->pStage->GetEnermyCount(); ++i) {
		// �v���C���[�Ƃ̋����`�F�b�N
		Vertices<POINT> enermyXY = this->pEnermy->GetEnermyXY(i);
		int biggerPos;
		int smallerPos;
		switch (this->pEnermy->GetHeadingDirection(i)) {
			case AppCommon::Direction::TOP:
				biggerPos = enermyXY.topLeft.y;
				smallerPos = playerXY.topLeft.y;
				break;
			case AppCommon::Direction::RIGHT:
				smallerPos = enermyXY.bottomRight.x;
				biggerPos = playerXY.bottomRight.x;
				break;
			case AppCommon::Direction::BOTTOM:
				smallerPos = enermyXY.bottomRight.y;
				biggerPos = playerXY.bottomRight.y;
				break;
			case AppCommon::Direction::LEFT:
				biggerPos = enermyXY.topLeft.x;
				smallerPos = playerXY.topLeft.x;
				break;
		}
		bool canSeePlayer = ((biggerPos >= smallerPos) && (biggerPos - smallerPos <= this->pStage->GetEnermySearchableRadius()));

		// �v���C���[�Ƃ̊Ԃɕǂ����邩
		canSeePlayer = canSeePlayer && !this->pMap->ExistsWallBetween(CharacterCommon::CalcCenter(enermyXY), CharacterCommon::CalcCenter(playerXY));

		// �ːi
		this->pEnermy->Attack(i, canSeePlayer);
	}
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

void Controller::RevertStage() {
	this->pMap->MoveToDefault();
	this->pPlayer->GetKilled();
	this->pEnermy->BackToDefaultPosition();
}


}