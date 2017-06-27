#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "AppCommon.h"


//LPDIRECTINPUTDEVICE8;
typedef struct IDirectInputDevice8 *LPDIRECTINPUTDEVICE8;

namespace BestStealReplica {
class StageController;

namespace Drawing {
class Drawer;
}

namespace Stage {
class IStage;
}

class SceneController {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	SceneController();
	SceneController(const SceneController&) = delete;
	void Release();

	/* Operator Overloads ------------------------------------------------------------------------------- */
	SceneController& operator=(const SceneController&) = delete;

	/* Functions ---------------------------------------------------------------------------------------- */
	void StartStage();
	void Control(LPDIRECTINPUTDEVICE8 pDIDevice);

private:
	/* Variables ---------------------------------------------------------------------------------------- */
	Stage::IStage* pStage;
	StageController* pStageController;
	int blackoutFrameCount;

	/* Functions ---------------------------------------------------------------------------------------- */
	void ProcessKBInput(const LPDIRECTINPUTDEVICE8 pDIDevice, AppCommon::Key* pRet) const;

};

}

#endif
