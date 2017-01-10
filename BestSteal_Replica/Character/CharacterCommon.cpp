#include "CharacterCommon.h"


namespace BestStealReplica {
namespace Character {

/* Static Public Functions -------------------------------------------------------------------------- */
void CharacterCommon::CreateTexRect(int chipCount, int rowIdx, int colIdx, Rectangle<FloatPoint> pRetArr[]) {
	for (int i = 0; i < chipCount; ++i) {
		CreateTexRect(rowIdx, colIdx + i, pRetArr + i);
	}
}

void CharacterCommon::CreateTexRect(int rowIdx, int colIdx, Rectangle<FloatPoint>* pRet) {
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

void CharacterCommon::CreateDrawingVertexRect(const POINT& rTopLeftPoint, void(*convertTopLeftToRect)(const POINT&, Rectangle<POINT>*), const Rectangle<FloatPoint>& rChip, Rectangle<Drawing::DrawingVertex>* pRet) {
	Rectangle<POINT> rect;
	convertTopLeftToRect(rTopLeftPoint, &rect);

	pRet->topLeft.x = rect.topLeft.x;
	pRet->topLeft.y = rect.topLeft.y;
	pRet->topLeft.tu = rChip.topLeft.x;
	pRet->topLeft.tv = rChip.topLeft.y;

	pRet->bottomRight.x = rect.bottomRight.x;
	pRet->bottomRight.y = rect.bottomRight.y;
	pRet->bottomRight.tu = rChip.bottomRight.x;
	pRet->bottomRight.tv = rChip.bottomRight.y;
}

void CharacterCommon::ConvertTopLeftPointToRect(const POINT& rTopLeftPoint, Rectangle<POINT>* pRet) {
	pRet->topLeft.x = rTopLeftPoint.x;
	pRet->topLeft.y = rTopLeftPoint.y;
	pRet->bottomRight.x = rTopLeftPoint.x + CharacterCommon::WIDTH - 1;
	pRet->bottomRight.y = rTopLeftPoint.y + CharacterCommon::HEIGHT - 1;
}

void CharacterCommon::CalcCharacterRect(const POINT& rTopLeftPoint, int width, int height, Rectangle<POINT>* pRet) {
	ConvertTopLeftPointToRect(rTopLeftPoint, pRet);
	int xDiff = (CharacterCommon::WIDTH - width) / 2;
	int yDiff = (CharacterCommon::HEIGHT - height) / 2;
	pRet->topLeft.x += xDiff;
	pRet->topLeft.y += yDiff;
	pRet->bottomRight.x -= xDiff;
	pRet->bottomRight.y -= yDiff;
}

void CharacterCommon::CalcCenter(const Rectangle<POINT>& rRect, POINT* pRet) {
	pRet->x = rRect.topLeft.x + (rRect.bottomRight.x - rRect.topLeft.x) / 2;
	pRet->y = rRect.topLeft.y + (rRect.bottomRight.y - rRect.topLeft.y) / 2;
}

double CharacterCommon::CalcDistance(const POINT& rPoint1, const POINT& rPoint2) {
	return sqrt(pow(rPoint1.x - rPoint2.x, 2.0) + pow(rPoint1.y - rPoint2.y, 2.0));
}

bool CharacterCommon::IsOverlapping(const Rectangle<POINT>& rRect1, const Rectangle<POINT>& rRect2) {
	return (rRect1.topLeft.x < rRect2.bottomRight.x && rRect2.topLeft.x < rRect1.bottomRight.x
		&& rRect1.topLeft.y < rRect2.bottomRight.y && rRect2.topLeft.y < rRect1.bottomRight.y);
}

}
}