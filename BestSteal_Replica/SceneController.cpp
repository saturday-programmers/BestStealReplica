#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "SceneController.h"
#include "StageController.h"
#include "Stage1.h"
#include "Drawer.h"


namespace BestStealReplica {

/* Constructor / Destructor ------------------------------------------------------------------------- */
SceneController::SceneController(Drawer* pDrawer) : pDrawer(pDrawer), blackoutFrameCount(0) {
	this->pStageController = new StageController(pDrawer);
}

void SceneController::Release() {
	delete this->pStage;
	delete this->pStageController;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void SceneController::StartStage() {
	this->pStage = new Stage::Stage1();
	this->pStageController->LoadStage(pStage);
	AppCommon::SetScene(AppCommon::SceneType::DRAWING_MAP);
}

void SceneController::Control(LPDIRECTINPUTDEVICE8 pDIDevice) {
	switch (AppCommon::GetScene()) {
		case AppCommon::SceneType::DRAWING_MAP:
		{
			AppCommon::Key key = ProcessKBInput(pDIDevice);
			this->pStageController->Control(key);
			if (AppCommon::GetScene() == AppCommon::SceneType::DRAWING_MAP) {
				this->pStageController->Draw();
			}
			break;
		}
		case AppCommon::SceneType::BLACKOUT:
			this->pDrawer->Blackout();
			++this->blackoutFrameCount;
			if (blackoutFrameCount > 15) {
				AppCommon::SetScene(AppCommon::SceneType::DRAWING_MAP);
				this->blackoutFrameCount = 0;
			}
			break;
	}
}


/* Private Functions  ------------------------------------------------------------------------------- */
AppCommon::Key SceneController::ProcessKBInput(LPDIRECTINPUTDEVICE8 pDIDevice) {
	#define KEYDOWN(name, key) (name[key] & 0x80) 

	AppCommon::Key ret;
	char     buffer[256];
	HRESULT  hr;
	hr = pDIDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);
	if FAILED(hr) {
		// If it failed, the device has probably been lost. 
		// Check for (hr == DIERR_INPUTLOST) 
		// and attempt to reacquire it here. 
		return ret;
	}

	if (KEYDOWN(buffer, DIK_Z)) {
		ret.z = true;
	} 
	if (KEYDOWN(buffer, DIK_RIGHT)) {
		ret.right = true;
	} 
	if (KEYDOWN(buffer, DIK_LEFT)) {
		ret.left = true;;
	}  
	if (KEYDOWN(buffer, DIK_UP)) {
		ret.up = true;;
	} 
	if (KEYDOWN(buffer, DIK_DOWN)) {
		ret.down = true;;
	} 
	if (KEYDOWN(buffer, DIK_LSHIFT) || KEYDOWN(buffer, DIK_RSHIFT)) {
		ret.shift = true;
	}

	return ret;
}

}