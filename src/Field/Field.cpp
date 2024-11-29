#include "Field.hpp"

namespace Battleship {

bool Field::IsInBounds(uint64_t x, uint64_t y) const {
    return x < width_ && y < height_;
}

} // namespace Battleship
