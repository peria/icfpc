#pragma once

#include "wrapper.h"
#include "map.h"

namespace algorithm {

// Move |w| to the nearest non wrapped cell from its original position.
// Returns false if no non-wrapped cells are left in |m|.  Note once |w| decides
// the way to a non-wrapped cell, it goes there even if another wrapper wraps
// the destination cell.
bool goToNearestNonWrapped(Wrapper& w, Map& m);

}  // namespace algorithm
