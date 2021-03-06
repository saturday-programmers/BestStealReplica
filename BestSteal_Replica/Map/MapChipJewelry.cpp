﻿#include "MapChipJewelry.h"


namespace BestStealReplica {
namespace Map {

/* Constructor / Destructor ------------------------------------------------------------------------- */
MapChipJewelry::MapChipJewelry() : MapChip(MapChipType::JEWELRY), state(State::CLOSED) {}


/* Getters / Setters -------------------------------------------------------------------------------- */
MapChipJewelry::State MapChipJewelry::GetState() const {
	return this->state;
}


/* Public Functions  -------------------------------------------------------------------------------- */
void MapChipJewelry::OpenBox() {
	if (this->state == MapChipJewelry::State::CLOSED) {
		++this->chipNumber;
		this->state = MapChipJewelry::State::OPENED;
		ConfigureTexRect();
	}
}

}
}