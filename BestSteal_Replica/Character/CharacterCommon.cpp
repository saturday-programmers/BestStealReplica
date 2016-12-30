#include "CharacterCommon.h"


namespace BestStealReplica {
namespace Character {

/* Static Public Functions -------------------------------------------------------------------------- */
void CharacterCommon::CreateChipTuTvs(int chipCount, int rowIdx, int colIdx, Vertices<FloatPoint> pRetArr[]) {
	for (int i = 0; i < chipCount; ++i) {
		CreateTuTv(rowIdx, colIdx + i, pRetArr + i);
	}
}

void CharacterCommon::CreateTuTv(int rowIdx, int colIdx, Vertices<FloatPoint>* pRet) {
	pRet->topLeft.x = colIdx / (float)CharacterCommon::CHIP_COUNT_PER_COL;
	pRet->topLeft.y = rowIdx / (float)CharacterCommon::CHIP_COUNT_PER_ROW;
	pRet->bottomRight.x = (colIdx + 1) / (float)CharacterCommon::CHIP_COUNT_PER_COL;
	pRet->bottomRight.y = (rowIdx + 1) / (float)CharacterCommon::CHIP_COUNT_PER_ROW;
}

void CharacterCommon::CountUpAnimationCnt(int* pAnimationCnt, int chipCntPerDir) {
	++(*pAnimationCnt);
	if (*pAnimationCnt >= chipCntPerDir * AppCommon::FRAME_COUNT_PER_CUT) {
		*pAnimationCnt = 0;
	}
}

int CharacterCommon::GetAnimationNumber(int currentAnimationCnt) {
	return currentAnimationCnt / AppCommon::FRAME_COUNT_PER_CUT;
}

void CharacterCommon::CreateDrawingVertices(const POINT& rTopLeftXY, void(*convertTopLeftToVertices)(const POINT&, Vertices<POINT>*), const Vertices<FloatPoint>& rChip, Vertices<Drawing::DrawingVertex>* pRet) {
	Vertices<POINT> xy;
	convertTopLeftToVertices(rTopLeftXY, &xy);

	pRet->topLeft.x = xy.topLeft.x;
	pRet->topLeft.y = xy.topLeft.y;
	pRet->topLeft.tu = rChip.topLeft.x;
	pRet->topLeft.tv = rChip.topLeft.y;

	pRet->bottomRight.x = xy.bottomRight.x;
	pRet->bottomRight.y = xy.bottomRight.y;
	pRet->bottomRight.tu = rChip.bottomRight.x;
	pRet->bottomRight.tv = rChip.bottomRight.y;
}

void CharacterCommon::ConvertTopLeftXYToVertices(const POINT& rTopLeftXY, Vertices<POINT>* pRet) {
	pRet->topLeft.x = rTopLeftXY.x;
	pRet->topLeft.y = rTopLeftXY.y;
	pRet->bottomRight.x = rTopLeftXY.x + CharacterCommon::WIDTH - 1;
	pRet->bottomRight.y = rTopLeftXY.y + CharacterCommon::HEIGHT - 1;
}

void CharacterCommon::CalcCharacterXY(const POINT& rTopLeftXY, int width, int height, Vertices<POINT>* pRet) {
	ConvertTopLeftXYToVertices(rTopLeftXY, pRet);
	int xDiff = (CharacterCommon::WIDTH - width) / 2;
	int yDiff = (CharacterCommon::HEIGHT - height) / 2;
	pRet->topLeft.x += xDiff;
	pRet->topLeft.y += yDiff;
	pRet->bottomRight.x -= xDiff;
	pRet->bottomRight.y -= yDiff;
}

void CharacterCommon::CalcCenter(const Vertices<POINT>& rXY, POINT* pRet) {
	pRet->x = rXY.topLeft.x + (rXY.bottomRight.x - rXY.topLeft.x) / 2;
	pRet->y = rXY.topLeft.y + (rXY.bottomRight.y - rXY.topLeft.y) / 2;
}

double CharacterCommon::CalcDistance(const POINT& rXY1, const POINT& rXY2) {
	return sqrt(pow(rXY1.x - rXY2.x, 2.0) + pow(rXY1.y - rXY2.y, 2.0));
}

bool CharacterCommon::IsOverlapping(const Vertices<POINT>& rXY1, const Vertices<POINT>& rXY2) {
	return (rXY1.topLeft.x < rXY2.bottomRight.x && rXY2.topLeft.x < rXY1.bottomRight.x
		&& rXY1.topLeft.y < rXY2.bottomRight.y && rXY2.topLeft.y < rXY1.bottomRight.y);
}

}
}