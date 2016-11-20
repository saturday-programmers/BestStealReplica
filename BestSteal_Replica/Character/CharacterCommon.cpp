#include "CharacterCommon.h"

namespace BestStealReplica {
namespace Character {

/* Static Public Functions -------------------------------------------------------------------------- */
void CharacterCommon::SetTuTvs(Vertices<FloatPoint> chips[], int chipCntPerDir, int rowNum, int colNum) {
	for (int i = 0; i < chipCntPerDir; ++i) {
		chips[i] = GetTuTv(rowNum, colNum + i);
	}
}

Vertices<FloatPoint> CharacterCommon::GetTuTv(int rowNum, int colNum) {
	Vertices<FloatPoint> ret;
	ret.topLeft.x = colNum / (float)CharacterCommon::CHIP_COUNT_PER_COL;
	ret.topLeft.y = rowNum / (float)CharacterCommon::CHIP_COUNT_PER_ROW;
	ret.bottomRight.x = (colNum + 1) / (float)CharacterCommon::CHIP_COUNT_PER_COL;
	ret.bottomRight.y = (rowNum + 1) / (float)CharacterCommon::CHIP_COUNT_PER_ROW;
	return ret;
}

void CharacterCommon::CountUpAnimationCnt(int* currentAnimationCnt, int chipCntPerDir) {
	++(*currentAnimationCnt);
	if (*currentAnimationCnt >= chipCntPerDir * AppCommon::FRAME_COUNT_PER_CUT) {
		*currentAnimationCnt = 0;
	}
}

int CharacterCommon::GetAnimationNumber(int currentAnimationCnt) {
	return currentAnimationCnt / AppCommon::FRAME_COUNT_PER_CUT;
}

Vertices<DrawingVertex> CharacterCommon::GetVertex(POINT topLeftXY, Vertices<POINT>(*getXY)(POINT), Vertices<FloatPoint> chip) {
	Vertices<DrawingVertex> ret;
	Vertices<POINT> xy = getXY(topLeftXY);

	ret.topLeft.x = xy.topLeft.x;
	ret.topLeft.y = xy.topLeft.y;
	ret.topLeft.tu = chip.topLeft.x;
	ret.topLeft.tv = chip.topLeft.y;

	ret.bottomRight.x = xy.bottomRight.x;
	ret.bottomRight.y = xy.bottomRight.y;
	ret.bottomRight.tu = chip.bottomRight.x;
	ret.bottomRight.tv = chip.bottomRight.y;

	return ret;
}

Vertices<POINT> CharacterCommon::GetChipXY(POINT topLeftXY) {
	Vertices<POINT> ret;
	ret.topLeft.x = topLeftXY.x;
	ret.topLeft.y = topLeftXY.y;
	ret.bottomRight.x = topLeftXY.x + CharacterCommon::WIDTH - 1;
	ret.bottomRight.y = topLeftXY.y + CharacterCommon::HEIGHT - 1;
	return ret;
}

POINT CharacterCommon::CalcCenter(Vertices<POINT> xy) {
	POINT ret;
	ret.x = xy.topLeft.x + (xy.bottomRight.x - xy.topLeft.x) / 2;
	ret.y = xy.topLeft.y + (xy.bottomRight.y - xy.topLeft.y) / 2;
	return ret;
}

double CharacterCommon::CalcDistance(POINT xy1, POINT xy2) {
	return sqrt(pow(xy1.x - xy2.x, 2.0) + pow(xy1.y - xy2.y, 2.0));
}


}
}