#include "Field.hpp"

namespace Battleship {

bool Field::IsInBounds(uint64_t x, uint64_t y) const {
    return x < width_ && y < height_;
}

uint64_t Field::GetWidth() const {
    return width_;
}

uint64_t Field::GetHeight() const {
    return height_;
}

} // namespace Battleship
