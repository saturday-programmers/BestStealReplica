#ifndef CHARACTER_COMMON_H_
#define CHARACTER_COMMON_H_

#include "AppCommon.h"


namespace BestStealReplica {
namespace Character {

class CharacterCommon {
public:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int HEIGHT = 80;
	static const int WIDTH = 80;

	/* Static Functions --------------------------------------------------------------------------------- */
	static void SetTuTvs(Vertices<FloatPoint> chips[], int chipCount, int rowNum, int colNum);
	static Vertices<FloatPoint> GetTuTv(int rowNum, int colNum);
	static void CountUpAnimationCnt(int* currentAnimationCnt, int chipCntPerDir);
	static int GetAnimationNumber(int currentAnimationCnt);
	static Vertices<DrawingVertex> GetVertex(POINT topLeftXY, Vertices<POINT>(*getXY)(POINT), Vertices<FloatPoint> chip);
	static Vertices<POINT> GetChipXY(POINT topLeftXY);
	static POINT CalcCenter(Vertices<POINT> xy);
	static double CalcDistance(POINT xy1, POINT xy2);

private:
	/* Constants ---------------------------------------------------------------------------------------- */
	static const int CHIP_COUNT_PER_ROW = 6;
	static const int CHIP_COUNT_PER_COL = 6;
};

}
}

#endif