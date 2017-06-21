#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "SceneController.h"
#include "StageController.h"
#include "Stage/Stage1.h"
#include "Drawing/Drawer.h"


namespace BestStealReplica {

/* Constructor / Destructor ------------------------------------------------------------------------- */
SceneController::SceneController() : pStage(nullptr), pStageController(new StageController()), blackoutFrameCount(0) {}

void SceneController::Release() {
	if (this->pStage != nullptr) {
		delete this->pStage;
		this->pStage = nullptr;
	}
	delete this->pStageController;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void SceneController::StartStage() {
	this->pStage = new Stage::Stage1();
	this->pStageController->LoadStage(*this->pStage);
	AppCommon::SetScene(AppCommon::SceneType::DRAWING_MAP);
}

void SceneController::Control(LPDIRECTINPUTDEVICE8 pDIDevice) {
	switch (AppCommon::GetScene()) {
		case AppCommon::SceneType::DRAWING_MAP:
		{
			AppCommon::Key key;
			ProcessKBInput(pDIDevice, &key);
			this->pStageController->Control(key);
			if (AppCommon::GetScene() == AppCommon::SceneType::DRAWING_MAP) {
				Drawing::Drawer::Draw();
			}
			break;
		}
		case AppCommon::SceneType::BLACKOUT:
			Drawing::Drawer::Blackout();
			++this->blackoutFrameCount;
			if (blackoutFrameCount > 15) {
				AppCommon::SetScene(AppCommon::SceneType::DRAWING_MAP);
				this->blackoutFrameCount = 0;
			}
			break;
	}
}


/* Private Functions  ------------------------------------------------------------------------------- */
void SceneController::ProcessKBInput(const LPDIRECTINPUTDEVICE8 pDIDevice, AppCommon::Key* pRet) const {
	#define KEYDOWN(name, key) (name[key] & 0x80) > 0

	char     buffer[256];
	HRESULT  hr;
	hr = pDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
	if FAILED(hr) {
		// If it failed, the device has probably been lost. 
		// Check for (hr == DIERR_INPUTLOST) 
		// and attempt to reacquire it here. 
		return;
	}

	pRet->z = KEYDOWN(buffer, DIK_Z);
	pRet->x = KEYDOWN(buffer, DIK_X);
	pRet->right = KEYDOWN(buffer, DIK_RIGHT);
	pRet->left = KEYDOWN(buffer, DIK_LEFT);
	pRet->up = KEYDOWN(buffer, DIK_UP);
	pRet->down = KEYDOWN(buffer, DIK_DOWN);
	pRet->shift = (KEYDOWN(buffer, DIK_LSHIFT) || KEYDOWN(buffer, DIK_RSHIFT));
}

}