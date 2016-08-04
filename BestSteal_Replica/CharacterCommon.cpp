#include "CharacterCommon.h"

namespace BestStealReplica {
namespace Character {
void CharacterCommon::SetTuTvs(Vertices<FloatPoint> chips[], int chipCntPerDir, int rowNum, int colNum) {
	for (int i = 0; i < chipCntPerDir; ++i) {
		chips[i].topLeft.x = (colNum + i) / (float)CharacterCommon::CHIP_COUNT_PER_COL;
		chips[i].topLeft.y = rowNum / (float)CharacterCommon::CHIP_COUNT_PER_ROW;
		chips[i].bottomRight.x = (colNum + i + 1) / (float)CharacterCommon::CHIP_COUNT_PER_COL;
		chips[i].bottomRight.y = (rowNum + 1) / (float)CharacterCommon::CHIP_COUNT_PER_ROW;
	}
}

void CharacterCommon::CountUpAnimationCnt(int* currentAnimationCnt, int chipCntPerDir) {
	++(*currentAnimationCnt);
	if (*currentAnimationCnt >= chipCntPerDir * CharacterCommon::FRAME_COUNT_PER_CUT) {
		*currentAnimationCnt = 0;
	}
}

int CharacterCommon::GetAnimationNumber(int currentAnimationCnt) {
	return currentAnimationCnt / CharacterCommon::FRAME_COUNT_PER_CUT;
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

}
}