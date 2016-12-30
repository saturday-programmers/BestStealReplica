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
	static void CreateChipTuTvs(int chipCount, int rowIdx, int colIdx, Vertices<FloatPoint> pRetArr[]);
	static void CreateTuTv(int rowIdx, int colIdx, Vertices<FloatPoint>* pRet);
	static void CountUpAnimationCnt(int* pAnimationCnt, int chipCntPerDir);
	static int GetAnimationNumber(int currentAnimationCnt);
	static void CreateDrawingVertices(const POINT& rTopLeftXY, void(*convertTopLeftToVertices)(const POINT&, Vertices<POINT>*), const Vertices<FloatPoint>& rChip, Vertices<Drawing::DrawingVertex>* pRet);
	static void ConvertTopLeftXYToVertices(const POINT& rTopLeftXY, Vertices<POINT>* pRet);
	static void CalcCharacterXY(const POINT& rTopLeftXY, int width, int height, Vertices<POINT>* pRet);
	static void CalcCenter(const Vertices<POINT>& rXY, POINT* pRet);
	static double CalcDistance(const POINT& rXY1, const POINT& rXY2);
	static bool IsOverlapping(const Vertices<POINT>& rXY1, const Vertices<POINT>& rXY2);

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