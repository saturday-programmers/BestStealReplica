#include "MapChipDoor.h"


namespace BestStealReplica {
namespace Map {

MapChipDoor::MapChipDoor(MapCommon::MapChipType chipType) : MapChip(chipType),
	state(State::CLOSED),
	openingCount(0)
{}

void MapChipDoor::StartOpeningDoor() {
	this->state = State::START_OPENING;
}

void MapChipDoor::OpenDoor() {
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
				if (this->openingCount / AppCommon::FRAME_COUNT_PER_CUT == MapChipDoor::OPENING_CHIP_COUNT - 1) {
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