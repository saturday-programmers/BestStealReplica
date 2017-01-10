#ifndef CHARACTER_COMMON_H_
#define CHARACTER_COMMON_H_

#include "../AppCommon.h"
#include "../Drawing/DrawingCommon.h"


namespace BestStealReplica {
namespace Character {

class CharacterCommon {
public:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int HEIGHT = 80;
	static const int WIDTH = 80;

	/* Static Functions --------------------------------------------------------------------------------- */
	static void CreateTexRect(int chipCount, int rowIdx, int colIdx, Rectangle<FloatPoint> pRetArr[]);
	static void CreateTexRect(int rowIdx, int colIdx, Rectangle<FloatPoint>* pRet);
	static void CountUpAnimationCnt(int* pAnimationCnt, int chipCntPerDir);
	static int GetAnimationNumber(int currentAnimationCnt);
	static void CreateDrawingVertexRect(const POINT& rTopLeftPoint, void(*convertTopLeftToRect)(const POINT&, Rectangle<POINT>*), const Rectangle<FloatPoint>& rChip, Rectangle<Drawing::DrawingVertex>* pRet);
	static void ConvertTopLeftPointToRect(const POINT& rTopLeftPoint, Rectangle<POINT>* pRet);
	static void CalcCharacterRect(const POINT& rTopLeftPoint, int width, int height, Rectangle<POINT>* pRet);
	static void CalcCenter(const Rectangle<POINT>& rRect, POINT* pRet);
	static double CalcDistance(const POINT& rPoint1, const POINT& rPoint2);
	static bool IsOverlapping(const Rectangle<POINT>& rRect1, const Rectangle<POINT>& rRect2);

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int CHIP_COUNT_PER_ROW = 6;
	static const int CHIP_COUNT_PER_COL = 6;

	/* Constructor / Destructor ------------------------------------------------------------------------- */
	CharacterCommon();
};

}
}

#endif