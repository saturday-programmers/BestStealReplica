#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "AppCommon.h"

//LPDIRECTINPUTDEVICE8;
typedef struct IDirectInputDevice8 *LPDIRECTINPUTDEVICE8;

namespace BestStealReplica {

class Drawer;
class StageController;

namespace Stage {
class IStage;
}

class SceneController {
public:
	/* Constructor / Destructor ------------------------------------------------------------------------- */
	explicit SceneController(Drawer* pDrawer);
	void Release();

	/* Functions ---------------------------------------------------------------------------------------- */
	void StartStage();
	void Control(LPDIRECTINPUTDEVICE8 pDIDevice);

private:
	/* Variables ---------------------------------------------------------------------------------------- */
	Drawer* pDrawer;
	Stage::IStage* pStage;
	StageController* pStageController;
	int blackoutFrameCount;

	/* Functions ---------------------------------------------------------------------------------------- */
	AppCommon::Key ProcessKBInput(LPDIRECTINPUTDEVICE8 pDIDevice);
};

}

#endif
