#include "AppCommon.h"


namespace BestStealReplica {
namespace {
static int windowHeidht;
static int windowWidth;
static AppCommon::GameState gameState;
}

/* Constructor / Destructor ------------------------------------------------------------------------- */
FloatPoint::FloatPoint() {}

FloatPoint::FloatPoint(float x, float y) :
	x(x),
	y(y)
{}


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

AppCommon::GameState AppCommon::GetGameState() {
	return gameState;
}

void AppCommon::SetGameState(AppCommon::GameState state) {
	gameState = state;
}

}