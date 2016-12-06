#include "MapChipGate.h"


namespace BestStealReplica {
namespace Map {

/* Constructor / Destructor ------------------------------------------------------------------------- */
MapChipGate::MapChipGate(MapChipType chipType) : MapChip(chipType),
	state(State::CLOSED),
	openingCount(0)
{}


/* Getters / Setters -------------------------------------------------------------------------------- */
MapChipGate::State MapChipGate::GetState() const {
	return this->state;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void MapChipGate::StartOpeningGate() {
	this->state = State::START_OPENING;
}

void MapChipGate::OpenGate() {
	switch (this->state) {
		case State::START_OPENING:
			++this->openingCount;
			++this->chipNumber;
			this->state = State::OPENING;
			break;
		case State::OPENING:
			++this->openingCount;
			if (this->openingCount % AppCommon::FRAME_COUNT_PER_CUT == 0) {
				++this->chipNumber;
				if (this->openingCount / AppCommon::FRAME_COUNT_PER_CUT == MapChipGate::OPENING_CHIP_COUNT - 1) {
					this->state = State::OPENED;
				}
			}
			break;
		default:
			break;
	}
	SetTuTv();
}

}
}