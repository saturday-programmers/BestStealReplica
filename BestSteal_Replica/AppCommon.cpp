#include "AppCommon.h"


namespace BestStealReplica {
namespace {
static int windowHeidht;
static int windowWidth;
static AppCommon::SceneType scene;
}

/* Constructor / Destructor ------------------------------------------------------------------------- */
FloatPoint::FloatPoint(float x, float y) : x(x), y(y) {}


/* Getters / Setters -------------------------------------------------------------------------------- */
int AppCommon::GetWindowHeight() {
	return windowHeidht;
}

void AppCommon::SetWindowHeight(int height) {
	windowHeidht = height;
}

int AppCommon::GetWindowWidth() {
	return windowWidth;
}

void AppCommon::SetWindowWidth(int width) {
	windowWidth = width;
}

AppCommon::SceneType AppCommon::GetScene() {
	return scene;
}

void AppCommon::SetScene(AppCommon::SceneType argScene) {
	scene = argScene;
}

}