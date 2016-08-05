#include "AppCommon.h"


namespace BestStealReplica {
namespace {
static int windowHeidht;
static int windowWidth;
}

FloatPoint::FloatPoint() {}

FloatPoint::FloatPoint(float x, float y) :
	x(x),
	y(y)
{}


void AppCommon::SetWindowHeight(int height) {
	windowHeidht = height;
}

void AppCommon::SetWindowWidth(int width) {
	windowWidth = width;
}

int AppCommon::GetWindowHeight() {
	return windowHeidht;
}

int AppCommon::GetWindowWidth() {
	return windowWidth;
}
}