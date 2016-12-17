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
	static void CharacterCommon::CreateChipTuTvs(int chipCount, int rowIdx, int colIdx, Vertices<FloatPoint> chips[]);
	static Vertices<FloatPoint> CreateTuTv(int rowIdx, int colIdx);
	static void CountUpAnimationCnt(int* pAnimationCnt, int chipCntPerDir);
	static int GetAnimationNumber(int currentAnimationCnt);
	static Vertices<Drawing::DrawingVertex> CreateVertex(POINT topLeftXY, Vertices<POINT>(*getXY)(POINT), Vertices<FloatPoint> chip);
	static Vertices<POINT> GetChipXY(POINT topLeftXY);
	static POINT CalcCenter(Vertices<POINT> xy);
	static double CalcDistance(POINT xy1, POINT xy2);

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