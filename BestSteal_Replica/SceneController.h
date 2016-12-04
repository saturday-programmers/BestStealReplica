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
	void Release();

	/* Functions ---------------------------------------------------------------------------------------- */
	void StartStage();
	void Control(LPDIRECTINPUTDEVICE8 pDIDevice);

private:
	/* Variables ---------------------------------------------------------------------------------------- */
	Stage::IStage* pStage;
	StageController* pStageController;
	int blackoutFrameCount;

	/* Functions ---------------------------------------------------------------------------------------- */
	AppCommon::Key ProcessKBInput(LPDIRECTINPUTDEVICE8 pDIDevice) const;
};

}

#endif
